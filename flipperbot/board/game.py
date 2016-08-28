from .shared import gpio, ThreadEx, sleep, time, SharedVariable
from .totems import totems
from .display import Display, DisplayEx
from .everythingButton import EverythingButton
from .audio import Audio, SoundEffect
from ..robot import Robot
from ..controller import Controller
from .led import LED
from .debug.log import Log
from .debug.debug import Debug

from random import random

from math import ceil

class Game (ThreadEx):
  deltaTmax = SharedVariable(13)   # Time before totem changes
  deltaTmin = SharedVariable(7)   # Time before totem changes
  minT      = SharedVariable(0.5)  # Minimum time between hits
  blinkT    = SharedVariable(5)    # Missing time when totem starts blinking
  blinkF    = SharedVariable(4)    # Totem blinking frequency
  cycleT    = SharedVariable(0.01) # Time between cycles
  updateF   = SharedVariable(80)   # Update frequency of display
  scrollF   = SharedVariable(2)    # Scroll frequency of display

  blinkR    = SharedVariable(5)   # Blinking frequency on robot connected
  blinkC    = SharedVariable(10)  # Blinking frequency on controller connected

  hitDelay  = SharedVariable(0.1) # Maximum time between totem hit and robot hit to be considered simultaneous
  
  # MODES
  WAIT    = -2
  IDLE    = -1
  MENU    = 0
  GAME    = 1
  PAUSE   = 2
  LOST    = 3
  NOSTOP  = 4
  SUCCESS = 5
  
  def getModeName(self, mode):
    if mode == -2:
      return "Wait"
    elif mode == -1:
      return "Idle"
    elif mode == 0:
      return "Main menu"
    elif mode == 1:
      return "Game started"
    elif mode == 2:
      return "Game paused"
    elif mode == 3:
      return "Game lost -> Main Menu"
    elif mode == 4:
      return "No control"
  
  totemList = SharedVariable()
  mode = SharedVariable(MENU)
  robots = SharedVariable()
  controllers = SharedVariable()
  
  def __init__(self, totemList, displayex=True, logging=True, debug=False):
    self.log = Log(filename="main.log", enabled=logging)
    self.debug = Debug(
      log=self.log,
      logging=logging,
      stdout=debug,
      parent=self,
      name="Game"
    )
    self.mode = self.IDLE
    self.totemList = list(totemList)
    self.robots = [Robot(), Robot()]
    self.controllers = [Controller(), Controller()]
    if displayex:
      self.debug("Using external display")
      self.display = DisplayEx(self.updateF, self.scrollF, debug=self.debug)
    else:
      self.debug("Using internal display")
      self.display = Display(self.updateF, self.scrollF, debug=self.debug)
    self.everythingButton = EverythingButton(self)
    self.audio = Audio(debug=self.debug)
    self.menuThread  = MenuThread(self)
    self.gameThread  = GameThread(self)
    self.pauseThread = PauseThread(self)
    self.randomThread = RandomThread(self)
    self.LED1 = LED(26)
    self.LED2 = LED(29)
    super(Game, self).__init__(name="main")
    for t in totemList:
      self.addChild(t)
    self.addChild(self.display)
    self.addChild(self.everythingButton)
    self.addChild(self.audio)
    self.addChild(self.randomThread)
    self.addChild(self.LED1)
    self.addChild(self.LED2)
    self.debug("Initialized")
  
  def setMode(self, mode):
    self.debug("Changing mode")
    if mode == self.IDLE:
      self.debug("Mode selected: IDLE")
      self.display.show("OoOoOo")
      self.audio.stop()
    elif mode == self.MENU:
      self.debug("Mode selected: MENU")
      self.gameThread.stop()
      self.pauseThread.stop()
      self.menuThread = MenuThread(self)
      self.menuThread.start()
    elif mode == self.GAME:
      self.debug("Mode selected: GAME")
      self.menuThread.stop()
      self.pauseThread.stop()
      if self.gameThread is None or self.gameThread.stopped():
        self.gameThread = GameThread(self)
        self.gameThread.start()
      elif not self.gameThread.started():
        self.gameThread.start()
      elif self.gameThread.paused():
        self.gameThread.resume()
        sound = SoundEffect(self.audio.RSME, debug=self.debug)
        sound.start()
    elif mode == self.PAUSE:
      self.debug("Mode selected: PAUSE")
      self.gameThread.pause()
      self.menuThread.stop()
      self.pauseThread = PauseThread(self)
      self.pauseThread.start()
    elif mode == self.LOST:
      self.debug("Mode selected: LOST")
      self.gameThread.stop()
      self.pauseThread.stop()
      self.menuThread = MenuThread(self)
      self.menuThread.start()
    elif mode == self.SUCCESS:
      self.debug("Mode selected: SUCCESS")
      self.gameThread.stop()
      self.pauseThread.stop()
      self.menuThread = MenuThread(self)
      self.menuThread.start()
    self.mode = mode
  
  def setup(self):
    self.display.start()
    for t in self.totemList:
      t.start()
    self.everythingButton.start()
    self.LED1.start()
    self.LED2.start()
    self.randomThread.start()
    self.setMode(self.MENU)
    self.debug("Thread started")
  
  def loop(self):
    if self.robots[0].active:
      if self.controllers[0].active:
        self.LED1.on()
        if self.mode == self.GAME:
          self.robots[0].direction = self.controllers[0].direction
        else:
          self.robots[0].direction = Robot.Direction.STOP
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
        if self.mode == self.GAME:
          self.robots[1].direction = self.controllers[1].direction
        else:
          self.robots[1].direction = Robot.Direction.STOP
      else:
        self.LED2.blink(self.blinkR)
    else:
      if self.controllers[1].active:
        self.LED2.blink(self.blinkC)
      else:
        self.LED2.off()
  
  def _nextTotem(self):
    self.debug("Selecting new totem")
    old = self.totIndex
    while old == self.totIndex:
      self.totIndex = int(random()*len(self.totemList))
    self.totem = self.totemList[self.totIndex]
    self.totem.clearHit()
    self.debug("Totem {} selected".format(self.totem.pos))
  
  def cleanup(self):
    self.menuThread.stop()
    self.menuThread.wait()
    self.gameThread.stop()
    self.gameThread.wait()
    self.pauseThread.stop()
    self.pauseThread.wait()
    gpio.cleanup()
    self.debug("Thread stopped")
    self.debug("Closing log")
    self.log.closeall()
  
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
  def __init__(self, game):
    self.game = game
    self.debug = Debug(
      log=self.game.log,
      logging=self.game.debug.logging,
      stdout=self.game.debug.stdout,
      parent=self,
      name="MenuThread"
    )
    super(self.__class__, self).__init__(name="menu")
    self.debug("Initialized")
  
  def setup(self):
    self.debug("Entering menu")
    if self.game.mode == self.game.LOST:
      self.game.display.show("LOSE")
      self.game.audio.start(self.game.audio.LOST)
    elif self.game.mode == self.game.SUCCESS:
      self.game.display.show("SUCCESS")
      self.game.audio.start(self.game.audio.MENU)
    else:
      self.game.display.show(["F", "li"]+list("pperbot    "))
      self.game.audio.start(self.game.audio.MENU)
    self.game.totIndex = 0;
    self.game.totem = self.game.totemList[self.game.totIndex]
  
  def loop(self):
    self.game.totem.off()
    self.game._nextTotem()
    self.game.totem.on()
    sleep(1)
  
  def cleanup(self):
    self.game.totem.off()
    self.game.audio.stop()
    self.debug("Stopped")

