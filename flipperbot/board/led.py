from .shared import gpio, ThreadEx, sleep, SharedVariable

class LED (ThreadEx):
  _on = SharedVariable(False)
  _blink = SharedVariable(False)
  _bfreq = SharedVariable(0)
  
  def __init__(self, pin, *args, **kwargs):
    self.pin = pin
    gpio.setup(self.pin, gpio.OUT, initial=gpio.LOW)
    if not "name" in kwargs:
      kwargs["name"] = "LED[{}]".format(self.pin)
    super(LED, self).__init__(*args, **kwargs)
  
  def on(self):
    self._on = True
    self._blink = False
  
  def off(self):
    self._on = False
    self._blink = False
  
  def blink(self, freq):
    self._blink = True
    self._bfreq = freq
  
  def loop(self):
    if self._blink:
      self._on = not self._on
      sleep(1.0/self._bfreq)
    gpio.output(self.select, self._on)
  
  def cleanup(self):
    self.off()
