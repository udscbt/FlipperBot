from .shared import gpio, Thread, sleep

class Totem (Thread):
  def __init__(self, dic):
    self.select  = dic['select']
    self.hit     = dic['hit']
    self._on     = False
    self._blink  = False
    self._stopped = True
    gpio.setup(self.select, gpio.OUT, initial=gpio.LOW)
    gpio.setup(self.hit, gpio.IN, pull_up_down=gpio.PUD_DOWN)
    self._stopped = False
    self._actually_stopped = False
    
    super(self.__class__, self).__init__()
  
  def on(self):
    self._on = True
    self._blink = False
  
  def off(self):
    self._on = False
    self._blink = False
  
  def isHit(self):
    return gpio.input(self.hit)
  
  def blink(self, freq):
    self._blink = True
    self.bfreq = freq
  
  def stop(self):
    self.off()
    self._stopped = True
  
  def stopped(self):
    return self._actually_stopped
  
  def run(self):
    while not self._stopped:
      if self._blink:
        self._on = not self._on
        sleep(1.0/self.bfreq)
      gpio.output(self.select, self._on)
    print("totemThread stopped")
    self._actually_stopped = True


totems = {
  'DL' : Totem({'select' : 5,
    'hit' : 8
  }),
  'DR' : Totem({
    'select' : 15,
    'hit' : 16
  }),
  'UR'  : Totem({
    'select' : 21,
    'hit' : 22
  }),
  'UL' : Totem({
    'select' : 23,
    'hit' : 24
  }),
  'C' : Totem({
    'select' : 3,
    'hit' : 7
  })
}