class GameThread (ThreadEx):
  def __init__(self, game):
    self.game = game
    self.debug = Debug(
      log=self.game.log,
      logging=self.game.debug.logging,
      stdout=self.game.debug.stdout,
      parent=self,
      name="GameThread"
    )
    super(self.__class__, self).__init__(name="game")
    self.debug("Initialized")
  
  def deltaT(self):
    if self.points == 0:
      return self.game.deltaTmax
    else:
      tmp = self.game.deltaTmax-(int(self.points/15))*3
      if tmp < self.game.deltaTmin:
        self.debug("Winner!")
        sound = SoundEffect(self.game.audio.SUCCESS, debug=self.debug)
        sound.start()
        self.game.setMode(self.game.SUCCESS)
        return
      return tmp
  
  def setup(self):
    self.debug("Game started")
    self.debug("Showing animation")
    self.game.display.show("_-^-"*2)
    self.game.display.setScrollSpeed(10);
    sound = SoundEffect(self.game.audio.READY, debug=self.debug)
    self.game.audio.stop()
    sound.start()
    sound.wait()
    sound = SoundEffect(self.game.audio.START, debug=self.debug)
    sound.start()
    
    self.debug("Initializing variables")
    self.game.audio.start(self.game.audio.GAME)
    self.game.totIndex = 0
    self.game._nextTotem()
    self.game.totem.on()
    self.points = 0
    self.lastHit = time()
    self.lastLoop = self.lastHit
    self.missing = self.deltaT()
    self.game.display.setScrollSpeed(self.game.scrollF)
    self.debug("Game actually started")
  
  def loop(self):
    # Correcting for pause
    self.lastHit = self.lastHit + time() - self.lastLoop
    
    #~ # Slow down cycle
    #~ sleep(self.game.cycleT)
    
    # Update display
    self.missing = self.deltaT() + self.lastHit - time()
    text = "{:02d}{:02d}".format(self.points%100, int(ceil(self.missing))%100)
    self.game.display.show(text, [False, True, False, False])
    
    # Almost out of time!
    if not self.game.totem._blink and self.missing < self.game.blinkT:
      self.debug("Warning that time is almost up")
      sound = SoundEffect(self.game.audio.OOT, debug=self.debug)
      sound.start()
      self.game.totem.blink(self.game.blinkF)
    
    # Check for hit
    if self.game.totem.isHit():# or self.game.isHit2():
      self.debug("Selected totem was hit")
      hit = True
      #~ t1 = time()
      #~ t2 = self.game.robots[0].hit
      #~ if not t2 or abs(t2-t1) > self.game.hitDelay:
        #~ sleep(self.game.hitDelay)
        #~ t2 = self.game.robots[0].hit
        #~ if not t2 or abs(t2-t1) > self.game.hitDelay:
          #~ hit = False
          #~ self.debug("Fake hit")
      if hit:
        self.debug("Confirmed hit")
        sound = SoundEffect(self.game.audio.HIT, debug=self.debug)
        sound.start()
        self.debug("Increasing points count")
        self.points = self.points + 1
        self.game.totem.off()
        self.game._nextTotem()
        self.game.totem.on()
        self.lastHit = time()
        self.lastLoop = self.lastHit
        return
    
    # Lose
    if self.missing < 0:
      self.debug("Time is up")
      sound = SoundEffect(self.game.audio.LOSE, debug=self.debug)
      sound.start()
      self.game.setMode(self.game.LOST)
      return
    
    self.lastLoop = time()
  
  def cleanup(self):
    if not self.game.totem.stopped():
      self.debug("Turning off totems")
      self.game.totem.off()
    self.game.audio.stop()
    self.debug("Stopped")

