import tkinter as tk

from ..debug.debug import Debug

class FakeLED (tk.Canvas):
  RADIUS_F  = 0.9
  
  COLOR_ACTIVE  = 'yellow'
  COLOR_PASSIVE = '#AA2'
  COLOR_BG      = 'gray70'
  
  def __init__(self, led, master=None):
    self.led = led
    self.debug = Debug(
      log=led.debug.log,
      logging=led.debug.logging,
      stdout=led.debug.stdout,
      parent=self,
      name="FakeLED<{}>".format(led.pin)
    )
    self.debug("FakeLED used")
    if master is None:
      self.root = tk.Tk()
    else:
      self.root = master
    tk.Canvas.__init__(self, master=self.root, bd=0, highlightthickness=0)
    if master is None:
      self.pack(fill=tk.BOTH, expand=tk.YES)
    self.oval = self.create_oval(0, 0, 0, 0, width=0)
    
    self.bind("<Configure>", self.config)
    self.config()
    self.updateColours()
    self.loop()

  def config(self, event=None):    
    self.w = self.winfo_width()
    self.h = self.winfo_height()
    self.dim = min(self.w, self.h)
    self.RADIUS  = self.dim*self.RADIUS_F
    
    self.coords(self.oval, self.w/2-self.RADIUS/2, self.h/2-self.RADIUS/2, self.w/2+self.RADIUS/2, self.h/2+self.RADIUS/2)

  def updateColours(self):
    self['bg'] = self.COLOR_BG
    self.itemconfigure(self.oval, fill=(self.COLOR_ACTIVE if self.led._on else self.COLOR_PASSIVE))
  
  def loop(self):
    self.updateColours()
    self.root.after(50, self.loop)
