from .shared import text, textLock, gpio, Thread, sleep, time
from .totems import totems
from .display import Display
from .everythingButton import EverythingButton
from .audio import Audio

from math import ceil

# GAME PARAMETERS
anim_deltat = 0.1
anim_duration = 2

class Game (Thread):
  deltaT  = 13   # Time before totem changes
  minT    = 0.5  # Minimum time between hits
  blinkT  = 5    # Missing time when totem starts blinking
  blinkF  = 4    # Totem blinking frequency
  cycleT  = 0.01 # Time between cycles
  updateF = 60   # Update frequency of display
  scrollF = 2    # Scroll frequency of display
  
  # MODES
  MENU  = 0
  GAME  = 1
  PAUSE = 2
  
  def __init__(self, totemList):
    self._stopped = False
    self._actually_stopped = False
    self.totemList = totemList
    self.mode = self.MENU
    self.display = Display(self.updateF, self.scrollF)
    self.everythingButton = EverythingButton(self)
    self.audio = Audio()
    
    super(self.__class__, self).__init__()
  
  def setMode(self, mode):
    if mode == self.MENU:
      print("MENU")
      self.display.show(["F", "li"]+list("pperbot    "))
      self.audio.start(self.audio.MENU)
    elif mode == self.GAME:
      print("GAME")
      self.audio.start(self.audio.GAME)
    elif mode == self.PAUSE:
      print("PAUSE")
      self.audio.start(self.audio.PAUSE)
    self.mode = mode
  
  def start(self):
    self.display.start()
    for t in self.totemList:
      t.start()
    self.everythingButton.start()
    self.setMode(self.MENU)
    super(Game, self).start()
  
  def stop(self):
    self._stopped = True
  
  def stopped(self):
    return self._actually_stopped
  
  def _nextTotem(self):
    self.totem = self.totemList[self.totIndex%len(self.totemList)]
    self.totIndex = self.totIndex+1
  
  def run(self):    
    while not self._stopped:
      self.totIndex = 0
      self._nextTotem()
      self.totem.on()
         
      while not self._stopped and self.mode != self.GAME:
        self.totem.off()
        self._nextTotem()
        self.totem.on()
        sleep(1)
      
      if self._stopped:
        break
      
      s = time()
      self.audio.start(self.audio.START)
      while time() - s < 4:
        self.display.show("._-^-_")
      
      self.audio.start(self.audio.GAME)
      
      points = 0
      last = time()
      lasthit = last
      
      self.totIndex = 0
      self._nextTotem()
      self.totem.on()
      
      while not self._stopped and self.mode == self.GAME:
        sleep(self.cycleT)
        now = time()
        missing = self.deltaT+last-now
        text = "{:02d}{:02d}".format(points%100, int(ceil(missing))%100)
        self.display.show(text)
        while self.mode == self.PAUSE and not self._stopped:
          pass
        now = time()
        last = now+missing-self.deltaT
        if self._stopped or self.mode != self.GAME:
          break
        if not self.totem._blink and missing < self.blinkT:
          self.totem.blink(self.blinkF)
        if self.totem.isHit():
          _lasthit = lasthit
          lasthit = now
          if lasthit-_lasthit >= self.minT:
            points = points + 1
            self.totem.off()
            self._nextTotem()
            self.totem.on()
            last = now
            continue
        if missing < 0:
          # Lose
          self.setMode(self.MENU)
          self.display.show("LOSE")
          break
      self.totem.off()
    print("gameThread stopped")
    self.display.stop()
    for t in self.totemList:
      t.stop()
    self.everythingButton.stop()
    self.audio.stop()
    while not (self.display.stopped() and self.everythingButton.stopped()):
      pass
    for t in self.totemList:
      while not t.stopped():
        pass
    gpio.cleanup()
    self._actually_stopped = True

