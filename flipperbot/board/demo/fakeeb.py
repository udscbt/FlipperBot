import tkinter as tk
from time import time

class FakeEB (tk.Canvas):
  RADIUS_OUT_F = 5/6
  RADIUS_IN1_F = 4/6
  RADIUS_IN2_F = 4/7
  
  COLOR_OUT  = 'gray20'
  COLOR_IN1_P = 'red3'
  COLOR_IN1_A = 'red1'
  COLOR_IN2_P = 'black'
  COLOR_IN2_A = 'gray3'
  COLOR_BTN1  = COLOR_IN1_P
  COLOR_BTN2  = COLOR_IN2_P
  COLOR_BG   = 'gray70'
  
  def __init__(self, game, master=None):
    self.game = game
    if master is None:
      self.root = tk.Tk()
    else:
      self.root = master
    tk.Canvas.__init__(self, master=self.root, bd=0, highlightthickness=0)
    self.pressStart = None
    self.bind('<Configure>', self.config)
    if master is None:
      self.pack(fill=tk.BOTH, expand=tk.YES)
    
    self.out = self.create_oval(0, 0, 0, 0)
    self.in1 = self.create_oval(0, 0, 0, 0)
    self.in2 = self.create_oval(0, 0, 0, 0)
    self.bind('<ButtonPress-1>', self.onPress)
    self.bind('<ButtonRelease-1>', self.onRelease)
    self.config()
    self.updateColours()
  
  def config(self, event=None):
    self.w = self.winfo_width()
    self.h = self.winfo_height()
    self.RADIUS_OUT = min(self.w, self.h)*self.RADIUS_OUT_F
    self.RADIUS_IN1 = min(self.w, self.h)*self.RADIUS_IN1_F
    self.RADIUS_IN2 = min(self.w, self.h)*self.RADIUS_IN2_F
    self.cx = self.w/2
    self.cy = self.h/2
    
    self.coords(self.out, self.cx-self.RADIUS_OUT/2, self.cy-self.RADIUS_OUT/2, self.cx+self.RADIUS_OUT/2, self.cy+self.RADIUS_OUT/2)
    self.coords(self.in1, self.cx-self.RADIUS_IN1/2, self.cy-self.RADIUS_IN1/2, self.cx+self.RADIUS_IN1/2, self.cy+self.RADIUS_IN1/2)
    self.coords(self.in2, self.cx-self.RADIUS_IN2/2, self.cy-self.RADIUS_IN2/2, self.cx+self.RADIUS_IN2/2, self.cy+self.RADIUS_IN2/2)
  
  def updateColours(self):
    self['bg'] = self.COLOR_BG
    self.itemconfigure(self.out, fill=self.COLOR_OUT)
    self.itemconfigure(self.in1, fill=self.COLOR_BTN1)
    self.itemconfigure(self.in2, fill=self.COLOR_BTN2)
  
  def onPress(self, event):
    self.COLOR_BTN1 = self.COLOR_IN1_A
    self.COLOR_BTN2 = self.COLOR_IN2_A
    self.updateColours()
    if self.game.mode == self.game.GAME:
      self.pressStart = None
      self.game.setMode(self.game.PAUSE)
    elif self.game.mode == self.game.PAUSE:
      self.pressStart = time()
  
  def onRelease(self, event):
    self.COLOR_BTN1 = self.COLOR_IN1_P
    self.COLOR_BTN2 = self.COLOR_IN2_P
    self.updateColours()
    if self.game.mode == self.game.MENU or self.game.mode == self.game.LOST:
      self.game.setMode(self.game.GAME)
    elif self.game.mode == self.game.PAUSE and self.pressStart is not None:
      if time() - self.pressStart < 3:
        self.game.setMode(self.game.GAME)
      else:
        self.game.setMode(self.game.MENU)
      self.pressStart = None
 
