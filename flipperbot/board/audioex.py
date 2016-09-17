from os.path import join, dirname, basename
from threading import Thread
import socket
from . import audio


class Audio (audio.Audio):
  address = '0.0.0.0'
  port = 12345
  
  def __init__(self, logging=True, debug=None):
    self.thread = Thread(target=self.server)
    self.connected = False
    self._hard_stop = False
    super(Audio, self).__init__(logging, debug)
    self.thread.start()
  
  def server(self):
    self.serversocket = socket.socket(socket.AF_INET, socket.SOCK_STREAM)
    self.serversocket.bind((self.address, self.port))
    self.serversocket.listen(1)
    self.serversocket.settimeout(0.5)
    self.debug("Server started on {}:{}".format(self.address, self.port))
    self.debug("Waiting for external player to connect")
    while not self._hard_stop:
      try:
        self.sock, address = self.serversocket.accept()
      except socket.timeout:
          continue
      self.debug("Player connected: {}".format(address))
      self.connected = True
      while not self._hard_stop and self.connected:
        pass
      if self._hard_stop:
        self.debug("Server stopped")
      elif not self.connected:
        self.debug("Player disconnected")
      else:
        self.debug("WHAT?!")
      try:
        self.connected = False
        self.sock.close()
      except:
        pass
    try:
      self.serversocket.close()
    except:
      pass
  
  def hardStop(self):
    self._hard_stop = True
  
  def start(self, song):
    song = join(basename(dirname(song)), basename(song))
    self.stop()
    self.debug("Playing song '{}'".format(song))
    if self.connected:
      try:
        self.sock.send(("SONG"+song).encode('utf-8'))
        self.proc = self.sock.recv(10).decode('utf-8')
      except Exception as e:
        self.debug(type(e), tags=["Exception"])
        self.debug(e.args, tags=["Exception"])
        self.debug(e, tags=["Exception"])
        self.connected = False
    if not self.connected:
      self.debug("Player not connected... faking it")
      self.proc = None
    self._started = True
    self._stopped = False

  def stop(self):
    self.debug("Stopping last song")
    if self.proc is None:
      self.debug("Last song was already stopped")
    else:
      if self.connected:
        try:
          self.sock.send(("STOP"+self.proc).encode('utf-8'))
        except Exception as e:
          self.debug(type(e), tags=["Exception"])
          self.debug(e.args, tags=["Exception"])
          self.debug(e, tags=["Exception"])
          self.connected = False
      if not self.connected:
        self.debug("Player not connected... faking it")
    self.proc = None
    self._stopped = True

class SoundEffect (audio.SoundEffect):
  root = None
  
  def start(self):
    self.sound = join(basename(dirname(self.sound)), basename(self.sound))
    self.stop()
    self.debug("Playing sound effect '{}'".format(self.sound))
    if self.root is not None and self.root.connected:
      try:
        self.root.sock.send(("SOUND"+self.sound).encode('utf-8'))
        self.proc = self.root.sock.recv(10).decode('utf-8')
      except Exception as e:
        self.debug(type(e), tags=["Exception"])
        self.debug(e.args, tags=["Exception"])
        self.debug(e, tags=["Exception"])
        self.root.connected = False
        self.debug("Player not connected... faking it")
        self.proc = None
    else:
      self.debug("Player not connected... faking it")
      self.proc = None
    self._started = True
    self._stopped = False
    self.running.append(self)
    SoundEffect.clean()
  
  def ended(self):
    if self.proc is None:
      self.debug("Sound already stopped")
      return True
    if self.root is not None and self.root.connected:
      try:
        self.root.sock.send(("POLL"+self.proc).encode('utf-8'))
        return bool(int(self.root.sock.recv(10).decode('utf-8')))
      except Exception as e:
        self.debug(type(e), tags=["Exception"])
        self.debug(e.args, tags=["Exception"])
        self.debug(e, tags=["Exception"])
        self.root.connected = False
        self.debug("Player not connected... faking it")
        return True
    else:
      self.debug("Player not connected... faking it")
      return True
  
  def wait(self):
    self.debug("Waiting for sound effect to terminate")
    if self.proc is None:
      self.debug("Sound effect was already stopped")
    elif self.root is None or not self.root.connected:
      self.debug("Player not connected... faking it")
    while not self.ended():
      pass
    self.proc = None
    SoundEffect.clean()
  
  def stop(self):
    self.debug("Stopping sound effect")
    if self.proc is None:
      self.debug("Sound effect was already stopped")
    elif self.root is not None and self.root.connected:
      try:
        self.root.sock.send(("STOP"+self.proc).encode('utf-8'))
      except Exception as e:
        self.debug(type(e), tags=["Exception"])
        self.debug(e.args, tags=["Exception"])
        self.debug(e, tags=["Exception"])
        self.root.connected = False
        self.debug("Player not connected... faking it")
    else:
      self.debug("Player not connected... faking it")
    self.proc = None
    self._stopped = True
    SoundEffect.clean()
