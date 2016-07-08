from .shared import gpio, Thread, sleep, time

class EverythingButton:
  def __init__(self, game):
    self.game = game
    self.everythingButton = 18
    gpio.setup(self.everythingButton, gpio.IN, pull_up_down=gpio.PUD_DOWN)
    self._lastPress = None
    self._started = False
    self._stopped = False
    super(EverythingButton, self).__init__()
  
  def start(self):
    if not self._started:
      gpio.add_event_detect(self.everythingButton, gpio.BOTH, callback=self._managePress, bouncetime=50)
      self._started = True
  
  def stop(self):
    gpio.remove_event_detect(self.everythingButton)
    self._stopped = True
  
  def started(self):
    return self._started
  
  def stopped(self):
    return self._stopped
  
  def _managePress(self, channel):
    if gpio.input(channel):
      rising = True
      falling = False
    else:
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

