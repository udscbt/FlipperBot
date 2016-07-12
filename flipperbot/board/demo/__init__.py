from ..game import Game
from ..totems import totems
from ..server import Server

from .fakeboard import FakeBoard
from .fakejoystick import FakeJoystick, VirtualJoystick
from .fakeeb import FakeEB
from .fakedisplay import FakeDisplay
from .fakeled import FakeLED
from .fakebot import FakeBot

from time import sleep

from tkinter import Tk

class Demo:
  #~ def __init__(self, remote=False):
    #~ self.remote = remote
    #~ self.g = Game(totems.values())
    #~ self.fb = FakeBoard(self.g)
    #~ self.fj = FakeJoystick(self.g)
    #~ self.fe = FakeEB(self.g)
    #~ self.fd = FakeDisplay(self.g)
    #~ self.g.start()
    #~ if self.remote:
      #~ self.s = Server(self.g)
      #~ self.s.start()
    #~ else:
      #~ self.g.controllers[0].active = True
    #~ sleep(1)
    #~ self.fb.loop()
    #~ self.fd.loop()
  
  #~ def stop(self):
    #~ self.g.stop()
    #~ if self.remote:
      #~ self.s.stop()
  def __init__(self, remote=False):
    try:
      self.remote = remote
      self.front = Tk()
      self.g = Game(totems.values())
      self.g.start()
      sleep(1)
      self.fb  = FakeBoard(self.g)
      self.fj  = FakeJoystick(self.g)
      self.fe  = FakeEB(self.g, master=self.front)
      self.fd  = FakeDisplay(self.g, master=self.front)
      self.fl1 = FakeLED(self.g.LED1, master=self.front)
      self.fl2 = FakeLED(self.g.LED2, master=self.front)
      self.fr  = FakeBot(self.g)
      if self.remote:
        self.s = Server(self.g)
        self.s.start()
      else:
        self.g.controllers[0].active = True
      self.front.columnconfigure(1, weight=8)
      self.front.columnconfigure(2, weight=8)
      self.front.columnconfigure(3, weight=4)
      self.front.columnconfigure(4, weight=1)
      self.front.rowconfigure(1, weight=1)
      self.fl1.grid(row=1, column=1, sticky="wens")
      self.fl2.grid(row=1, column=2, sticky="wens")
      self.fe.grid(row=1, column=3, sticky="wens")
      self.fd.grid(row=1, column=4, sticky="wens")
    except NameError as e:
      print(e)
      self.error = e
      self.stop()
  
  def stop(self):
    self.g.stop()
    if self.remote:
      self.s.stop()
