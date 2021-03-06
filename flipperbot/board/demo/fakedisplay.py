import tkinter as tk

from ..debug.debug import Debug

class FakeDisplay (tk.Canvas):
  COLOR_P   = 'gray70'
  COLOR_A   = 'green1'
  COLOR_BG  = 'white'
  COLOR_BG1 = 'gray60'
  
  H_F  = 0.9
  W_F  = 0.95
  LW_F = 5.0
  DW_F = 0.8
  
  def __init__(self, game, joined=False, master=None):
    self.game = game
    self.debug = Debug(
      log=game.display.debug.log,
      logging=game.display.debug.logging,
      stdout=game.display.debug.stdout,
      parent=self,
      name="FakeDisplay"
    )
    self.debug("FakeDisplay used")
    if master is None:
      self.root = tk.Tk()
    else:
      self.root = master
    tk.Canvas.__init__(self, master=self.root, bd=0, highlightthickness=0)
    if master is None:
      self.pack(fill=tk.BOTH, expand=tk.YES)
    self.segments = {i : None for i in range(4)}
    self.joined = joined
    
    self.box = self.create_rectangle(0, 0, 0, 0)
    
    self.poly = [
      {
        seg : self.create_polygon(
          0, 0,
          0, 0,
          0, 0,
          0, 0,
          0, 0,
          0, 0
        ) for seg in "abcdefg"
      } for i in range(4)
    ]
    for i in range(4):
      self.poly[i]['dp'] = self.create_oval(0, 0, 0, 0)
    
    self.text = None
    self.dots = None
    self.tIndex = None
    
    self.bind("<Configure>", self.config)
    self.config()
    self.updateColours()
    self.loop()
  
  def drawSegHor(self, seg, x=0, y=0):
    if self.joined:
      self.coords(
        seg,
        x-self.SEG_LEN/2-self.SEG_WID/2, y,
        x-self.SEG_LEN/2, y-self.SEG_WID/2,
        x+self.SEG_LEN/2, y-self.SEG_WID/2,
        x+self.SEG_LEN/2+self.SEG_WID/2, y,
        x+self.SEG_LEN/2, y+self.SEG_WID/2,
        x-self.SEG_LEN/2, y+self.SEG_WID/2
      )
    else:
      self.coords(
        seg,
        x-self.SEG_LEN/2, y,
        x-self.SEG_LEN/2+self.SEG_WID/2, y-self.SEG_WID/2,
        x+self.SEG_LEN/2-self.SEG_WID/2, y-self.SEG_WID/2,
        x+self.SEG_LEN/2, y,
        x+self.SEG_LEN/2-self.SEG_WID/2, y+self.SEG_WID/2,
        x-self.SEG_LEN/2+self.SEG_WID/2, y+self.SEG_WID/2
      )
  
  def drawSegVer(self, seg, x=0, y=0):
    if self.joined:
      self.coords(
        seg,
        x, y-self.SEG_LEN/2-self.SEG_WID/2,
        x-self.SEG_WID/2, y-self.SEG_LEN/2,
        x-self.SEG_WID/2, y+self.SEG_LEN/2,
        x, y+self.SEG_LEN/2+self.SEG_WID/2,
        x+self.SEG_WID/2, y+self.SEG_LEN/2,
        x+self.SEG_WID/2, y-self.SEG_LEN/2
      )
    else:
      self.coords(
        seg,
        x, y-self.SEG_LEN/2,
        x-self.SEG_WID/2, y-self.SEG_LEN/2+self.SEG_WID/2,
        x-self.SEG_WID/2, y+self.SEG_LEN/2-self.SEG_WID/2,
        x, y+self.SEG_LEN/2,
        x+self.SEG_WID/2, y+self.SEG_LEN/2-self.SEG_WID/2,
        x+self.SEG_WID/2, y-self.SEG_LEN/2+self.SEG_WID/2
      )
  
  def drawDot(self, seg, x=0, y=0):
    self.coords(seg, x-self.DP_SIZE/2, y-self.DP_SIZE/2, x+self.DP_SIZE/2, y+self.DP_SIZE/2)
  
  def drawDigit(self, digit, x=0, y=0):
    x = x - self.DP_SIZE/2
    self.drawSegHor(digit['a'], x, y-(self.SEG_LEN+self.SEG_WID))
    self.drawSegVer(digit['b'], x+(self.SEG_LEN+self.SEG_WID)/2, y-(self.SEG_LEN+self.SEG_WID)/2)
    self.drawSegVer(digit['c'], x+(self.SEG_LEN+self.SEG_WID)/2, y+(self.SEG_LEN+self.SEG_WID)/2)
    self.drawSegHor(digit['d'], x, y+(self.SEG_LEN+self.SEG_WID))
    self.drawSegVer(digit['e'], x-(self.SEG_LEN+self.SEG_WID)/2, y+(self.SEG_LEN+self.SEG_WID)/2)
    self.drawSegVer(digit['f'], x-(self.SEG_LEN+self.SEG_WID)/2, y-(self.SEG_LEN+self.SEG_WID)/2)
    self.drawSegHor(digit['g'], x, y)
    self.drawDot(digit['dp'], x+self.SEG_LEN/2+self.SEG_WID+self.DP_SIZE/2, y+(self.SEG_LEN+self.SEG_WID))
  
  def loop(self):
    oldt = self.text
    oldd = self.dots
    oldi = self.tIndex
    if self.game.display._shown:
      self.text, self.dots = self.game.display._get_text()
    else:
      self.text = " "*4
      self.dots = [False]*4
    self.tIndex = self.game.display.tIndex
    if self.tIndex != oldi or self.dots != oldd or self.text != oldt:
      self.redraw()
    self.root.after(100, self.loop)
  
  def redraw(self):
    if len(self.text) < 4:
      self.text = self.text + " "*(len(self.text)-4)
    self.segments = list(map(lambda x: {s : s in self.game.display.valuemap[x] for s in "abcdefg"}, self.text))
    for i in range(len(self.segments)):
      self.segments[i]['dp'] = self.dots[i]
    for digit in range(4):
      index = (digit+self.tIndex)%len(self.text)
      for segment in list("abcdefg")+["dp"]:
        color = self.COLOR_A if self.segments[index][segment] else self.COLOR_P
        self.itemconfigure(self.poly[digit][segment], fill=color, outline=color)
  
  def config(self, event=None):
    self.w = self.winfo_width()
    self.h = self.winfo_height()
    
    self.W = self.W_F*self.w
    self.H = self.H_F*self.h
    self.HW_F = 3+2*self.LW_F
    self.WW_F = 8+4*self.LW_F+4*self.DW_F
    self.SEG_WID = min(self.H/self.HW_F, self.W/self.WW_F)
    self.SEG_LEN = self.SEG_WID*self.LW_F
    self.DP_SIZE = self.SEG_WID*self.DW_F
    self.W       = self.SEG_WID*self.WW_F
    self.H       = self.SEG_WID*self.HW_F
    
    self.coords(self.box, 0, 0, self.w, self.h)
    
    for i in range(4):
      self.drawDigit(self.poly[i], (self.w-self.W)/2+self.W*(2*i+1)/8, self.h/2)
  
  def updateColours(self):
    self['bg'] = self.COLOR_BG
    self.itemconfig(self.box, fill=self.COLOR_BG1)
    if self.text is not None:
      self.redraw()
