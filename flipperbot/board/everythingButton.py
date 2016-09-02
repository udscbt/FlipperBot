from .shared import gpio, Thread, sleep, time

from .debug.debug import Debug

class EverythingButton:
  def __init__(self, game):
    self.game = game
    self.debug = Debug(
      log=game.debug.log,
      logging=game.debug.logging,
      stdout=game.debug.stdout,
      parent=self,
      name="EverythingButton"
    )
    self.everythingButton = 18
    gpio.setup(self.everythingButton, gpio.IN, pull_up_down=gpio.PUD_DOWN)
    self._lastPress = None
    self._started = False
    self._stopped = False
    super(EverythingButton, self).__init__()
    self.debug("Initialized")
  
  def start(self):
    if not self._started:
      gpio.add_event_detect(self.everythingButton, gpio.BOTH, callback=self._managePress, bouncetime=50)
      self._started = True
      self.debug("Started")
  
  def stop(self):
    gpio.remove_event_detect(self.everythingButton)
    self._stopped = True
    self.debug("Stopped")
  
  def started(self):
    return self._started
  
  def stopped(self):
    return self._stopped
  
  def wait(self):
    while not self.stopped():
      pass
  
  def _managePress(self, channel):
    if gpio.input(channel):
      self.debug("Button pressed")
      rising = True
      falling = False
    else:
      self.debug("Button released")
      rising = False
      falling = True
    if self.game.mode == self.game.IDLE:
      return
    elif self.game.mode == self.game.MENU or self.game.mode == self.game.LOST:
      if falling:
        self.game.setMode(self.game.GAME)
    elif self.game.mode == self.game.GAME:
      if rising:
        self.game.setMode(self.game.PAUSE)
    elif self.game.mode == self.game.PAUSE:
      if rising:
        self._lastPress = time()
      if falling and self._lastPress is not None:
        if time()-self._lastPress < 3:
          self.game.setMode(self.game.GAME)
        else:
          self.game.setMode(self.game.MENU)
        self._lastPress = None

class RemoteEverythingButton:
  def __init__(self, game):
    self.game = game
    self.debug = Debug(
      log=game.debug.log,
      logging=game.debug.logging,
      stdout=game.debug.stdout,
      parent=self,
      name="RemoteEverythingButton"
    )
    self._lastPress = None
    self.pressed = False
  
  def press(self):
    self.debug("Button pressed")
    self.rising = True
    self.falling = False
    self.pressed = True
    self._managePress()
  
  def release(self):
    self.debug("Button released")
    self.rising = False
    self.falling = True
    self.pressed = False
    self._managePress()
  
  def _managePress(self):
    if self.game.mode == self.game.IDLE:
      return
    elif self.game.mode == self.game.MENU or self.game.mode == self.game.LOST:
      if self.falling:
        self.game.setMode(self.game.GAME)
    elif self.game.mode == self.game.GAME:
      if self.rising:
        self.game.setMode(self.game.PAUSE)
    elif self.game.mode == self.game.PAUSE:
      if self.rising:
        self._lastPress = time()
      if self.falling and self._lastPress is not None:
        if time()-self._lastPress < 3:
          self.game.setMode(self.game.GAME)
        else:
          self.game.setMode(self.game.MENU)
        self._lastPress = None

