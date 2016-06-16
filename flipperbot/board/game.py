from .shared import gpio, ThreadEx, sleep, time, SharedVariable
from .totems import totems
from .display import Display
from .everythingButton import EverythingButton
from .audio import Audio, SoundEffect
from .robot import Robot
from .controller import Controller
from .led import LED

from random import random

from math import ceil

class Game (ThreadEx):
  deltaT  = SharedVariable(13)   # Time before totem changes
  minT    = SharedVariable(0.5)  # Minimum time between hits
  blinkT  = SharedVariable(5)    # Missing time when totem starts blinking
  blinkF  = SharedVariable(4)    # Totem blinking frequency
  cycleT  = SharedVariable(0.01) # Time between cycles
  updateF = SharedVariable(80)   # Update frequency of display
  scrollF = SharedVariable(2)    # Scroll frequency of display

  blinkR  = SharedVariable(5)   # Blinking frequency on robot connected
  blinkC  = SharedVariable(10)  # Blinking frequency on controller connected
  
  LED1 = LED(26)
  LED2 = LED(29)
  
  # MODES
  WAIT  = -2
  IDLE  = -1
  MENU  = 0
  GAME  = 1
  PAUSE = 2
  LOST  = 3
  
  totemList = SharedVariable()
  mode = SharedVariable(MENU)
  robots = SharedVariable()
  controllers = SharedVariable()
  
  def __init__(self, totemList):
    self.mode = self.IDLE
    self.totemList = list(totemList)
    self.robots = [Robot(), Robot()]
    self.controllers = [Controller(), Controller()]
    self.display = Display(self.updateF, self.scrollF)
    self.everythingButton = EverythingButton(self)
    self.audio = Audio()
    self.menuThread  = MenuThread(self)
    self.gameThread  = GameThread(self)
    self.pauseThread = PauseThread(self)
    super(Game, self).__init__(name="main")
    for t in totemList:
      self.addChild(t)
    self.addChild(self.display)
    self.addChild(self.everythingButton)
    self.addChild(self.audio)    
  
  def setMode(self, mode):
    if mode == self.IDLE:
      print("IDLE")
      self.display.show("OoOoOo")
      self.audio.stop()
    elif mode == self.MENU:
      print("MENU")
      self.gameThread.stop()
      self.pauseThread.stop()
      self.menuThread = MenuThread(self)
      self.menuThread.start()
    elif mode == self.GAME:
      print("GAME")
      self.menuThread.stop()
      self.pauseThread.stop()
      if self.gameThread is None or self.gameThread.stopped():
        self.gameThread = GameThread(self)
        self.gameThread.start()
      elif not self.gameThread.started():
        self.gameThread.start()
      elif self.gameThread.paused():
        self.gameThread.resume()
    elif mode == self.PAUSE:
      print("PAUSE")
      self.gameThread.pause()
      self.menuThread.stop()
      self.pauseThread = PauseThread(self)
      self.pauseThread.start()
    elif mode == self.LOST:
      print("LOST")
      self.gameThread.stop()
      self.pauseThread.stop()
      self.menuThread = MenuThread(self, lost=True)
      self.menuThread.start()
    self.mode = mode
  
  def setup(self):
    self.display.start()
    for t in self.totemList:
      t.start()
    self.everythingButton.start()
    self.setMode(self.MENU)
  
  def loop(self):
    if self.robots[0].active:
      if self.controllers[0].active:
        self.LED1.on()
      else:
        self.LED1.blink(self.blinkR)
    else:
      if self.controllers[0].active:
        self.LED1.blink(self.blinkC)
      else:
        self.LED1.off()
    
    if self.robots[1].active:
      if self.controllers[1].active:
        self.LED2.on()
      else:
        self.LED2.blink(self.blinkR)
    else:
      if self.controllers[1].active:
        self.LED2.blink(self.blinkC)
      else:
        self.LED2.off()
  
  def _nextTotem(self):
    self.totIndex = int(random()*len(self.totemList))
    self.totem = self.totemList[self.totIndex%len(self.totemList)]
    #self.totIndex = self.totIndex+1
  
  def cleanup(self):
    self.menuThread.stop()
    self.gameThread.stop()
    self.pauseThread.stop()
    gpio.cleanup()
  
  def isHit2(self):
    if self.controllers[0].active:
      if self.totem.pos == 'DL' and self.controllers[0].direction == Controller.Direction.BACKWARD_LEFT:
        return True
      elif self.totem.pos == 'DR' and self.controllers[0].direction == Controller.Direction.BACKWARD_RIGHT:
        return True
      elif self.totem.pos == 'UR' and self.controllers[0].direction == Controller.Direction.FORWARD_RIGHT:
        return True
      elif self.totem.pos == 'UL' and self.controllers[0].direction == Controller.Direction.FORWARD_LEFT:
        return True
      elif self.totem.pos == 'C' and self.controllers[0].direction == Controller.Direction.STOP:
        return True
      else:
        return False

class MenuThread (ThreadEx):
  def __init__(self, game, lost=False):
    self.game = game
    self.lost = lost
    super(self.__class__, self).__init__(name="menu")
  
  def setup(self):
    print("Entering menu")
    if self.lost:
      self.game.display.show("LOSE")
      self.game.audio.start(self.game.audio.LOST)
    else:
      self.game.display.show(["F", "li"]+list("pperbot    "))
      self.game.audio.start(self.game.audio.MENU)        
  
  def cleanup(self):
    self.game.audio.stop()

class GameThread (ThreadEx):
  def __init__(self, game):
    self.game = game
    super(self.__class__, self).__init__(name="game")
  
  def setup(self):
    print("Game started")
    self.game.display.show("_-^-"*2)
    sound = SoundEffect(self.game.audio.START)
    self.game.audio.stop()
    sound.start()
    sound.wait()
    
    self.game.audio.start(self.game.audio.GAME)
    self.game.totIndex = 0
    self.game._nextTotem()
    self.game.totem.on()
    self.points = 0
    self.lastHit = time()
    self.lastLoop = self.lastHit
    self.missing = self.game.deltaT
  
  def loop(self):
    # Correcting for pause
    self.lastHit = self.lastHit + time() - self.lastLoop
    
    # Slow down cycle
    sleep(self.game.cycleT)
    
    # Update display
    self.missing = self.game.deltaT + self.lastHit - time()
    text = "{:02d}{:02d}".format(self.points%100, int(ceil(self.missing))%100)
    self.game.display.show(text, [False, True, False, False])
    
    # Almost out of time!
    if not self.game.totem._blink and self.missing < self.game.blinkT:
      self.game.totem.blink(self.game.blinkF)
    
    # Check for hit
    if self.game.totem.isHit() or self.game.isHit2():
      self.points = self.points + 1
      self.game.totem.off()
      self.game._nextTotem()
      self.game.totem.on()
      self.lastHit = time()
      self.lastLoop = self.lastHit
      return
    
    # Lose
    if self.missing < 0:
      self.game.setMode(self.game.LOST)
      return
    
    self.lastLoop = time()
  
  def cleanup(self):
    if not self.game.totem.stopped():
      self.game.totem.off()
    self.game.audio.stop()

class PauseThread (ThreadEx):
  def __init__(self, game):
    self.game = game
    super(self.__class__, self).__init__(name="pause")
  
  def setup(self):
    print("Entering pause")
    self.game.audio.start(self.game.audio.PAUSE)
  
  def cleanup(self):
    self.game.audio.stop()
