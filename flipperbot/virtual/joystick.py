import tkinter as tk
from time import time
from ..controller import Controller

import socket
from .. import fbcp

fbcp.loadCommands()

class Joystick (tk.Canvas):
  RADIUS_BG1_F = 1.0/5
  RADIUS_BG2_F = 1.0
  BORDER_F     = 1.0/50
  RADIUS_IN_F  = 1.0/10
  RADIUS_OUT_F = 1.0/3
  RADIUS_MOV_F = 1.0/4  
  
  COLOR_UPPER  = 'red'
  COLOR_LOWER  = 'gray15'
  COLOR_BG1    = 'black'
  COLOR_BG2    = 'gray'
  COLOR_BG     = 'white'
  COLOR_BORDER = 'gray20'
  
  direction = None
  
  address = "192.168.1.1"
  port = 10000
    
  def __init__(self, serial, autostop=True, master=None):
    self.serial = fbcp.CONTR_PREFIX + str(serial)
    
    if master is None:
      self.root = tk.Tk()
    else:
      self.root = master
    tk.Canvas.__init__(self, master=self.root, bd=0, highlightthickness=0)
    if master is None:
      self.pack(fill=tk.BOTH, expand=tk.YES)
    
    self.box = self.create_rectangle(0, 0, 0, 0)
    self.out = self.create_oval(0, 0, 0, 0)
    self.low = self.create_oval(0, 0, 0, 0)
    self.stick = self.create_line(0, 0, 0, 0)
    self.ball = self.create_oval(0, 0, 0, 0)
    
    self.config()
    self.updateColours()
    
    self.connected = False
    
    self._mouse = False
    self._snap = False
    self._click = False
    self._autostop = autostop
    self.input()
    self.bind('<Configure>', self.config)
  
  directionKeys = {
    'Left'     : Controller.Direction.LEFT,
    'KP_4'     : Controller.Direction.LEFT,
    'KP_Left'  : Controller.Direction.LEFT,
    'Up'       : Controller.Direction.FORWARD,
    'KP_8'     : Controller.Direction.FORWARD,
    'KP_Up'    : Controller.Direction.FORWARD,
    'Right'    : Controller.Direction.RIGHT,
    'KP_6'     : Controller.Direction.RIGHT,
    'KP_Right' : Controller.Direction.RIGHT,
    'Down'     : Controller.Direction.BACKWARD,
    'KP_2'     : Controller.Direction.BACKWARD,
    'KP_Down'  : Controller.Direction.BACKWARD,
    'KP_5'     : Controller.Direction.STOP,
    'KP_Begin' : Controller.Direction.STOP,
    'KP_7'     : Controller.Direction.FORWARD_LEFT,
    'KP_Home'  : Controller.Direction.FORWARD_LEFT,
    'KP_9'     : Controller.Direction.FORWARD_RIGHT,
    'KP_Prior' : Controller.Direction.FORWARD_RIGHT,
    'KP_1'     : Controller.Direction.BACKWARD_LEFT,
    'KP_End'   : Controller.Direction.BACKWARD_LEFT,
    'KP_3'     : Controller.Direction.BACKWARD_RIGHT,
    'KP_Next'  : Controller.Direction.BACKWARD_RIGHT
  }
  
  def input(self):
    self.focus_set()
    self.enableKeyboard()
    self.bind('<Return>', lambda e: self.EBPressed())
    self.bind('<KeyRelease-Return>', lambda e: self.EBReleased())
    self.enableMouse()
    self.autostop(self._autostop)
  
  def enableKeyboard(self, value=True):
    if value:
      for k, v in self.directionKeys.items():
        def handler(d):
          return lambda e: self.setDirection(d)
        self.bind("<{}>".format(k), handler(v))
    else:
      for k, v in self.directionKeys.items():
        self.unbind("<{}>".format(k))
  
  def autostop(self, value=None):
    if value is None:
      return self._autostop
    else:
      self._autostop = value
      if self._autostop:
        for k in self.directionKeys.keys():
          self.bind('<KeyRelease-{}>'.format(k), lambda e: self.setDirection(Controller.Direction.STOP))
      else:
        for k in self.directionKeys.keys():
          self.unbind('<KeyRelease-{}>'.format(k))
  
  def enableMouse(self, value=True):
    self._mouse = value
    if value:
      if self._click:
        self.bind('<B1-Motion>', self.mouseHandler)
        self.bind('<Button>', self.mouseHandler)
      else:
        self.bind('<Motion>', self.mouseHandler)
    else:
      self.unbind('<B1-Motion>')
      self.unbind('<Motion>')
      self.unbind('<Button1>')
  
  def enableSnap(self, value=True):
    self._snap = value
  
  def requireClick(self, value=True):
    self._click = value
    if self._mouse:
      self.enableMouse(False)
      self.enableMouse(True)
    
  def mouseHandler(self, e):
    x = (e.x-self.cx)/self.RADIUS_MOV
    y = (e.y-self.cy)/self.RADIUS_MOV    
    if (x*x+y*y) > 1:
      norm = (x*x+y*y)**0.5
      x = x/norm
      y = y/norm
    
    
    self.draw([x,y])
    if y < -0.3:
      if x < -0.3:
        self.setDirection(Controller.Direction.FORWARD_LEFT, redraw=self._snap)
      elif x > 0.3:
        self.setDirection(Controller.Direction.FORWARD_RIGHT, redraw=self._snap)
      else:
        self.setDirection(Controller.Direction.FORWARD, redraw=self._snap)
    elif y > 0.3:
      if x < -0.3:
        self.setDirection(Controller.Direction.BACKWARD_LEFT, redraw=self._snap)
      elif x > 0.3:
        self.setDirection(Controller.Direction.BACKWARD_RIGHT, redraw=self._snap)
      else:
        self.setDirection(Controller.Direction.BACKWARD, redraw=self._snap)
    else:
      if x < -0.3:
        self.setDirection(Controller.Direction.LEFT, redraw=self._snap)
      elif x > 0.3:
        self.setDirection(Controller.Direction.RIGHT, redraw=self._snap)
      else:
        self.setDirection(Controller.Direction.STOP, redraw=self._snap)
  
  def setDirection(self, direction,redraw=True):
    if self.direction != direction:
      self.direction = direction
      self.sendDirection()
    if redraw:
        self.redraw()
  
  def draw(self, pos):
    Cx = self.cx+self.RADIUS_MOV*pos[0]
    Cy = self.cy+self.RADIUS_MOV*pos[1]
    Left = Cx-self.RADIUS_IN/2
    Right = Cx+self.RADIUS_IN/2
    Top = Cy-self.RADIUS_IN/2
    Bottom = Cy+self.RADIUS_IN/2
    
    self.coords(self.stick, self.cx, self.cy, Cx, Cy)
    self.coords(self.ball, Cx-self.RADIUS_OUT/2, Cy-self.RADIUS_OUT/2, Cx+self.RADIUS_OUT/2, Cy+self.RADIUS_OUT/2)
  
  def redraw(self):
    if self.direction == Controller.Direction.STOP:
      self.draw((0, 0))
    elif self.direction == Controller.Direction.FORWARD:
      self.draw((0, -1))
    elif self.direction == Controller.Direction.BACKWARD:
      self.draw((0, 1))
    elif self.direction == Controller.Direction.LEFT:
      self.draw((-1, 0))
    elif self.direction == Controller.Direction.RIGHT:
      self.draw((1, 0))
    elif self.direction == Controller.Direction.FORWARD_LEFT:
      self.draw((-0.7, -0.7))
    elif self.direction == Controller.Direction.FORWARD_RIGHT:
      self.draw((0.7, -0.7))
    elif self.direction == Controller.Direction.BACKWARD_LEFT:
      self.draw((-0.7, 0.7))
    elif self.direction == Controller.Direction.BACKWARD_RIGHT:
      self.draw((0.7, 0.7))
    else:
      self.draw((0, 0))

  def config(self, event=None):
    self.w = self.winfo_width()
    self.h = self.winfo_height()
    dim = min(self.w, self.h)
    self.RADIUS_BG1 = dim*self.RADIUS_BG1_F
    self.BORDER  = dim*self.BORDER_F
    self.RADIUS_BG2 = dim*self.RADIUS_BG2_F-self.BORDER
    self.RADIUS_OUT = dim*self.RADIUS_OUT_F
    self.RADIUS_IN  = dim*self.RADIUS_IN_F
    self.RADIUS_MOV = dim*self.RADIUS_MOV_F
    
    self.cx = self.w/2
    self.cy = self.h/2
    self.left = self.cx-self.RADIUS_IN/2
    self.right = self.cx+self.RADIUS_IN/2
    self.top = self.cy-self.RADIUS_IN/2
    self.bottom = self.cy+self.RADIUS_IN/2
    
    self.coords(self.box, self.cx-self.RADIUS_BG2/2, self.cy-self.RADIUS_BG2/2, self.cx+self.RADIUS_BG2/2, self.cy+self.RADIUS_BG2/2)
    self.itemconfigure(self.box, width=self.BORDER)
    self.coords(self.out, self.cx-self.RADIUS_BG1/2, self.cy-self.RADIUS_BG1/2, self.cx+self.RADIUS_BG1/2, self.cy+self.RADIUS_BG1/2)
    self.itemconfigure(self.out, width=0)
    self.coords(self.low, self.left, self.top, self.right, self.bottom)
    self.itemconfigure(self.low, width=0)
    self.itemconfigure(self.stick, width=self.RADIUS_IN)
    
    self.redraw()
  
  def updateColours(self):
    self['bg'] = self.COLOR_BG
    self.itemconfigure(self.box, fill=self.COLOR_BG2, outline=self.COLOR_BORDER)
    self.itemconfigure(self.out, fill=self.COLOR_BG1, outline='black')
    self.itemconfigure(self.low, fill=self.COLOR_LOWER, outline=self.COLOR_LOWER)
    self.itemconfigure(self.stick, fill=self.COLOR_LOWER)
    self.itemconfigure(self.ball, fill=self.COLOR_UPPER, outline='black')
  
  pressRequest = False
  releaseRequest = True
  
  def EBPressed(self):
    if not self.pressRequest:
      self.sendEB(True)
    self.pressRequest = True
    self.releaseRequest = False
  
  def EBReleased(self):
    self.releaseRequest = True
    self.after(50, self.tryReleaseEB)
  
  def tryReleaseEB(self):
    if self.releaseRequest:
      self.releaseRequest = False
      self.pressRequest = False
      self.sendEB(False)
  
  def sendEB(self, val):
    if self.connected:
      cmd = fbcp.CommandLine()
      if val:
        cmd.command = fbcp.Command.Q_EVERYTHING_ON
      else:
        cmd.command = fbcp.Command.Q_EVERYTHING_OFF
      self.socket.send(cmd.write().encode('utf-8'))
  
  def sendDirection(self):
    if self.connected:
      cmd = fbcp.CommandLine()
      cmd.command = fbcp.Command.Q_ROBOT_COMMAND
      if self.direction == Controller.Direction.STOP:
        cmd.params['direction'] = fbcp.Param.DIRECTION_STOP.id
      elif self.direction == Controller.Direction.FORWARD:
        cmd.params['direction'] = fbcp.Param.DIRECTION_FORWARD.id
      elif self.direction == Controller.Direction.BACKWARD:
        cmd.params['direction'] = fbcp.Param.DIRECTION_BACKWARD.id
      elif self.direction == Controller.Direction.LEFT:
        cmd.params['direction'] = fbcp.Param.DIRECTION_LEFT.id
      elif self.direction == Controller.Direction.RIGHT:
        cmd.params['direction'] = fbcp.Param.DIRECTION_RIGHT.id
      elif self.direction == Controller.Direction.FORWARD_LEFT:
        cmd.params['direction'] = fbcp.Param.DIRECTION_FORWARD_LEFT.id
      elif self.direction == Controller.Direction.FORWARD_RIGHT:
        cmd.params['direction'] = fbcp.Param.DIRECTION_FORWARD_RIGHT.id
      elif self.direction == Controller.Direction.BACKWARD_LEFT:
        cmd.params['direction'] = fbcp.Param.DIRECTION_BACKWARD_LEFT.id
      elif self.direction == Controller.Direction.BACKWARD_RIGHT:
        cmd.params['direction'] = fbcp.Param.DIRECTION_BACKWARD_RIGHT.id
      else:
        cmd.params['direction'] = fbcp.Param.DIRECTION_STOP.id
      self.socket.send(cmd.write().encode('utf-8'))
  
  def connect(self):    
    try:
      self.socket = socket.socket(socket.AF_INET, socket.SOCK_STREAM)
      self.socket.connect((self.address, self.port))
      cmd = fbcp.CommandLine()
      cmd.command = fbcp.Command.Q_SINGLE_PRESENTATION
      cmd.params['serial'] = self.serial
      self.socket.send(cmd.write().encode('utf-8'))
      
      buf = self.socket.recv(256).decode("utf-8")
      if cmd.parse(buf):
        if cmd.command == fbcp.Command.A_GRANT_ACCESS:
          self.connected = True
          return True
        else:
          print("Wrong answer")
      else:
        print("Can't understand answer")
    except Exception as e:
      print(e)
    return False
  
  def disconnect(self):
    self.connected = False
    self.socket.close()


