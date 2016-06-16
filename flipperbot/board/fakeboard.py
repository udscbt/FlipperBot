import tkinter as tk
from .totems import totems

class FakeBoard:
  RADIUS_IN_F  = 1.0/10
  RADIUS_OUT_F = 1.0/8
  
  ACTIVE_COLOR  = 'red'
  PASSIVE_COLOR = 'gray'
  BG1_COLOR     = 'gray15'
  BG_COLOR      = 'white'
  
  def __init__(self, game):
    self.game = game
    self.root = tk.Tk()
    self.canvas = tk.Canvas(self.root)
    self.canvas['bg'] = self.BG_COLOR
    self.canvas.pack(fill=tk.BOTH, expand=tk.YES)
  
  def loop(self):
    self.canvas.delete(tk.ALL)
    
    self.w = self.canvas.winfo_width()
    self.h = self.canvas.winfo_height()
    self.RADIUS_IN  = min(self.w, self.h)*self.RADIUS_IN_F
    self.RADIUS_OUT = min(self.w, self.h)*self.RADIUS_OUT_F
    
    pos = {
      'DL' : (-1, 1),
      'DR' : (1, 1),
      'UL' : (-1, -1),
      'UR' : (1, -1),
      'C'  : (0, 0)
    }
    for t in self.game.totemList:
      cx = self.w/2+self.w/3*pos[t.pos][0]
      cy = self.h/2+self.h/3*pos[t.pos][1]
      self.canvas.create_oval(cx - self.RADIUS_OUT/2, cy - self.RADIUS_OUT/2, cx + self.RADIUS_OUT/2, cy + self.RADIUS_OUT/2, fill=self.BG1_COLOR, outline='black')
      self.canvas.create_oval(cx - self.RADIUS_IN/2, cy - self.RADIUS_IN/2, cx + self.RADIUS_IN/2, cy + self.RADIUS_IN/2, fill=(self.ACTIVE_COLOR if t._on else self.PASSIVE_COLOR), outline='black')
    self.root.after(50, self.loop)
