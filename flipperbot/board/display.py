from .shared import gpio, ThreadEx, sleep, time, Lock, SharedVariable
from serial import Serial

from .debug.debug import Debug, fakedebug

class Display (ThreadEx):
  text = SharedVariable(None)
  dots = SharedVariable(None)
  tIndex = SharedVariable(0)
  animT = SharedVariable(time())
  values = SharedVariable(None)
  
  segment = {
    'a' : 31,
    'b' : 32,
    'c' : 33,
    'd' : 35,
    'e' : 36,
    'f' : 37,
    'g' : 38,
    'dp': 40
  }
  cathode = {
    # Label numbering
    # From right to left
    '0' : 10,
    '1' : 11,
    '2' : 12,
    '3' : 13,
    
    # Software numbering
    # From left to right
    0 : 13,
    1 : 12,
    2 : 11,
    3 : 10
  }
  
  def __init__(self, updateF, scrollF, debug=None):
    if debug is None:
      self.debug = fakedebug
    else:
      self.debug = Debug(
        log=debug.log,
        logging=debug.logging,
        stdout=debug.stdout,
        parent=self,
        name="Display"
      )
    for seg in self.segment.values():
      gpio.setup(seg, gpio.OUT, initial=gpio.LOW)
    for cat in self.cathode.values():
      gpio.setup(cat, gpio.OUT, initial=gpio.HIGH)
    self.updateF = updateF
    self.scrollF = scrollF
    self._raw_values = {}
    self._raw_digits = {}
    super(self.__class__, self).__init__(name="display")
    self.debug("Initialized")
  
  def _raw(self, digits, on):    
    for name, seg in self.segment.items():
      gpio.output(seg, gpio.LOW)
      self._raw_values[name] = False
    for name, cat in self.cathode.items():
      gpio.output(cat, gpio.HIGH)
      self._raw_values[name] = False
      
    for seg in on:
      gpio.output(self.segment[seg], gpio.HIGH)
      self._raw_values[seg] = True
    for digit in digits:
      gpio.output(self.cathode[digit], gpio.LOW)
      self._raw_values[digit] = True
    
    for i in range(4):
      if i in digits:
        self._raw_digits[i] = self._raw_values
  
  valuemap = {
     0  : "abcdef",
    '0' : "abcdef",
     1  : "bc",
    '1' : "bc",
     2  : "abedg",
    '2' : "abedg",
     3  : "abcdg",
    '3' : "abcdg",
     4  : "bcfg",
    '4' : "bcfg",
     5  : "acdfg",
    '5' : "acdfg",
     6  : "acdefg",
    '6' : "acdefg",
     7  : "abc",
    '7' : "abc",
     8  : "abcdefg",
    '8' : "abcdefg",
     9  : "abcdfg",
    '9' : "abcdfg",
    'A' : "abcefg",
    "B" : "abcdefg",
    'b' : "cdefg",
    'C' : "adef",
    'c' : "deg",
    'd' : "bcdeg",
    'E' : "adefg",
    'e' : "abgdef",
    'F' : "aefg",
    'H' : "bcefg",
    'h' : "cefg",
    'I' : "bc",
    'i' : "c",
    'J' : "bcd",
    'j' : "cd",
    'L' : "def",
    'l' : "ef",
    'li': "cef",
    'O' : "abcdef",
    'o' : "cdeg",
    'P' : "abefg",
    'p' : "abefg",
    'r' : "eg",
    'S' : "afgcd",
    't' : "defg",
    'U' : "bcdef",
    'u' : "cde",
    'y' : "bcdfg",
    '-' : "g",
    '_' : "d",
    '^' : "a",
    ' ' : "",
    '.' : ["dp"],
    '|' : "ef"
  }
  
  def _show(self, digit, value, dot=False):
    if value not in Display.valuemap:
      return False
    else:
      on = Display.valuemap[value]
    
    if dot:
      on = list(on)+["dp"]
    
    self._raw([digit], on)
    return True
  
  def raw(self, values):
    self._set_text(None)
    self._set_values(values)
  
  def show(self, text, dots=None):
    if dots is None:
      dots = [False]*len(text)
    self.debug(
      "Showing text '{}'".format(
        "".join(
          [x+("." if y else "") for x,y in zip(text,dots)]
        )
      )
    )
    self._set_text(text, dots)
    self._set_values(None)
  
  def _set_text(self, text, dots=None):
    if dots is None:
      dots = [False]*len(text)
    self.text = text
    self.dots = dots
    self.tIndex = 0
    self.animT = time()
  
  def _set_values(self, values):
    self.values = values
  
  def _get_text(self):
    return (self.text, self.dots) if self.text is not None else None
  
  def _get_values(self):
    return self.values
  
  def shown(self):
    pair = self._get_text()
    if pair is not None:
      return pair[0]
    values = self._get_values()
    return values
  
  def loop(self):
    pair = self._get_text()
    tIndex = self.tIndex
    animT  = self.animT
    if pair is not None:
      text, dots = pair
      tl = len(text)
      if tl > 4:
        tl = 4
        if time() - animT > 1.0/self.scrollF:
          self.tIndex = self.tIndex+1
          self.animT = time()
      for d in range(tl):
        index = (d+tIndex)%len(text)
        self._show(d, text[index], dots[index])
        sleep(0.25/self.updateF)
    else:
      values = self._get_values()
      if values is not None:
        for d in range(4):
          self._raw([d], values[d])
          sleep(0.25/self.updateF)
      else:
        self._raw([0,1,2,3], "")
        sleep(1.0/self.updateF)
    
  def setOptions(self, updateF, scrollF):
    self.debug(
      "Changing options: {{ RefreshRate={}, ScrollSpeed={}}}".format(
        updateF, scrollF
      )
    )
    self.updateF = updateF
    self.scrollF = scrollF
  
  def setRefreshRate(self, updateF):
    self.debug("Changing refresh rate to {}".format(updateF))
    self.setOptions(updateF, self.scrollF)
  
  def setScrollSpeed(self, scrollF):
    self.debug("Changing scroll speed to {}".format(scrollF))
    self.setOptions(self.updateF, scrollF)

