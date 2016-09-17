#!/usr/bin/env python
import tkinter as tk
from flipperbot.board.audioplayer import AudioPlayer as AP
import sys

class Player (tk.Tk):
  class LogViewer (tk.Listbox):
    def __init__(self, master):
      super(self.__class__, self).__init__(master)
    
    def write(self, s):
      for line in s.split("\n"):
        if line != '':
          self.insert(tk.END, line)
      self.yview(tk.END)

    def flush(self, *args, **kwargs):
      return
  
  def __init__(self):
    super(Player, self).__init__()
    self.address = tk.Entry(self)
    self.address.insert(tk.END, '192.168.1.1')
    self.port = tk.Entry(self)
    self.port.insert(tk.END, '12345')
    self.connect = tk.Button(self, text="Connect", command=self.condiscon)
    self.logviewer = self.LogViewer(self)
    self.ap = AP()
    sys.stdout = self.logviewer

    self.rowconfigure(1, weight=1)
    self.rowconfigure(2, weight=10)
    for i in range(5):
      self.columnconfigure(i+1, weight=1+i%2)
    tk.Label(self, text="Address").grid(row=1, column=1)
    self.address.grid(row=1, column=2)
    tk.Label(self, text="Port").grid(row=1, column=3)
    self.port.grid(row=1, column=4)
    self.connect.grid(row=1, column=5)
    self.logviewer.grid(row=2, column=1, columnspan=5, sticky='nsew')
  
  def condiscon(self, *args):
    self.connect['state'] = 'disabled'
    if self.ap.connected:
      self.ap.disconnect()
      self.connect['text'] = "Connect"
      self.connect.update_idletasks()
    else:
      self.ap.connect(self.address.get(), int(self.port.get()))
      if self.ap.connected:
        self.connect['text'] = "Disconnect"
        self.connect.update_idletasks()
    self.connect['state'] = 'normal'

Player().mainloop()
