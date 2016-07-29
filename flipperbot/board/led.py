from .shared import gpio, ThreadEx, sleep, SharedVariable

from .debug.debug import Debug, fakedebug

class LED (ThreadEx):
  _on = SharedVariable(False)
  _blink = SharedVariable(False)
  _bfreq = SharedVariable(0)
  
  def __init__(self, pin, *args, debug=None, **kwargs):
    if debug is None:
      self.debug = fakedebug
    else:
      self.debug = Debug(
        log=debug.log,
        logging=debug.logging,
        stdout=debug.stdout,
        parent=self,
        name="LED<{}>".format(pin)
      )
    self.pin = pin
    gpio.setup(self.pin, gpio.OUT, initial=gpio.LOW)
    if not "name" in kwargs:
      kwargs["name"] = "LED[{}]".format(self.pin)
    super(LED, self).__init__(*args, **kwargs)
    self.debug("Initialized")
  
  def on(self):
    self.debug("Turned on")
    self._on = True
    self._blink = False
  
  def off(self):
    self.debug("Turned off")
    self._on = False
    self._blink = False
  
  def blink(self, freq):
    self.debug("Blinking at {}Hz".format(freq))
    self._blink = True
    self._bfreq = freq
  
  def loop(self):
    if self._blink:
      self._on = not self._on
      sleep(1.0/self._bfreq)
    gpio.output(self.pin, self._on)
  
  def cleanup(self):
    self.off()
    self.debug("Stopping")
