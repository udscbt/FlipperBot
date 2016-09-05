from .shared import gpio, ThreadEx, time, sleep, SharedVariable

from .debug.debug import Debug, fakedebug

class BlinkThread (ThreadEx):
  def __init__(self):
    self.leds = []
    self.blink = {}
    super(BlinkThread, self).__init__()

  def setBlink(self, led, freq):
    self.blink[led] = (freq, time())
    if not self.started():
      self.start()

  def unsetBlink(self, led):
    try:
      self.blink.pop(led)
    except:
      pass
  
  def add(self, led):
    self.leds.append(led)
  
  def remove(self, led):
    try:
      self.leds.remove(led)
      self.unsetBlink(led)
    except:
      pass
    if len(self.leds) == 0:
      self.stop()

  def loop(self):
    leds = list(self.blink.keys())
    for led in leds:
      f,t = self.blink.get(led, (None, None))
      if f is None:
        continue
      tn = time()
      if tn-t > 1.0/f:
        led.toggle()
        self.blink[led] = (f, time())

class LED (ThreadEx):
  _on = SharedVariable(False)
  _blink = SharedVariable(False)
  
  root = None
  
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
    self.update()
  
  def off(self):
    self.debug("Turned off")
    self._on = False
    self._blink = False
    self.root.unsetBlink(self)
    self.update()

  def toggle(self):
    self.debug("Changed status")
    self._on = not self._on
    self.root.unsetBlink(self)
    self.update()

  def update(self):
    gpio.output(self.pin, self._on)
  
  def blink(self, freq):
    if not self._blink or self._bfreq != freq:
      self.debug("Blinking at {}Hz".format(freq))
      self.root.setBlink(self, freq)
      self._blink = True
      self._bfreq = freq
  
  def cleanup(self):
    self.off()
    self.debug("Stopping")

  def start(self):
    if LED.root is None:
      LED.root = BlinkThread()
    self._paused = False
    self._stopped = False
    self._actually_stopped = False
    self.root.add(self)
  
  def stop(self):
    self.root.remove(self)
    self.cleanup()
    self._stopped = True
    self._actually_stopped = True