class PauseThread (ThreadEx):
  def __init__(self, game):
    self.game = game
    self.debug = Debug(
      log=self.game.log,
      logging=self.game.debug.logging,
      stdout=self.game.debug.stdout,
      parent=self,
      name="PauseThread"
    )
    super(self.__class__, self).__init__(name="pause")
    self.debug("Initialized")
  
  def setup(self):
    self.debug("Entering pause")
    self.game.audio.start(self.game.audio.PAUSE)
  
  def cleanup(self):
    self.game.audio.stop()
    self.debug("Stopped")

class RandomThread (ThreadEx):
  def __init__(self, game):
    self.game = game
    self.debug = Debug(
      log=self.game.log,
      logging=self.game.debug.logging,
      stdout=self.game.debug.stdout,
      parent=self,
      name="RandomThread"
    )
    super(self.__class__, self).__init__(name="random")
    self.debug("Initialized")
  
  def setup(self):
    self.prob = 1
  
  def loop(self):
    if self.game.mode == self.game.PAUSE:
      if random() > 1.0/self.prob:
        self.debug("Playing random sound effect")
        sound = SoundEffect(self.game.audio.RAND, debug=self.debug)
        sound.start()
        sound.wait()
        self.debug("Reducing probability of random sound effect")
        self.prob = 1
      else:
        self.debug("Increasing probability of random sound effect")
        self.prob = 1.5*self.prob
    sleep(5)
    
#~ from .simonsays import GameThread
