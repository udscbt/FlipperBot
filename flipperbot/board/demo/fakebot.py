import tkinter as tk
from ...robot import Robot
from collections import OrderedDict as OD

from ..debug.debug import Debug

class FakeBot (tk.Canvas):
  RADIUS_MOV1_F = 1.0/10
  RADIUS_MOV2_F = 1.0/3
  SIZE_F        = 1.0/30
  
  COLOR_LINE  = 'red'
  COLOR_BG = 'white'
  
  direction = None
  
  def __init__(self, game, index=0, master=None):
    self.game = game
    self.index = index
    self.debug = Debug(
      log=game.debug.log,
      logging=game.debug.logging,
      stdout=game.debug.stdout,
      parent=self,
      name="FakeRobot<{}>".format(self.index)
    )
    self.debug("FakeRobot used")
    if master is None:
      self.root = tk.Tk()
    else:
      self.root = master
    tk.Canvas.__init__(self, master=self.root, bd=0, highlightthickness=0)
    if master is None:
      self.pack(fill=tk.BOTH, expand=tk.YES)
    self.bind('<Configure>', self.config)
    
    self.line = self.create_line(0, 0, 0, 0)
    self.dot  = self.create_oval(0, 0, 0, 0, width=0)
    
    self.config()
    self.updateColours()
    self.loop()
  
  def draw(self, pos):
    if pos == (0,0):
      self.itemconfigure(self.dot, fill=self.COLOR_LINE)
    else:
      self.itemconfigure(self.dot, fill=self.COLOR_BG)
    self.coords(self.line, self.cx+pos[0]*self.RADIUS_MOV1, self.cy+pos[1]*self.RADIUS_MOV1, self.cx+pos[0]*self.RADIUS_MOV2, self.cy+pos[1]*self.RADIUS_MOV2)
  
  def redraw(self):
    if self.direction == Robot.Direction.STOP:
      self.draw((0, 0))
    elif self.direction == Robot.Direction.FORWARD:
      self.draw((0, -1))
    elif self.direction == Robot.Direction.BACKWARD:
      self.draw((0, 1))
    elif self.direction == Robot.Direction.LEFT:
      self.draw((-1, 0))
    elif self.direction == Robot.Direction.RIGHT:
      self.draw((1, 0))
    elif self.direction == Robot.Direction.FORWARD_LEFT:
      self.draw((-0.7, -0.7))
    elif self.direction == Robot.Direction.FORWARD_RIGHT:
      self.draw((0.7, -0.7))
    elif self.direction == Robot.Direction.BACKWARD_LEFT:
      self.draw((-0.7, 0.7))
    elif self.direction == Robot.Direction.BACKWARD_RIGHT:
      self.draw((0.7, 0.7))
    else:
      self.draw((0, 0))
  
  def loop(self):
    old_dir = self.direction
    self.direction = self.game.robots[self.index].direction
    if self.direction != old_dir:
      self.debug("Direction changed from {} to {}".format(
        Robot.Direction.getName(old_dir),
        Robot.Direction.getName(self.direction)
      ))
      self.redraw()
    self.root.after(50, self.loop)
  
  def config(self, event=None):
    self.w = self.winfo_width()
    self.h = self.winfo_height()
    dim = min(self.w, self.h)
    self.RADIUS_MOV1 = dim*self.RADIUS_MOV1_F
    self.RADIUS_MOV2 = dim*self.RADIUS_MOV2_F
    self.SIZE = dim*self.SIZE_F
    
    self.cx = self.w/2
    self.cy = self.h/2
    
    self.itemconfigure(self.line, width=self.SIZE)
    self.coords(self.dot, self.cx-self.SIZE/2, self.cy-self.SIZE/2, self.cx+self.SIZE/2, self.cy+self.SIZE/2)
    
    self.redraw()
  
  def updateColours(self):
    self['bg'] = self.COLOR_BG
    self.itemconfigure(self.line, fill=self.COLOR_LINE)
    self.itemconfigure(self.dot, fill=self.COLOR_LINE)