class DisplayEx (Display):  
  BAUDRATE = 9600
#  PORT = '/dev/ttyACM0'
  PORT = '/dev/ttyUSB0'
  
  def __init__(self, updateF, scrollF, debug=None):
    if debug is None:
      self.debug = fakedebug
    else:
      self.debug = Debug(
        log=debug.log,
        logging=debug.logging,
        stdout=debug.stdout,
        parent=self,
        name="Display"
      )
    self.updateF = updateF
    self.scrollF = scrollF
    self._raw_values = {}
    self._raw_digits = {}
    super(Display, self).__init__(name="display")
    self.debug("Initialized")
    self.ready = False
  
  def _raw(self, digits, on):    
    raise NotImplementedError()
  
  def _show(self, digit, value, dot=False):
    NotImplementedError

  @ThreadEx.running
  def _set_text(self, text, dots=None):
    while not self.ready:
      pass
    if dots is None:
      dots = [False]*len(text)
    self.text = text
    self.dots = dots
    self.tIndex = 0
    self.animT = time()
    n = len(text)
    for i in range(n):
      self.serial.write(text[i].encode('utf-8'))
      try:
        if dots[i]:
          self.serial.write(b'.')
      except:
        pass
      if i < n-1:
        self.serial.write(b':')
    self.serial.write(b';')
  
  def loop(self):
    if self.oldUpdateF != self.updateF or self.oldScrollF != self.scrollF:
      self.oldUpdateF = self.updateF;
      self.oldScrollF = self.scrollF;
    
    pair = self._get_text()
    tIndex = self.tIndex
    animT  = self.animT
    if pair is not None:
      text, dots = pair
      tl = len(text)
      if tl > 4:
        tl = 4
        if time() - animT > 1.0/self.scrollF:
          self.tIndex = self.tIndex+1
          self.animT = time()
      for d in range(tl):
        index = (d+tIndex)%len(text)
        sleep(0.25/self.updateF)
    else:
      values = self._get_values()
      if values is not None:
        for d in range(4):
          sleep(0.25/self.updateF)
      else:
        sleep(1.0/self.updateF)
  
  def run(self):
    self.setup()
  
  def stop(self):
    self.cleanup()
    super(Display, self).stop()
    self._actually_stopped = True
  
  def setup(self):
    # ssty -F <device> <baud> -hup
    self.serial = Serial(self.PORT, self.BAUDRATE)
    self.setOptions(self.updateF, self.scrollF)
    self.ready = True
  
  def cleanup(self):
    self.show("")
    self.serial.close()
  
  def sendOptions(self):
    self.serial.write("({};{})".format(self.updateF, self.scrollF).encode('utf-8'))
  
  def setOptions(self, updateF, scrollF):
    self.debug(
      "Changing options: {{ RefreshRate={}, ScrollSpeed={}}}".format(
        updateF, scrollF
      )
    )
    self.oldUpdateF = updateF;
    self.oldScrollF = scrollF;
    self.updateF = updateF
    self.scrollF = scrollF
    self.sendOptions()
  
  def setRefreshRate(self, updateF):
    self.debug("Changing refresh rate to {}".format(updateF))
    self.setOptions(updateF, self.scrollF)
  
  def setScrollSpeed(self, scrollF):
    self.debug("Changing scroll speed to {}".format(scrollF))
    self.setOptions(self.updateF, scrollF)

