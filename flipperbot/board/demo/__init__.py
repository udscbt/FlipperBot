from ..game import Game
from ..totems import totems
from ..server import Server

from .fakeboard import FakeBoard
from .fakejoystick import FakeJoystick, VirtualJoystick
from .fakeeb import FakeEB
from .fakedisplay import FakeDisplay

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
      self.root = Tk()
      self.g = Game(totems.values())
      self.g.start()
      sleep(1)
      self.fb = FakeBoard(self.g, master=self.root)
      self.fj = VirtualJoystick(self.g, master=self.root)
      self.fe = FakeEB(self.g, master=self.root)
      self.fd = FakeDisplay(self.g, master=self.root)
      if self.remote:
        self.s = Server(self.g)
        self.s.start()
      else:
        self.g.controllers[0].active = True
      self.root.columnconfigure(1, weight=1)
      self.root.columnconfigure(2, weight=1)
      self.root.rowconfigure(1, weight=1)
      self.root.rowconfigure(2, weight=1)
      self.root.rowconfigure(3, weight=1)
      self.fb.grid(row=1, column=1, sticky="wens", rowspan=3)
      self.fj.grid(row=1, column=2, sticky="wens")
      self.fe.grid(row=2, column=2, sticky="wens")
      self.fd.grid(row=3, column=2, sticky="wens")
    except NameError as e:
      print(e)
      self.error = e
      self.stop()
  
  def stop(self):
    self.g.stop()
    if self.remote:
      self.s.stop()
