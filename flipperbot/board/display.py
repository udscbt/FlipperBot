from .shared import gpio, ThreadEx, sleep, time, Lock, SharedVariable

class Display (ThreadEx):
  text = SharedVariable(None)
  dots = SharedVariable(None)
  tIndex = SharedVariable(0)
  animT = SharedVariable(time())
  values = SharedVariable(None)
  updateF = SharedVariable()
  scrollF = SharedVariable()
  
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
  
  def __init__(self, updateF, scrollF):
    for seg in self.segment.values():
      gpio.setup(seg, gpio.OUT, initial=gpio.LOW)
    for cat in self.cathode.values():
      gpio.setup(cat, gpio.OUT, initial=gpio.HIGH)
    self.updateF = updateF
    self.scrollF = scrollF
    self._raw_values = {}
    self._raw_digits = {}
    super(self.__class__, self).__init__(name="display")
  
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
      if len(text) > 4:
        if time() - animT > 1.0/self.scrollF:
          self.tIndex = self.tIndex+1
          self.animT = time()
      for d in range(4):
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
