from .shared import gpio, Thread, sleep, time
from os.path import dirname, abspath, join
from subprocess import Popen, call

audio_root = dirname(abspath(__file__))

class Audio:
  PAUSE = join(audio_root, "songs", "spaceoutro.ogg")
  GAME  = join(audio_root, "songs", "rainbow.ogg")
  MENU  = join(audio_root, "songs", "aquarium.ogg")
  START = join(audio_root, "songs", "start.ogg")
  
  def __init__(self):
    call(["amixer", "set", "100%"])
    self.pid = None
  
  def start(self, song):
    self.stop()
    self.log = open(join(audio_root, "{:d}.log".format(int(time()))), "w")
    proc = Popen(["ogg123", song], shell=False, stdout=self.log, stderr=self.log)
    self.pid = proc.pid
  
  def stop(self):
    if self.pid is not None:
      call(["kill", "%d" % self.pid])
      self.log.close()
    self.pid = None

