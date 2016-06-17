import tkinter as tk

class FakeDisplay:
  COLOR_P   = 'gray70'
  COLOR_A   = 'green1'
  COLOR_BG  = 'white'
  COLOR_BG1 = 'gray60'
  
  H_F = 9.0/10
  W_F = 9.0/10
  LW_F = 5.0
  DW_F = 0.8
  
  def __init__(self, game, joined=False):
    self.game = game
    self.root = tk.Tk()
    self.canvas = tk.Canvas(self.root)
    self.canvas['bg'] = self.COLOR_BG1
    self.canvas.pack(fill=tk.BOTH, expand=tk.YES)
    self.segments = {}
    for i in range(4):
      self.segments[i] = None
    self.joined = joined
  
  def drawSegHor(self, x=0, y=0, active=False):
    color = self.COLOR_A if active else self.COLOR_P
    if self.joined:
      return self.canvas.create_polygon(
        x-self.SEG_LEN/2-self.SEG_WID/2, y,
        x-self.SEG_LEN/2, y-self.SEG_WID/2,
        x+self.SEG_LEN/2, y-self.SEG_WID/2,
        x+self.SEG_LEN/2+self.SEG_WID/2, y,
        x+self.SEG_LEN/2, y+self.SEG_WID/2,
        x-self.SEG_LEN/2, y+self.SEG_WID/2,
        fill=color, outline=color
      )
    else:
      return self.canvas.create_polygon(
        x-self.SEG_LEN/2, y,
        x-self.SEG_LEN/2+self.SEG_WID/2, y-self.SEG_WID/2,
        x+self.SEG_LEN/2-self.SEG_WID/2, y-self.SEG_WID/2,
        x+self.SEG_LEN/2, y,
        x+self.SEG_LEN/2-self.SEG_WID/2, y+self.SEG_WID/2,
        x-self.SEG_LEN/2+self.SEG_WID/2, y+self.SEG_WID/2,
        fill=color, outline=color
      )
  
  def drawSegVer(self, x=0, y=0, active=False):
    color = self.COLOR_A if active else self.COLOR_P
    if self.joined:
      return self.canvas.create_polygon(
        x, y-self.SEG_LEN/2-self.SEG_WID/2,
        x-self.SEG_WID/2, y-self.SEG_LEN/2,
        x-self.SEG_WID/2, y+self.SEG_LEN/2,
        x, y+self.SEG_LEN/2+self.SEG_WID/2,
        x+self.SEG_WID/2, y+self.SEG_LEN/2,
        x+self.SEG_WID/2, y-self.SEG_LEN/2,
        fill=color, outline=color
      )
    else:
      return self.canvas.create_polygon(
        x, y-self.SEG_LEN/2,
        x-self.SEG_WID/2, y-self.SEG_LEN/2+self.SEG_WID/2,
        x-self.SEG_WID/2, y+self.SEG_LEN/2-self.SEG_WID/2,
        x, y+self.SEG_LEN/2,
        x+self.SEG_WID/2, y+self.SEG_LEN/2-self.SEG_WID/2,
        x+self.SEG_WID/2, y-self.SEG_LEN/2+self.SEG_WID/2,
        fill=color, outline=color
      )
  
  def drawDot(self, x=0, y=0, active=False):
    color = self.COLOR_A if active else self.COLOR_P
    return self.canvas.create_oval(x-self.DP_SIZE/2, y-self.DP_SIZE/2, x+self.DP_SIZE/2, y+self.DP_SIZE/2, fill=color, outline=color)
  
  def drawDigit(self, x=0, y=0, segments=None, active=False):
    ret = {}
    ret['a'] = self.drawSegHor(x, y-(self.SEG_LEN+self.SEG_WID), active and segments is not None and segments['a'])
    ret['b'] = self.drawSegVer(x+(self.SEG_LEN+self.SEG_WID)/2, y-(self.SEG_LEN+self.SEG_WID)/2, active and segments is not None and segments['b'])
    ret['c'] = self.drawSegVer(x+(self.SEG_LEN+self.SEG_WID)/2, y+(self.SEG_LEN+self.SEG_WID)/2, active and segments is not None and segments['c'])
    ret['d'] = self.drawSegHor(x, y+(self.SEG_LEN+self.SEG_WID), active and segments is not None and segments['d'])
    ret['e'] = self.drawSegVer(x-(self.SEG_LEN+self.SEG_WID)/2, y+(self.SEG_LEN+self.SEG_WID)/2, active and segments is not None and segments['e'])
    ret['f'] = self.drawSegVer(x-(self.SEG_LEN+self.SEG_WID)/2, y-(self.SEG_LEN+self.SEG_WID)/2, active and segments is not None and segments['f'])
    ret['g'] = self.drawSegHor(x, y, active and segments is not None and segments['g'])
    ret['dp'] = self.drawDot(x+self.SEG_LEN/2+self.SEG_WID+self.DP_SIZE/2, y+(self.SEG_LEN+self.SEG_WID), active and segments is not None and segments['dp'])
    return ret
  
  def loop(self):
    self.canvas.delete(tk.ALL)
    
    self.w = self.canvas.winfo_width()
    self.h = self.canvas.winfo_height()
    
    self.SEG_WID = min(self.H_F*self.h/(3+2*self.LW_F), self.W_F*self.w/(8+4*self.LW_F))
    self.SEG_LEN = self.SEG_WID*self.LW_F
    self.DP_SIZE = self.SEG_WID*self.DW_F
    
    self.text, self.dots = self.game.display._get_text()
    self.segments = list(map(lambda x: {s : s in self.game.display.valuemap[x] for s in "abcdefg"}, self.text))
    for i in range(len(self.segments)):
      self.segments[i]['dp'] = self.dots[i]
    
    for i in range(4):
      self.drawDigit(self.w*(2*i+1)/8, self.h/2, self.segments[(i+self.game.display.tIndex)%len(self.text)], True)
    
    self.root.after(100, self.loop)
