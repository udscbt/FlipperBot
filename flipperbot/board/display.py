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
  
  def __init__(self, updateF, scrollF, blinkF=0, debug=None):
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
    self.blinkF = blinkF
    self._shown = True
    self._blinkT = time()
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
    '0'   : "abcdef",
     0    : "abcdef",
    '0.'  : "abcdef.",  #dotted variant
    '1'   : "bc",
     1    : "bc",
    '1.'  : "bc.",      #dotted variant
    '2'   : "abedg",
     2    : "abedg",
    '2.'  : "abedg.",   #dotted variant
    '3'   : "abcdg",
     3    : "abcdg",
    '3.'  : "abcdg.",   #dotted variant
    '4'   : "bcfg",
     4    : "bcfg",
    '4.'  : "bcfg.",    #dotted variant
    '5'   : "acdfg",
     5    : "acdfg",
    '5.'  : "acdfg.",   #dotted variant
    '6'   : "acdefg",
     6    : "acdefg",
    '6.'  : "acdefg.",  #dotted variant
    '7'   : "abc",
     7    : "abc",
    '7.'  : "abc.",     #dotted variant
    '8'   : "abcdefg",
     8    : "abcdefg",
    '8.'  : "abcdefg.", #dotted variant
    '9'   : "abcdfg",
     9    : "abcdfg",
    '9.'  : "abcdfg.",  #dotted variant
    'A'   : "abcefg",
    'A.'  : "abcefg.",  #dotted variant
    'a'   : "abcdeg",
    'a.'  : "abcdeg.",  #dotted variant
    'B'   : "abcdefg",
    'B.'  : "abcdefg.", #dotted variant
    'b'   : "cdefg",
    'b.'  : "cdefg.",   #dotted variant
    'C'   : "adef",
    'C.'  : "adef.",    #dotted variant
    'c'   : "deg",
    'c.'  : "deg.",     #dotted variant
    'D'   : "abcdef",   #hardly recognizable
    'D.'  : "abcdef.",  #dotted variant
    'd'   : "bcdeg",
    'd.'  : "bcdeg.",   #dotted variant
    'E'   : "adefg",
    'E.'  : "adefg.",   #dotted variant
    'e'   : "abgdef",
    'e.'  : "abgdef.",  #dotted variant
    'F'   : "aefg",
    'F.'  : "aefg.",    #dotted variant
    'f'   : "aefg",     #actually uppercase
    'f.'  : "aefg.",    #dotted variant
    'G'   : "abcdfg",   #actually lowercase
    'G.'  : "abcdfg.",  #dotted variant
    'g'   : "abcdfg",
    'g.'  : "abcdfg.",  #dotted variant
    'H'   : "bcefg",
    'H.'  : "bcefg.",   #dotted variant
    'h'   : "cefg",
    'h.'  : "cefg.",    #dotted variant
    'I'   : "ef",
    'I.'  : "ef.",      #dotted variant
    'I>'  : "bc",       #right variant
    'I>.' : "bc.",      #dotted variant
    'I<'  : "ef",       #left variant
    'I<.' : "ef.",      #dotted variant
    'II'  : "bcef",
    'II.' : "bcef.",    #dotted variant
    'Il'  : "bcef",
    'Il.' : "bcef.",    #dotted variant
    'i'   : "c",
    'i.'  : "c.",       #dotted variant
    'i>'  : "c",        #right variant
    'i>.' : "c.",       #dotted variant
    'i<'  : "e",        #left variant
    'i<.' : "e.",       #dotted variant
    'ii'  : "ce",
    'ii.' : "ce.",      #dotted variant
    'il'  : "bce",
    'il.' : "bce.",     #dotted variant
    'J'   : "bcd",
    'J.'  : "bcd.",     #dotted variant
    'j'   : "cd",
    'j.'  : "cd.",      #dotted variant
    'K'   : "bcefg",    #hardly recognizable
    'K.'  : "bcefg.",   #dotted variant
    'k'   : "bcefg",    #actually uppercase
    'k.'  : "bcefg.",   #dotted variant
    'L'   : "def",
    'L.'  : "def.",     #dotted variant
    'l'   : "bc",
    'l.'  : "bc.",      #dotted variant
    'l>'  : "bc",       #right variant
    'l>.' : "bc.",      #dotted variant
    'l<'  : "ef",       #left variant
    'l<.' : "ef.",      #dotted variant
    'lI'  : "bcef",
    'lI.' : "bcef.",    #dotted variant
    'li'  : "cef",
    'li.' : "cef.",     #dotted variant
    'll'  : "bcef",
    'll.' : "bcef.",    #dotted variant
    'N'   : "ceg",      #actually lowercase
    'N.'  : "ceg.",     #dotted variant
    'n'   : "ceg",
    'n.'  : "ceg.",     #dotted variant
    'O'   : "abcdef",
    'O.'  : "abcdef.",  #dotted variant
    'o'   : "cdeg",
    'o.'  : "cdeg.",    #dotted variant
    'P'   : "abefg",
    'P.'  : "abefg.",   #dotted variant
    'p'   : "abefg",    #actually uppercase
    'p.'  : "abefg.",   #dotted variant
    'R'   : "abcefg",   #hardly recognizable
    'R.'  : "abcefg.",  #dotted variant
    'r'   : "eg",
    'r.'  : "eg.",      #dotted variant
    'S'   : "afgcd",
    'S.'  : "afgcd.",   #dotted variant
    's'   : "afgcd",    #actually uppercase
    's.'  : "afgcd.",   #dotted variant
    'T'   : "defg",     #actually lowercase
    'T.'  : "defg.",    #dotted variant
    't'   : "defg",
    't.'  : "defg.",    #dotted variant
    'U'   : "bcdef",
    'U.'  : "bcdef.",   #dotted variant
    'u'   : "cde",
    'u.'  : "cde.",     #dotted variant
    'Y'   : "befg",     #hardly recognizable
    'Y.'  : "befg.",    #dotted variant
    'y'   : "bcdfg",
    'y.'  : "bcdfg.",   #dotted variant
    '-'   : "g",
    '-.'  : "g.",       #dotted variant
    '_'   : "d",
    '_.'  : "d.",       #dotted variant
    '^'   : "a",
    '^.'  : "a.",       #dotted variant
    ' '   : "",
    '.'   : ".",
    '|'   : "ef",
    '|.'  : "ef.",      #dotted variant
    '|>'  : "bc",       #right variant
    '|>.' : "bc.",      #dotted variant
    '|<'  : "ef",       #left variant
    '|<.' : "ef.",      #dotted variant
    '?'   : "abeg",     #needs . in previous symbol
    '?.'  : "abeg.",    #dotted variant
    '!'   : "b.",
    '['   : "adef",
    '[.'  : "adef-",    #dotted variant
    ']'   : "abcd",
    '].'  : "abcd.",    #dotted variant
    '°'   : "abfg",
    '°.'  : "abfg.",    #dotted variant
    "'"   : "f",
    "'."  : "f.",       #dotted variant
    "'>"  : "b",        #right variant
    "'>." : "b.",       #dotted variant
    "'<"  : "f",        #left variant
    "'<." : "f.",       #dotted variant
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
    if self.blinkF == 0:
      self._shown = True
    elif time() - self._blinkT > 1.0/self.blinkF:
      self._shown = not self._shown
    if self._shown:
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
    else:
      self._raw([0,1,2,3], "")
      sleep(1.0/self.updateF)
    
  def setOptions(self, updateF, scrollF, blinkF):
    self.debug(
      "Changing options: {{ RefreshRate={}, ScrollSpeed={}, BlinkFreq={}}}".format(
        updateF, scrollF, blinkF
      )
    )
    self.updateF = updateF
    self.scrollF = scrollF
    self.blinkF = blinkF
    self._shown = True
  
  def setRefreshRate(self, updateF):
    self.debug("Changing refresh rate to {}".format(updateF))
    self.setOptions(updateF, self.scrollF, self.blinkF)
  
  def setScrollSpeed(self, scrollF):
    self.debug("Changing scroll speed to {}".format(scrollF))
    self.setOptions(self.updateF, scrollF, self.blinkF)
  
  def setBlinkFreq(self, blinkF):
    self.debug("Changing blink frequency to {}".format(blinkF))
    self.setOptions(self.updateF, self.scrollF, blinkF)



