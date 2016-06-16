import tkinter as tk
from .controller import Controller

class FakeJoystick:
  RADIUS_BG1_F = 1.0/5
  RADIUS_BG2_F = 4.0/5
  RADIUS_IN_F  = 1.0/10
  RADIUS_OUT_F = 1.0/3
  RADIUS_MOV_F = 1.0/5
  
  UPPER_COLOR = 'red'
  LOWER_COLOR = 'gray15'
  BG1_COLOR   = 'black'
  BG2_COLOR   = 'gray'
  BG_COLOR    = 'white'
  
  def __init__(self, game):
    self.game = game
    self.root = tk.Tk()
    self.canvas = tk.Canvas(self.root)
    self.canvas['bg'] = self.BG_COLOR
    self.canvas.pack(fill=tk.BOTH, expand=tk.YES)
  
  def draw(self, pos):
    self.canvas.delete(tk.ALL)
    
    self.w = self.canvas.winfo_width()
    self.h = self.canvas.winfo_height()
    self.RADIUS_BG1 = min(self.w, self.h)*self.RADIUS_BG1_F
    self.RADIUS_BG2 = min(self.w, self.h)*self.RADIUS_BG2_F
    self.RADIUS_OUT = min(self.w, self.h)*self.RADIUS_OUT_F
    self.RADIUS_IN  = min(self.w, self.h)*self.RADIUS_IN_F
    self.RADIUS_MOV = min(self.w, self.h)*self.RADIUS_MOV_F
    
    cx = self.w/2
    cy = self.h/2
    left = cx-self.RADIUS_IN/2
    right = cx+self.RADIUS_IN/2
    top = cy-self.RADIUS_IN/2
    bottom = cy+self.RADIUS_IN/2
    self.canvas.create_rectangle(cx-self.RADIUS_BG2/2, cy-self.RADIUS_BG2/2, cx+self.RADIUS_BG2/2, cy+self.RADIUS_BG2/2, fill=self.BG2_COLOR, outline='black')
    self.canvas.create_oval(cx-self.RADIUS_BG1/2, cy-self.RADIUS_BG1/2, cx+self.RADIUS_BG1/2, cy+self.RADIUS_BG1/2, fill=self.BG1_COLOR, outline='black')
    self.canvas.create_oval(left, top, right, bottom, fill=self.LOWER_COLOR, outline=self.LOWER_COLOR)
    
    Cx = cx+self.RADIUS_MOV*pos[0]
    Cy = cy+self.RADIUS_MOV*pos[1]
    Left = Cx-self.RADIUS_IN/2
    Right = Cx+self.RADIUS_IN/2
    Top = Cy-self.RADIUS_IN/2
    Bottom = Cy+self.RADIUS_IN/2
    #if pos[0] != 0:
    #  self.canvas.create_polygon(cx, bottom, cx, top, Cx, Top, Cx, Bottom, fill=self.LOWER_COLOR, outline=self.LOWER_COLOR)
    #elif pos[1] != 0:
    #  self.canvas.create_polygon(left, cy, right, cy, Right, Cy, Left, Cy, fill=self.LOWER_COLOR, outline=self.LOWER_COLOR)
    self.canvas.create_line(cx, cy, Cx, Cy, width=self.RADIUS_IN, fill=self.LOWER_COLOR)
    self.canvas.create_oval(Cx-self.RADIUS_OUT/2, Cy-self.RADIUS_OUT/2, Cx+self.RADIUS_OUT/2, Cy+self.RADIUS_OUT/2, fill=self.UPPER_COLOR, outline='black')
  
  def loop(self):
    d = self.game.controllers[0].direction
    if d == Controller.Direction.STOP:
      self.draw((0, 0))
    elif d == Controller.Direction.FORWARD:
      self.draw((0, -1))
    elif d == Controller.Direction.BACKWARD:
      self.draw((0, 1))
    elif d == Controller.Direction.LEFT:
      self.draw((-1, 0))
    elif d == Controller.Direction.RIGHT:
      self.draw((1, 0))
    elif d == Controller.Direction.FORWARD_LEFT:
      self.draw((-0.7, -0.7))
    elif d == Controller.Direction.FORWARD_RIGHT:
      self.draw((0.7, -0.7))
    elif d == Controller.Direction.BACKWARD_LEFT:
      self.draw((-0.7, 0.7))
    elif d == Controller.Direction.BACKWARD_RIGHT:
      self.draw((0.7, 0.7))
    self.root.after(50, self.loop)
