import tkinter as tk
from ..controller import Controller
from collections import OrderedDict as OD

class FakeJoystick (tk.Canvas):
  RADIUS_BG1_F = 1.0/5
#  RADIUS_BG2_F = 4.0/5
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
  
  def __init__(self, game, input=True, autostop=True, master=None):
    self.game = game
    if master is None:
      self.root = tk.Tk()
    else:
      self.root = master
    tk.Canvas.__init__(self, master=self.root, bd=0, highlightthickness=0)
    if master is None:
      self.pack(fill=tk.BOTH, expand=tk.YES)
    self._autostop = autostop
    self.input(input)
    self.bind('<Configure>', self.config)
    
    self.box = self.create_rectangle(0, 0, 0, 0)
    self.out = self.create_oval(0, 0, 0, 0)
    self.low = self.create_oval(0, 0, 0, 0)
    self.stick = self.create_line(0, 0, 0, 0)
    self.ball = self.create_oval(0, 0, 0, 0)
    
    self.config()
    self.updateColours()
    self.loop()
  
  def input(self, value=None):
    if value is None:
      return self._input
    else:
      self._input = value
    if self._input:
      self.focus_set()
      self.bind('<Left>', lambda e: self.setDirection(Controller.Direction.LEFT))
      self.bind('<KP_4>', lambda e: self.setDirection(Controller.Direction.LEFT))
      self.bind('<Up>', lambda e: self.setDirection(Controller.Direction.FORWARD))
      self.bind('<KP_8>', lambda e: self.setDirection(Controller.Direction.FORWARD))
      self.bind('<Right>', lambda e: self.setDirection(Controller.Direction.RIGHT))
      self.bind('<KP_6>', lambda e: self.setDirection(Controller.Direction.RIGHT))
      self.bind('<Down>', lambda e: self.setDirection(Controller.Direction.BACKWARD))
      self.bind('<KP_2>', lambda e: self.setDirection(Controller.Direction.BACKWARD))
      self.bind('<KP_5>', lambda e: self.setDirection(Controller.Direction.STOP))
      self.bind('<KP_7>', lambda e: self.setDirection(Controller.Direction.FORWARD_LEFT))
      self.bind('<KP_9>', lambda e: self.setDirection(Controller.Direction.FORWARD_RIGHT))
      self.bind('<KP_1>', lambda e: self.setDirection(Controller.Direction.BACKWARD_LEFT))
      self.bind('<KP_3>', lambda e: self.setDirection(Controller.Direction.BACKWARD_RIGHT))
      self.autostop(self._autostop)
    else:
      self.unbind('<Left>')
      self.unbind('<KP_4>')
      self.unbind('<Up>')
      self.unbind('<KP_8>')
      self.unbind('<Right>')
      self.unbind('<KP_6>')
      self.unbind('<Down>')
      self.unbind('<KP_2>')
      self.unbind('<KP_5>')
      self.unbind('<KP_7>')
      self.unbind('<KP_9>')
      self.unbind('<KP_1>')
      self.unbind('<KP_3>')
      self.autostop(False)
  
  def autostop(self, value=None):
    if value is None:
      return self._autostop
    else:
      self._autostop = value
      if self._autostop:
        self.bind('<KeyRelease>', lambda e: self.setDirection(Controller.Direction.STOP))
      else:
        self.unbind('<KeyRelease>')
  
  def setDirection(self, direction):
    self.game.controllers[0].direction = direction
  
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
  
  def loop(self):
    old_dir = self.direction
    self.direction = self.game.controllers[0].direction
    if self.direction != old_dir:
      self.redraw()
    self.root.after(50, self.loop)
  
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
    self.coords(self.low, self.left, self.top, self.right, self.bottom)
    self.itemconfigure(self.stick, width=self.RADIUS_IN)
    
    self.redraw()
  
  def updateColours(self):
    self['bg'] = self.COLOR_BG
    self.itemconfigure(self.box, fill=self.COLOR_BG2, outline=self.COLOR_BORDER)
    self.itemconfigure(self.out, fill=self.COLOR_BG1, outline='black')
    self.itemconfigure(self.low, fill=self.COLOR_LOWER, outline=self.COLOR_LOWER)
    self.itemconfigure(self.stick, fill=self.COLOR_LOWER)
    self.itemconfigure(self.ball, fill=self.COLOR_UPPER, outline='black')

class VirtualJoystick (tk.Canvas):
  RADIUS_BG1_F = 1.0/5