class DisplayEx (Display):  
  BAUDRATE = 9600
#  PORT = '/dev/ttyACM0'
  PORT = '/dev/ttyUSB0'
  
  def __init__(self, updateF, scrollF, blinkF=0, debug=None):
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
    self.blinkF = blinkF
    self._raw_values = {}
    self._raw_digits = {}
    super(Display, self).__init__(name="display")
    self.debug("Initialized")
  
  def _raw(self, digits, on):    
    raise NotImplementedError()
  
  def _show(self, digit, value, dot=False):
    NotImplementedError
  
  def _set_text(self, text, dots=None):
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
  
  def setup(self):
    # ssty -F <device> <baud> -hup
    self.serial = Serial(self.PORT, self.BAUDRATE)
    self.setOptions(self.updateF, self.scrollF, self.blinkF)
  
  def cleanup(self):
    self.show("")
    self.serial.close()
  
  def sendOptions(self):
    self.serial.write("({};{};{})".format(self.updateF, self.scrollF, self.blinkF).encode('utf-8'))
  
  def setOptions(self, updateF, scrollF, blinkF):
    self.debug(
      "Changing options: {{ RefreshRate={}, ScrollSpeed={}, BlinkFreq={}}}".format(
        updateF, scrollF, blinkF
      )
    )
    self.oldUpdateF = updateF;
    self.oldScrollF = scrollF;
    self.oldBlinkF = blinkF;
    self.updateF = updateF
    self.scrollF = scrollF
    self.blinkF = blinkF
    self.sendOptions()
  
  def setRefreshRate(self, updateF):
    self.debug("Changing refresh rate to {}".format(updateF))
    self.setOptions(updateF, self.scrollF, self.blinkF)
  
  def setScrollSpeed(self, scrollF):
    self.debug("Changing scroll speed to {}".format(scrollF))
    self.setOptions(self.updateF, scrollF, self.blinkF)
  
  def setBlinkFreq(self, blinkF):
    self.debug("Changing blink frequency to {}".format(blinkF))
    self.setOptions(self.updateF, self.scrollF, blinkF)

