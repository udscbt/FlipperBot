from .shared import gpio, Thread, sleep, time
from os.path import dirname, abspath, join
from subprocess import Popen, call, DEVNULL
from random import random

from .debug.log import Log
from .debug.debug import Debug, fakedebug

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
  SUCCESS = [
    join(audio_root, "sounds", 'flipperbot7.ogg'),
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
  
  def __init__(self, logging=True, debug=None):
    #call(["amixer", "set", "PCM", "100%"])
    if debug is None:
      self.debug = fakedebug
    else:
      self.debug = Debug(
        log=debug.log,
        logging=debug.logging,
        stdout=debug.stdout,
        parent=self,
        name="Audio"
      )
    self.proc = None
    self._started = False
    self._stopped = False
    self.debug("Initialized")
  
  def start(self, song):
    self.stop()
    self.debug("Playing song '{}'".format(song))
    self.log = open(join(audio_root, "logs", "audio", "{:d}.log".format(int(time()))), "w")
#    self.proc = Popen([self.PLAYER, *self.PL_OPTIONS, song], stdout=self.log, stderr=self.log)
    #~ self.proc = Popen([self.PLAYER] + self.PL_OPTIONS + [song], stdout=self.log, stderr=self.log)
    self.proc = False
    self._started = True
    self._stopped = False

  def stop(self, async=False):
    try:
      self.debug("Stopping last song")
      self.proc.terminate()
      if not async:
        self.debug("Waiting for last song to stop")
        self.proc.wait()
        self.debug("Last song stopped")
      self.log.close()
    except:
      self.debug("Last song was already stopped")
    self.proc = None
    self._stopped = True
  
  def started(self):
    return self._started
  
  def stopped(self):
    return self._stopped
  
  def wait(self):
    while not self.stopped():
      pass
  

class SoundEffect:
  #PLAYER = "ogg123"
  #PL_OPTIONS = []
  PLAYER = "cvlc"
  PL_OPTIONS = ["--play-and-exit"]
  
  def __init__(self, sound, debug=None):
    if debug is None:
      self.debug = fakedebug
    else:
      self.debug = Debug(
        log=debug.log,
        logging=debug.logging,
        stdout=debug.stdout,
        parent=self,
        name="SoundEffect"
      )
    if isinstance(sound, list):
      self.sound = sound[int(random()*len(sound))]
    else:
      self.sound = sound
    self.proc = None
    self._started = False
    self._stopped = False
    self.debug("Inizialized")
  
  def start(self):
    self.stop()
    self.debug("Playing sound effect '{}'".format(self.sound))
#    self.proc = Popen([self.PLAYER, *self.PL_OPTIONS, self.sound])
    #~ self.proc = Popen([self.PLAYER] + self.PL_OPTIONS + [self.sound], stdout=DEVNULL, stderr=DEVNULL)
    self.proc = None
    self._started = True
    self._stopped = False
  
  def ended(self):
    try:
      self.proc.poll()
    except:
      return True
    if self.proc.returncode is None:
      self.proc = None
      return True
    return False
  
  def wait(self):
    self.debug("Waiting for sound effect to terminate")
    try:
      self.proc.wait()
    except:
      self.debug("Sound effect was already stopped")
    self.proc = None
  
  def stop(self):
    try:
      self.debug("Stopping sound effect")
      self.proc.terminate()
    except:
      self.debug("Sound effect was already stopped")
    self.proc = None
    self._stopped = True
  
  def started(self):
    return self._started
  
  def stopped(self):
    return self._stopped