#  RADIUS_BG2_F = 4.0/5
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
  
  def __init__(self, game, input=True, autostop=True, master=None):
    self.game = game
    if master is None:
      self.root = tk.Tk()
    else:
      self.root = master
    tk.Canvas.__init__(self, master=self.root, bd=0, highlightthickness=0)
    if master is None:
      self.pack(fill=tk.BOTH, expand=tk.YES)
    self._autostop = autostop
    self.input(input)
    self.bind('<Configure>', self.config)
    
    self.box = self.create_rectangle(0, 0, 0, 0)
    
    self.controls = OD([
      (x, self.create_rectangle(0, 0, 0, 0, width=0, outline='red'))
      for x in [
        Controller.Direction.FORWARD_LEFT,
        Controller.Direction.FORWARD,
        Controller.Direction.FORWARD_RIGHT,
        Controller.Direction.LEFT,
        Controller.Direction.STOP,
        Controller.Direction.RIGHT,
        Controller.Direction.BACKWARD_LEFT,
        Controller.Direction.BACKWARD,
        Controller.Direction.BACKWARD_RIGHT
      ]
    ])
    self.selected = 0
    
    self.out = self.create_oval(0, 0, 0, 0)
    self.low = self.create_oval(0, 0, 0, 0)
    self.stick = self.create_line(0, 0, 0, 0)
    self.ball = self.create_oval(0, 0, 0, 0)
    
    self.config()
    self.updateColours()
    self.loop()
    self.controlLoop()
  
  def input(self, value=None):
    if value is None:
      return self._input
    else:
      self._input = value
    if self._input:
      self.focus_set()
      self.bind('<Left>',  lambda e: self.setDirection(Controller.Direction.LEFT))
      self.bind('<KP_4>',  lambda e: self.setDirection(Controller.Direction.LEFT))
      self.bind('<Up>',    lambda e: self.setDirection(Controller.Direction.FORWARD))
      self.bind('<KP_8>',  lambda e: self.setDirection(Controller.Direction.FORWARD))
      self.bind('<Right>', lambda e: self.setDirection(Controller.Direction.RIGHT))
      self.bind('<KP_6>',  lambda e: self.setDirection(Controller.Direction.RIGHT))
      self.bind('<Down>',  lambda e: self.setDirection(Controller.Direction.BACKWARD))
      self.bind('<KP_2>',  lambda e: self.setDirection(Controller.Direction.BACKWARD))
      self.bind('<KP_5>',  lambda e: self.setDirection(Controller.Direction.STOP))
      self.bind('<KP_7>',  lambda e: self.setDirection(Controller.Direction.FORWARD_LEFT))
      self.bind('<KP_9>',  lambda e: self.setDirection(Controller.Direction.FORWARD_RIGHT))
      self.bind('<KP_1>',  lambda e: self.setDirection(Controller.Direction.BACKWARD_LEFT))
      self.bind('<KP_3>',  lambda e: self.setDirection(Controller.Direction.BACKWARD_RIGHT))
      self.bind('<Return>', lambda e: self.setDirection(list(self.controls.keys())[self.selected]))
      self.autostop(self._autostop)
    else:
      self.unbind('<Left>')
      self.unbind('<KP_4>')
      self.unbind('<Up>')
      self.unbind('<KP_8>')
      self.unbind('<Right>')
      self.unbind('<KP_6>')
      self.unbind('<Down>')
      self.unbind('<KP_2>')
      self.unbind('<KP_5>')
      self.unbind('<KP_7>')
      self.unbind('<KP_9>')
      self.unbind('<KP_1>')
      self.unbind('<KP_3>')
      self.unbind('<Return>')
      self.autostop(False)
  
  def autostop(self, value=None):
    if value is None:
      return self._autostop
    else:
      self._autostop = value
      if self._autostop:
        self.bind('<KeyRelease>', lambda e: self.setDirection(Controller.Direction.STOP))
      else:
        self.unbind('<KeyRelease>')
  
  def setDirection(self, direction):
    self.game.controllers[0].direction = direction
  
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
  
  def loop(self):
    old_dir = self.direction
    self.direction = self.game.controllers[0].direction
    if self.direction != old_dir:
      self.redraw()
    self.root.after(50, self.loop)
  
  def controlLoop(self):
    if self.selected is not None:
      self.itemconfigure(list(self.controls.values())[self.selected], width=0)
    self.selected = (self.selected+1)%9
    self.itemconfigure(list(self.controls.values())[self.selected], width=5)
    self.root.after(500, self.controlLoop)
  
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
    self.coords(self.low, self.left, self.top, self.right, self.bottom)
    self.itemconfigure(self.stick, width=self.RADIUS_IN)

    for d, c in self.controls.items():
      if d == Controller.Direction.STOP:
        pos = (0, 0)
      elif d == Controller.Direction.FORWARD:
        pos = (0, -1)
      elif d == Controller.Direction.BACKWARD:
        pos = (0, 1)
      elif d == Controller.Direction.LEFT:
        pos = (-1, 0)
      elif d == Controller.Direction.RIGHT:
        pos = (1, 0)
      elif d == Controller.Direction.FORWARD_LEFT:
        pos = (-1, -1)
      elif d == Controller.Direction.FORWARD_RIGHT:
        pos = (1, -1)
      elif d == Controller.Direction.BACKWARD_LEFT:
        pos = (-1, 1)
      elif d == Controller.Direction.BACKWARD_RIGHT:
        pos = (1, 1)
      else:
        pos = (0, 0)
      self.coords(
        c,
        pos[0]*self.w/3+self.w/3,
        pos[1]*self.h/3+self.h/3,
        pos[0]*self.w/3+self.w*2/3,
        pos[1]*self.h/3+self.h*2/3
      )
    
    self.redraw()
  
  def updateColours(self):
    self['bg'] = self.COLOR_BG
    self.itemconfigure(self.box, fill=self.COLOR_BG2, outline=self.COLOR_BORDER)
    self.itemconfigure(self.out, fill=self.COLOR_BG1, outline='black')
    self.itemconfigure(self.low, fill=self.COLOR_LOWER, outline=self.COLOR_LOWER)
    self.itemconfigure(self.stick, fill=self.COLOR_LOWER)
    self.itemconfigure(self.ball, fill=self.COLOR_UPPER, outline='black')
