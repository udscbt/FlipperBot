from .shared import gpio, Thread, sleep, time
from os.path import dirname, abspath, join
from subprocess import Popen, call

audio_root = dirname(abspath(__file__))

class Audio:
  PAUSE = join(audio_root, "songs", "spaceoutro.ogg")
  GAME  = join(audio_root, "songs", "rainbow.ogg")
  MENU  = join(audio_root, "songs", "aquarium.ogg")
  START = join(audio_root, "sounds", "start.ogg")
  
  def __init__(self):
    call(["amixer", "set", "PCM", "100%"])
    self.proc = None
  
  def start(self, song):
    self.stop()
    self.log = open(join(audio_root, "{:d}.log".format(int(time()))), "w")
    self.proc = Popen(["ogg123", "-r", song], stdout=self.log, stderr=self.log)

  def stop(self):
    if self.proc is not None:
      self.proc.terminate()
      self.log.close()
    self.proc = None

class SoundEffect:
  def __init__(self, sound):
    self.sound = sound
    self.proc = None
  
  def start(self):
    self.stop()
    self.proc = Popen(["ogg123", sound])
  
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
