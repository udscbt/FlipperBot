from .shared import gpio, Thread, sleep, time, Lock

class Display (Thread):
  def __init__(self, updateF, scrollF):
    self.segment = {
      'a' : 31,
      'b' : 32,
      'c' : 33,
      'd' : 35,
      'e' : 36,
      'f' : 37,
      'g' : 38,
      'dp': 40
    }
    self.cathode = {
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
    for seg in self.segment.values():
      gpio.setup(seg, gpio.OUT, initial=gpio.LOW)
    for cat in self.cathode.values():
      gpio.setup(cat, gpio.OUT, initial=gpio.HIGH)
    
    self.text = None
    self.dots = None
    self.tIndex = 0
    self.animT = time()
    self.textLock = Lock()
    self.values = None
    self.valLock = Lock()
    
    self.updateF = updateF
    self.scrollF = scrollF
    self._stopped = False
    self._actually_stopped = False
    
    super(self.__class__, self).__init__()

  def _raw(self, digits, on):
    for seg in self.segment.values():
      gpio.output(seg, gpio.LOW)
    for cat in self.cathode.values():
      gpio.output(cat, gpio.HIGH)
    for seg in on:
      gpio.output(self.segment[seg], gpio.HIGH)
    for digit in digits:
      gpio.output(self.cathode[digit], gpio.LOW)

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
    self.textLock.acquire()
    self.text = text
    self.dots = dots
    self.tIndex = 0
    self.animT = time()
    self.textLock.release()
  
  def _set_values(self, values):
    self.valLock.acquire()
    self.values = values
    self.valLock.release()
  
  def _get_text(self):
    self.textLock.acquire()
    text = self.text
    dots = self.dots
    self.textLock.release()
    return (text, dots) if text is not None else None
  
  def _get_values(self):
    self.valLock.acquire()
    values = self.values
    self.valLock.release()
    return values
  
  def shown(self):
    pair = self._get_text()
    if pair is not None:
      return pair[0]
    values = self._get_values()
    return values
  
  def stop(self):
    self._stopped = True
  
  def stopped(self):
    return self._actually_stopped
  
  def run(self):
    while not self._stopped:
      pair = self._get_text()
      self.textLock.acquire()
      tIndex = self.tIndex
      animT  = self.animT
      self.textLock.release()
      if pair is not None:
        text, dots = pair
        if len(text) > 4:
          if time() - animT > 1.0/self.scrollF:
            self.textLock.acquire()
            self.tIndex = self.tIndex+1
            self.animT = time()
            self.textLock.release()
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
    print("displayThread stopped")
    self._actually_stopped = True
