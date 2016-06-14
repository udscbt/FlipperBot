import RPi.GPIO as gpio
from time import sleep, time
from threading import Thread, Lock

gpio.setmode(gpio.BOARD)

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
    getattr(instance, self._lock).aquire()
  
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
