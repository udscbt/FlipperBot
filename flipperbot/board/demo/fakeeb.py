import tkinter as tk
from time import time

class FakeEB:
  RADIUS_OUT_F = 5/6
  RADIUS_IN_F  = 4/6
  
  COLOR_OUT  = 'gray20'
  COLOR_IN_P = 'red'
  COLOR_IN_A = 'green'
  COLOR_BTN  = COLOR_IN_P
  COLOR_BG   = 'white'
  
  def __init__(self, game):
    self.game = game
    self.root = tk.Tk()
    self.canvas = tk.Canvas(self.root)
    self.canvas['bg'] = self.COLOR_BG
    self.canvas.pack(fill=tk.BOTH, expand=tk.YES)
    self.pressStart = None
  
  def loop(self):
    self.w = self.canvas.winfo_width()
    self.h = self.canvas.winfo_height()
    self.RADIUS_OUT = min(self.w, self.h)*self.RADIUS_OUT_F
    self.RADIUS_IN = min(self.w, self.h)*self.RADIUS_IN_F
    self.canvas.delete(tk.ALL)
    cx = self.w/2
    cy = self.h/2
    self.canvas.create_oval(cx-self.RADIUS_OUT/2, cy-self.RADIUS_OUT/2, cx+self.RADIUS_OUT/2, cy+self.RADIUS_OUT/2, fill=self.COLOR_OUT)
    self.btn = self.canvas.create_oval(cx-self.RADIUS_IN/2, cy-self.RADIUS_IN/2, cx+self.RADIUS_IN/2, cy+self.RADIUS_IN/2, fill=self.COLOR_BTN)
    self.canvas.bind('<ButtonPress-1>', self.onPress)
    self.canvas.bind('<ButtonRelease-1>', self.onRelease)
    self.root.after(50, self.loop)
  
  def onPress(self, event):
    self.COLOR_BTN = self.COLOR_IN_A
#    print("Pressed")
    if self.game.mode == self.game.GAME:
      self.pressStart = None
      self.game.setMode(self.game.PAUSE)
    elif self.game.mode == self.game.PAUSE:
      self.pressStart = time()
  
  def onRelease(self, event):
    self.COLOR_BTN = self.COLOR_IN_P
#    print("Released")
    if self.game.mode == self.game.MENU:
      self.game.setMode(self.game.GAME)
    elif self.game.mode == self.game.PAUSE and self.pressStart is not None:
      if time() - self.pressStart < 3:
        self.game.setMode(self.game.GAME)
      else:
        self.game.setMode(self.game.MENU)
      self.pressStart = None
 