class JoystickContainer (tk.Frame):
  def __init__(self, serial, autostop=True, master=None):
    if master is None:
      self.root = tk.Tk()
    else:
      self.root = master
    tk.Frame.__init__(self, master=self.root)
    if master is None:
      self.pack(fill=tk.BOTH, expand=tk.YES)
    
    self.joystick = Joystick(serial, autostop, self)
    
    self.btnConnect = tk.Button(self, text="Connect")
    self.btnConnect.grid(row=1, column=1, columnspan=2, sticky='nsew')
    self.btnConnect.bind('<Button>', self.connect)
    
    self.rowconfigure(2, weight=1)

    self.options = tk.Frame(self)
    self.options.grid(row=2, column=2, sticky='nsew')
    self.options.columnconfigure(1, weight=1)
    #~ for i in range(1, 9):
      #~ self.options.rowconfigure(i, weight=1)
    
    self.lblKeyboard = tk.Label(self.options, text="Keyboard control:")
    self.lblKeyboard.grid(row=1, column=1, sticky='w')
    
    self.varKeyboard = tk.IntVar()
    self.chkKeyboard = tk.Checkbutton(self.options, text="Enable keyboard", variable=self.varKeyboard, command=self.onOptionChange)
    self.chkKeyboard.grid(row=2, column=1, sticky='w')
    self.chkKeyboard.hint = "Use the arrow keys and the number pad to move the controller"
    self.chkKeyboard.bind('<Enter>', self.tooltipOn)
    self.chkKeyboard.bind('<Leave>', self.tooltipOff)
    
    self.varSticky = tk.IntVar()
    self.chkSticky = tk.Checkbutton(self.options, text="Enable sticky keys", variable=self.varSticky, command=self.onOptionChange)
    self.chkSticky.grid(row=3, column=1, sticky='w')
    self.chkSticky.hint = "Keep the same direction when releasing the key"
    self.chkSticky.bind('<Enter>', self.tooltipOn)
    self.chkSticky.bind('<Leave>', self.tooltipOff)
    
    self.lblMouse = tk.Label(self.options, text="Mouse control:")
    self.lblMouse.grid(row=4, column=1, sticky='w')
    
    self.varMouse= tk.IntVar()
    self.chkMouse = tk.Checkbutton(self.options, text="Enable mouse", variable=self.varMouse, command=self.onOptionChange)
    self.chkMouse.grid(row=5, column=1, sticky='w')
    self.chkMouse.hint = "Use the mouse to move the controller"
    self.chkMouse.bind('<Enter>', self.tooltipOn)
    self.chkMouse.bind('<Leave>', self.tooltipOff)
    
    self.varSnap= tk.IntVar()
    self.chkSnap = tk.Checkbutton(self.options, text="Snap to default direction", variable=self.varSnap, command=self.onOptionChange)
    self.chkSnap.grid(row=6, column=1, sticky='w')
    self.chkSnap.hint = "Only allow predefined positions for the controller"
    self.chkSnap.bind('<Enter>', self.tooltipOn)
    self.chkSnap.bind('<Leave>', self.tooltipOff)
    
    self.varClick= tk.IntVar()
    self.chkClick = tk.Checkbutton(self.options, text="Require click", variable=self.varClick, command=self.onOptionChange)
    self.chkClick.grid(row=7, column=1, sticky='w')
    self.chkClick.hint = "Follow the mouse only when the left button is pressed"
    self.chkClick.bind('<Enter>', self.tooltipOn)
    self.chkClick.bind('<Leave>', self.tooltipOff)
    
    self.columnconfigure(1, weight=1)
    self.rowconfigure(2, weight=1)
    self.joystick.grid(row=2, column=1, sticky='nsew')
    
    self.lblInfo = tk.Label(self, text="")
    self.lblInfo.grid(row=3, column=1, columnspan=2, sticky='nsew')
    
    self.onOptionChange()
  
  def onOptionChange(self):
    if self.varKeyboard.get():
      self.joystick.enableKeyboard(True)
      self.chkSticky.configure(state='normal')
    else:
      self.joystick.enableKeyboard(False)
      self.chkSticky.configure(state='disabled')
    
    if self.chkSticky['state'] == 'disabled':
      self.joystick.autostop(False)
    elif self.varSticky.get():
      self.joystick.autostop(False)
    else:
      self.joystick.autostop(True)
    
    if self.varMouse.get():
      self.joystick.enableMouse(True)
      self.chkSnap.configure(state='normal')
      self.chkClick.configure(state='normal')
    else:
      self.joystick.enableMouse(False)
      self.chkSnap.configure(state='disabled')
      self.chkClick.configure(state='disabled')
    
    if self.chkSnap['state'] == 'disabled':
      self.joystick.enableSnap(False)
    elif self.varSnap.get():
      self.joystick.enableSnap(True)
    else:
      self.joystick.enableSnap(False)
    
    if self.chkClick['state'] == 'disabled':
      self.joystick.requireClick(False)
    elif self.varClick.get():
      self.joystick.requireClick(True)
    else:
      self.joystick.requireClick(False)
  
  def tooltipOn(self, e):
    self.lblInfo['text'] = e.widget.hint
  
  def tooltipOff(self, e):
    self.lblInfo['text'] = '<Hint>'
  
  def connect(self, e=None):
    self.btnConnect.unbind('<Button>')
    self.btnConnect['text'] = "Connecting"
    if self.joystick.connect():
      self.btnConnect['text'] = 'Disconnect'
      self.btnConnect.bind('<Button>', self.disconnect)
    else:
      self.btnConnect['text'] = "Failed: Retry"
      self.btnConnect.bind('<Button>', self.connect)
  
  def disconnect(self, e=None):
    self.btnConnect.unbind('<Button>')
    self.btnConnect['text'] = "Connect"
    self.joystick.disconnect()
    self.btnConnect.bind('<Button>', self.connect)
