from threading import Thread
from subprocess import Popen, call, DEVNULL
import socket
from os.path import join, dirname, abspath

audio_root = dirname(abspath(__file__))

class AudioPlayer:
  PLAYER = "cvlc"
  PL_SONG_OPTIONS = ["-R"]
  PL_SOUND_OPTIONS = ["--play-and-exit"]
  
  def __init__(self):
    self.proc = None
    self._started = False
    self._stopped = False
    self.processes = {}
    self.PID = 0
    self.connected = False
  
  def playSong(self, song):
    song = join(audio_root, song)
    self.PID = self.PID + 1
    self.processes[self.PID] = Popen([self.PLAYER] + self.PL_SONG_OPTIONS + [song], stdout=DEVNULL, stderr=DEVNULL)
    self._started = True
    self._stopped = False
    self.clean
    return self.PID
  
  def playSound(self, sound):
    sound = join(audio_root, sound)
    self.PID = self.PID + 1
    self.processes[self.PID] = Popen([self.PLAYER] + self.PL_SOUND_OPTIONS + [sound], stdout=DEVNULL, stderr=DEVNULL)
    self._started = True
    self._stopped = False
    self.clean()
    return self.PID

  def clean(self):
    pids = [pid for pid in self.processes.keys()]
    for pid in pids:
      self.poll(pid)

  def stop(self, pid):
    try:
      self.processes[pid].kill()
      self.processes[pid].wait()
    except:
      pass
    self.proc = None
    self._stopped = True
  
  def poll(self, pid):
    try:
      self.processes[pid].poll()
    except:
      try:
        self.processes.pop(pid)
      except:
        pass
      return True
    if self.processes[pid].returncode is not None:
      self.processes.pop(pid)
      return True
    return False
  
  def connect(self, address="192.168.1.1", port=12345):
    try:
      print("Connecting to {}:{}".format(address, port))
      self.socket = socket.socket(socket.AF_INET, socket.SOCK_STREAM)
      self.socket.connect((address, port))
      self.socket.settimeout(0.5)
      self.connected = True
      self.thread = Thread(target=self.server)
      self.thread.start()
    except:
      print("Connection failed")
      self.connected = False
  
  def server(self):
    while self.connected:
      try:
        cmd = self.socket.recv(1).decode('utf-8')
        while cmd[-1] != ';':
          cmd = cmd + self.socket.recv(1).decode('utf-8')
        print("Received '{}'".format(cmd))
        cmd = cmd[:-1]
        if cmd.startswith('SONG'):
          pid = self.playSong(cmd[4:])
          self.socket.send((str(pid)+";").encode('utf-8'))
          print("Sent '{}'".format(str(pid)))
        elif cmd.startswith('SOUND'):
          pid = self.playSound(cmd[5:])
          self.socket.send((str(pid)+";").encode('utf-8'))
          print("Sent '{}'".format(str(pid)))
        elif cmd.startswith('STOP'):
          self.stop(int(cmd[4:]))
        elif cmd.startswith('POLL'):
          end = self.poll(int(cmd[4:]))
          self.socket.send(("1;" if end else "0;").encode('utf-8'))
          print("Sent '{}'".format("1" if end else "0"))
        elif cmd == '':
          self.connected = False
      except socket.timeout:
        continue
      except Exception as e:
        print(type(e))
        print(e.args)
        print(e)
        print("Broken connection")
        self.connected = False
    try:
      print("Stop")
      self.socket.close()
    except:
      pass
    pids = [pid for pid in self.processes.keys()]
    for pid in pids:
      if not self.poll(pid):
        self.processes[pid].kill()
        self.processes[pid].wait()

  
  def disconnect(self):
    self.connected = False
