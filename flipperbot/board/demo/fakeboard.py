import tkinter as tk
from ..game import Game
from ..totems import totems

from ..debug.debug import Debug

class FakeBoard (tk.Canvas):
  RADIUS_IN_F  = 1.0/10
  RADIUS_OUT_F = 1.0/8
  BORDER_F     = 1/30
  
  COLOR_ACTIVE  = 'red'
  COLOR_PASSIVE = 'gray70'
  COLOR_OUT     = 'gray30'
  COLOR_HIT     = 'green'
  COLOR_BG1     = 'black'
  COLOR_BORDER  = 'gray5'
  COLOR_BG      = 'white'
  
  def __init__(self, game=None, master=None, input=False):
    if game is None:
      game = Game(totems.values())
    self.game = game
    self.debug = Debug(
      log=game.debug.log,
      logging=game.debug.logging,
      stdout=game.debug.stdout,
      parent=self,
      name="FakeBoard>".format(self.index)
    )
    self.debug("FakeBoard used")
    if master is None:
      self.root = tk.Tk()
    else:
      self.root = master
    tk.Canvas.__init__(self, master=self.root, bd=0, highlightthickness=0)
    if master is None:
      self.pack(fill=tk.BOTH, expand=tk.YES)
    
    self.box = self.create_rectangle(0, 0, 0, 0)
    self.totems = {
      tot.pos : {
        'out'   : self.create_oval(0, 0, 0, 0, outline='black'),
        'in'    : self.create_oval(0, 0, 0, 0, outline='black'),
        'totem' : tot
      } for tot in self.game.totemList
    }
    
    self.input(input)
    
    self.bind("<Configure>", self.config)
    self.config()
    self.updateColours()
    self.loop()
    
  def input(self, value=None):
    if value is None:
      return self._input
    else:
      self._input = value
      if self._input:
        self.debug("Input enabled")
        for t in self.totems.values():
          def handler(t):
            return lambda e: t['totem'].on()
          self.tag_bind(t['in'], '<Button-1>', handler(t))
          def handler(t):
            return lambda e: t['totem'].off()
          self.tag_bind(t['in'], '<ButtonRelease-1>', handler(t))
          
          def handler(t):
            def hitTotem(e):
              t['totem']._hit = True
            return hitTotem
          self.tag_bind(t['in'], '<Button-3>', handler(t))
          def handler(t):
            def hitTotem(e):
              t['totem']._hit = False
            return hitTotem
          self.tag_bind(t['in'], '<ButtonRelease-3>', handler(t))
      else:
        self.debug("Input disabled")
        for t in self.totems.values():
          self.tag_unbind(t['in'], '<Button-1>')
          self.tag_unbind(t['in'], '<ButtonRelease-1>')
          self.tag_unbind(t['in'], '<Button-3>')
          self.tag_unbind(t['in'], '<ButtonRelease-3>')
  
  def config(self, event=None):    
    self.w = self.winfo_width()
    self.h = self.winfo_height()
    self.dim = min(self.w, self.h)
    self.RADIUS_IN  = self.dim*self.RADIUS_IN_F
    self.RADIUS_OUT = self.dim*self.RADIUS_OUT_F
    self.BORDER     = self.dim*self.BORDER_F
    self.inner = self.dim-self.BORDER
    
    self.coords(self.box, self.w/2-self.inner/2, self.h/2-self.inner/2, self.w/2+self.inner/2, self.h/2+self.inner/2)
    self.itemconfigure(self.box, width=self.BORDER)
    
    pos = {
      'DL' : (-1, 1),
      'DR' : (1, 1),
      'UL' : (-1, -1),
      'UR' : (1, -1),
      'C'  : (0, 0)
    }
    for p, t in self.totems.items():
      cx = self.w/2+self.inner/4*pos[p][0]
      cy = self.h/2+self.inner/4*pos[p][1]
      self.coords(t['out'], cx - self.RADIUS_OUT/2, cy - self.RADIUS_OUT/2, cx + self.RADIUS_OUT/2, cy + self.RADIUS_OUT/2)
      self.coords(t['in'], cx - self.RADIUS_IN/2, cy - self.RADIUS_IN/2, cx + self.RADIUS_IN/2, cy + self.RADIUS_IN/2)
  
  def updateColours(self):
    self['bg'] = self.COLOR_BG
    self.itemconfigure(self.box, fill=self.COLOR_BG1, outline=self.COLOR_BORDER)
    for t in self.totems.values():
      self.itemconfigure(t['out'], fill=self.COLOR_OUT)
      self.updateTotem(t)
  
  def updateTotem(self, t):
    self.itemconfigure(t['in'], fill=(self.COLOR_ACTIVE if t['totem']._on else self.COLOR_PASSIVE))
    self.itemconfigure(t['out'], fill=(self.COLOR_HIT if t['totem']._hit else self.COLOR_OUT))
  
  def loop(self):
    for t in self.totems.values():
      self.updateTotem(t)
    self.root.after(50, self.loop)
