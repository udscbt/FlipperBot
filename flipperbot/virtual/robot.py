import tkinter as tk
from ..controller import Controller
from collections import OrderedDict as OD

import socket
from .. import fbcp

fbcp.loadCommands()

class Robot (tk.Canvas):
  def __init__(self, serial):
    self.serial = fbcp.ROBOT_PREFIX + str(serial)
