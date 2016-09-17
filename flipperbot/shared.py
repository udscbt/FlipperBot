from time import sleep, time
from threading import Thread, Lock

termColors = {
  'PURPLE' : '\033[95m',
  'BLUE' : '\033[94m',
  'GREEN' : '\033[92m',
  'YELLOW' : '\033[93m',
  'RED' : '\033[91m',
  'END' : '\033[0m',
  'BOLD' : '\033[1m',
  'UNDERLINE' : '\033[4m'
}

class SharedVariable:
  _counter = 0
  _cLock = Lock()
  
  def __init__(self, default=None):
    self._classLock  = Lock()
    self._classValue = default
    self.__class__._cLock.acquire()
    self._name = "_{}_{}".format(self.__class__.__name__, self.__class__._counter)
    self._lock = "{}_lock".format(self._name)
    self.__class__._counter = self.__class__._counter + 1
    self.__class__._cLock.release()
  
  def _check(self, instance):
    return hasattr(instance, self._name)
  
  def _init(self, instance):
    setattr(instance, self._name, self._classValue)
    setattr(instance, self._lock, Lock())
  
  def _lockObj(self, instance):
    getattr(instance, self._lock).acquire()
  
  def _unlockObj(self, instance):
    getattr(instance, self._lock).release()
  
  def __get__(self, instance, owner):
    if instance is None or not self._check(instance):
      self._classLock.acquire()
      value = self._classValue
      self._classLock.release()
      return value
    else:
      self._lockObj(instance)
      value = getattr(instance, self._name)
      self._unlockObj(instance)
      return value
  
  def __set__(self, instance, value):
    if instance is None:
      self._classLock.acquire()
      self._classValue = value
      self._classLock.release()
    else:
      if not self._check(instance):
        self._init(instance)
      self._lockObj(instance)
      setattr(instance, self._name, value)
      self._unlockObj(instance)


class ThreadEx (Thread):
  _paused  = SharedVariable(False)
  _stopped = SharedVariable(False)
  _actually_stopped = SharedVariable(False)
  _children = SharedVariable()
  
  def running(f):
    def wrapped(self, *args, **kwargs):
      if self._stopped:
        raise RuntimeError("Thread has already been stopped")
      else:
        f(self, *args, **kwargs)
    return wrapped
  
  def __init__(self, *args, **kwargs):
    if kwargs.get('setup', None) is not None:
      self.loop = lambda: kwargs['setup'](self)
      kwargs.pop('setup')
    if kwargs.get('loop', None) is not None:
      self.loop = lambda: kwargs['loop'](self)
      kwargs.pop('loop')
    if kwargs.get('cleanup', None) is not None:
      self.loop = lambda: kwargs['cleanup'](self)
      kwargs.pop('cleanup')
    if kwargs.get('parent', None) is not None:
      self.parent = kwargs['parent']
      self.parent.addChild(self)
      kwargs.pop('parent')
    self._children = []
    super(ThreadEx, self).__init__(*args, **kwargs)
  
  @running
  def start(self):
    self._paused = False
    self._stopped = False
    self._actually_stopped = False
    super(ThreadEx, self).start()
  
  @running
  def pause(self):
    self._paused = True
  
  @running
  def resume(self):
    self._paused = False
  
  def stop(self):
    self._stopped = True
    if not self.started():
      self._actually_stopped = True
  
  def started(self):
    return self._started.isSet()
  
  def stopped(self):
    return self._actually_stopped
  
  def paused(self):
    return self._paused
  
  def setup(self):
    pass
  
  def loop(self):
    pass
  
  def cleanup(self):
    pass
  
  def run(self):
    self.setup()
    while not self._stopped:
      if not self._paused:
        self.loop()
    for child in self._children:
      child.stop()
    for child in self._children:
      child.wait()
    self.cleanup()
    self._actually_stopped = True
  
  def wait(self):
    while not self.stopped():
      pass
  
  @running
  def addChild(self, child):
    self._children.append(child)
