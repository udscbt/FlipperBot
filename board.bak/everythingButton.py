from .shared import gpio, Thread, sleep, time

class EverythingButton (Thread):
  def __init__(self, game):
    self.game = game
    self.everythingButton = 18
    gpio.setup(self.everythingButton, gpio.IN, pull_up_down=gpio.PUD_DOWN)
    self._stopped = False
    self._actually_stopped = False
    self._down = None
    
    super(self.__class__, self).__init__()
  
  def stop(self):
    self._stopped = True
  
  def stopped(self):
    return self._actually_stopped
  
  def pressed(self):
    return gpio.input(self.everythingButton)
  
  def run(self):
    press = None
    while not self._stopped:
      
      if self.game.mode == self.game.MENU:
        to = gpio.wait_for_edge(self.everythingButton, gpio.FALLING, timeout=100)
        if to is None:
          continue
        self.game.setMode(self.game.GAME)
        
      elif self.game.mode == self.game.GAME:
        to = gpio.wait_for_edge(self.everythingButton, gpio.RISING, timeout=100)
        if to is None:
          continue
        self.game.setMode(self.game.PAUSE)
        
      elif self.game.mode == self.game.PAUSE:
        if press is None:
          to = gpio.wait_for_edge(self.everythingButton, gpio.RISING, timeout=100)
          if to is None:
            continue
          press = time()
        to = gpio.wait_for_edge(self.everythingButton, gpio.FALLING, timeout=100)
        if to is None:
          continue
        if time() - press < 3:
          self.game.setMode(self.game.GAME)
        else:
          self.game.setMode(self.game.MENU)
        press = None
      
      sleep(0.1)
    print("pauseThread stopped")
    self._actually_stopped = True

