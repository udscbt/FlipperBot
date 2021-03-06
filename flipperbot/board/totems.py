from .shared import gpio, ThreadEx, time, sleep, SharedVariable
from .led import LED

from .debug.debug import Debug, fakedebug

class Totem (LED):
  
  def __init__(self, dic, debug=None):
    self.select = dic['select']
    super(Totem, self).__init__(self.select, name="totem", debug=debug)
    if debug is not None:
      self.debug.name = "Totem<{}>".format(dic['pos'])
    self.hit    = dic['hit']
    self.pos   = dic['pos']
    self._hit   = False
    gpio.setup(self.hit, gpio.IN, pull_up_down=gpio.PUD_DOWN)
  
  def _manageHit(self, channel):
    self.debug("Hit")
    if gpio.input(self.hit):
      self._hit = time()
  
  def clearHit(self):
    self._hit = None
  
  def isHit(self):
    return self._hit is not None
  
  def isCurrentlyHit(self):
    return gpio.input(self.hit)
  
  def setup(self):
    super(Totem, self).setup()
    gpio.add_event_detect(self.hit, gpio.BOTH, callback=self._manageHit, bouncetime=50)
  
  def cleanup(self):
    super(Totem, self).cleanup()
    gpio.remove_event_detect(self.hit)

totems = {
  'DL' : Totem({
    'select' : 5,
    'hit' : 8,
    'pos' : 'DL'
  }),
  'DR' : Totem({
    'select' : 15,
    'hit' : 16,
    'pos' : 'DR'
  }),
  'UR'  : Totem({
    'select' : 21,
    'hit' : 22,
    'pos' : 'UR'
  }),
  'UL' : Totem({
    'select' : 23,
    'hit' : 24,
    'pos' : 'UL'
  }),
  'C' : Totem({
    'select' : 3,
    'hit' : 7,
    'pos' : 'C'
  })
}
