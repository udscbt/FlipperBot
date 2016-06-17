from ..game import Game
from ..totems import totems
from ..server import Server

from .fakeboard import FakeBoard
from .fakejoystick import FakeJoystick
from .fakeeb import FakeEB
from .fakedisplay import FakeDisplay

from time import sleep

class Demo:
  def __init__(self):
    self.g = Game(totems.values())
    self.s = Server(self.g)
    self.fb = FakeBoard(self.g)
    self.fj = FakeJoystick(self.g)
    self.fe = FakeEB(self.g)
    self.fd = FakeDisplay(self.g)
    self.g.start()
    self.s.start()
    sleep(1)
    self.fb.loop()
    self.fj.loop()
    self.fe.loop()
    self.fd.loop()
  
  def stop(self):
    self.g.stop()
    self.s.stop()
