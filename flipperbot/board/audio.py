from .shared import gpio, Thread, sleep, time
from os.path import dirname, abspath, join
from subprocess import Popen, call
from random import random

audio_root = dirname(abspath(__file__))

class Audio:
  PAUSE = join(audio_root, "songs", "nachtwandel.ogg")
  GAME  = join(audio_root, "songs", "rainbow.ogg")
  MENU  = join(audio_root, "songs", "aquarium.ogg")
  LOST  = join(audio_root, "songs", "spaceoutro.ogg")
  
  READY = join(audio_root, "sounds", "start.ogg")
  START = [
    join(audio_root, "sounds", 'flipperbot7.ogg'),
  ]
  RSME  = [
    join(audio_root, "sounds", 'flipperbot1.ogg'),
  ]
  HIT   = [
    join(audio_root, "sounds", 'flipperbot3.ogg'),
    join(audio_root, "sounds", 'flipperbot5.ogg'),
    join(audio_root, "sounds", 'flipperbot9.ogg'),
    join(audio_root, "sounds", 'flipperbot12.ogg'),
  ]
  LOSE  = [
    join(audio_root, "sounds", 'flipperbot4.ogg'),
    join(audio_root, "sounds", 'flipperbot6.ogg'),
    join(audio_root, "sounds", 'flipperbot14.ogg'),
    join(audio_root, "sounds", 'flipperbot11.ogg'),
  ]
  RAND  = [
    join(audio_root, "sounds", 'flipperbot2.ogg'),
    join(audio_root, "sounds", 'flipperbot10.ogg'),
    join(audio_root, "sounds", 'flipperbot13.ogg'),
    join(audio_root, "sounds", 'flipperbot8.ogg'),
  ]
  OOT   = [
    join(audio_root, "sounds", 'flipperbot14.ogg'),
    join(audio_root, "sounds", 'flipperbot15.ogg'),
  ]
  
  #PLAYER = "ogg123"
  #PL_OPTIONS = ["-r"]
  PLAYER = "cvlc"
  PL_OPTIONS = ["-R"]
  
  def __init__(self):
    #call(["amixer", "set", "PCM", "100%"])
    self.proc = None
    self._started = False
    self._stopped = False
  
  def start(self, song):
    self.stop()
    self.log = open(join(audio_root, "{:d}.log".format(int(time()))), "w")
#    self.proc = Popen([self.PLAYER, *self.PL_OPTIONS, song], stdout=self.log, stderr=self.log)
    self.proc = Popen([self.PLAYER] + self.PL_OPTIONS + [song], stdout=self.log, stderr=self.log)
    self._started = True
    self._stopped = False

  def stop(self, async=False):
    if self.proc is not None:
      self.proc.terminate()
      if not async:
        self.proc.wait()
      self.log.close()
    self.proc = None
    self._stopped = True
  
  def started(self):
    return self._started
  
  def stopped(self):
    return self._stopped

class SoundEffect:
  #PLAYER = "ogg123"
  #PL_OPTIONS = []
  PLAYER = "cvlc"
  PL_OPTIONS = ["--play-and-exit"]
  
  def __init__(self, sound):
    if isinstance(sound, list):
      self.sound = sound[int(random()*len(sound))]
    else:
      self.sound = sound
    self.proc = None
    self._started = False
    self._stopped = False
  
  def start(self):
    self.stop()
#    self.proc = Popen([self.PLAYER, *self.PL_OPTIONS, self.sound])
    self.proc = Popen([self.PLAYER] + self.PL_OPTIONS + [self.sound])
    self._started = True
    self._stopped = False
  
  def ended(self):
    if self.proc is None:
      return True
    self.proc.poll()
    if self.proc.returncode is None:
      self.proc = None
      return True
    return False
  
  def wait(self):
    self.proc.wait()
    self.proc = None
  
  def stop(self):
    if self.proc is not None:
      self.proc.terminate()
    self.proc = None
    self._stopped = True
  
  def started(self):
    return self._started
  
  def stopped(self):
    return self._stopped
