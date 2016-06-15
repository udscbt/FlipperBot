from .shared import gpio, ThreadEx, sleep, SharedVariable

class Totem (ThreadEx):
  _on = SharedVariable(False)
  _blink = SharedVariable(False)
  _bfreq = SharedVariable(0)
  
  def __init__(self, dic):
    self.select = dic['select']
    self.hit    = dic['hit']
    self._hit   = False
    gpio.setup(self.select, gpio.OUT, initial=gpio.LOW)
    gpio.setup(self.hit, gpio.IN, pull_up_down=gpio.PUD_DOWN)
    super(self.__class__, self).__init__(name="totem")
  
  def on(self):
    self._on = True
    self._blink = False
  
  def off(self):
    self._on = False
    self._blink = False
  
  def _manageHit(self, channel):
    self._hit = gpio.input(self.hit)
  
  def isHit(self):
    return self._hit
  
  def blink(self, freq):
    self._blink = True
    self._bfreq = freq
  
  def setup(self):
    gpio.add_event_detect(self.hit, gpio.BOTH, callback=self._manageHit, bouncetime=50)
  
  def loop(self):
    if self._blink:
      self._on = not self._on
      sleep(1.0/self._bfreq)
    gpio.output(self.select, self._on)
  
  def cleanup(self):
    self.off()
    gpio.remove_event_detect(self.hit)

totems = {
  'DL' : Totem({
    'select' : 5,
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
