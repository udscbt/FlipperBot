import socket
from threading import Thread, Lock
from time import time, sleep, strftime
from .. import fbcp
from collections import OrderedDict as OD

termColors = {
    'PURPLE' : '\033[95m',
    'BLUE' : '\033[94m',
    'GREEN' : '\033[92m',
    'YELLOW' : '\033[93m',
    'RED' : '\033[91m',
    'END' : '\033[0m',
    'BOLD' : '\033[1m',
    'UNDERLINE' : '\033[4m'
 }

fbcp.loadCommands()

class ClientThread (Thread):
  INDEX = 0
  def __init__(self, server, sock, address, port):
    self.server = server
    self.sockIn = sock
    self.address = address
    self.port = port
    self.index = ClientThread.INDEX
    ClientThread.INDEX = ClientThread.INDEX + 1
    self.logname = "Client #{}".format(self.index)
    super(ClientThread, self).__init__()

  def debug(self, *args, **kwargs):
    print(
      "[{}] {}{}:{} ".format(
        strftime("%H:%M:%S"),
        termColors['BOLD'],
        self.logname,
        termColors['END']
      ), *args, **kwargs
    )
  
  def stop(self):
    cmd = fbcp.CommandLine()
    cmd.command = fbcp.Command.Q_CLEAN
    s = cmd.write()
    self.sockOut.send(s.encode("UTF-8"))
    self.debug("Sent:", s)
    self.sockIn.close()
    self.sockOut.close()
    self.debug("Thread stopped")
  
  def run(self):
    self.debug("Handling {}:{}".format(self.address, self.port))
    cmdIn = fbcp.CommandLine()
    cmdOut = fbcp.CommandLine()
    self.debug("Waiting for data...")
    buf = self.sockIn.recv(256).decode("UTF-8")
    self.debug("Received:", buf)
    if not cmdIn.parse(buf):
      self.debug("Can't understand message")
      cmdOut.command = fbcp.Command.A_ERROR
      s = cmdOut.write()
      self.sockIn.send(s.encode("UTF-8"))
      self.debug("Sent:", s)
      self.stop()
      return
    if cmdIn.command == fbcp.Command.Q_SINGLE_PRESENTATION:
      self.debug("Client asked for access")
      serial = cmdIn.params['serial']
      accepted = False
      if serial.startswith("FlipperBot-Robot-"):
        self.server.robLock.acquire()
        if len(self.server.robots) < 2:          
          self.debug("Robot connected:", serial)
          accepted = True
          self.sockOut = socket.socket(socket.AF_INET, socket.SOCK_STREAM)
          self.debug("Creating connection to {}:{}".format(self.address, self.server.port))
          self.sockOut.connect((self.address, self.server.port))
          self.server.robots[serial] = {'in': self.sockIn, 'out': self.sockOut}
          robot = True
        else:
          self.debug("Too many robots")
        self.server.robLock.release()
      elif serial.startswith("FlipperBot-Controller-"):
        self.server.conLock.acquire()
        if len(self.server.robots) < 2:          
          self.debug("Controller connected:", serial)
          accepted = True
          self.sockOut = socket.socket(socket.AF_INET, socket.SOCK_STREAM)
          #self.debug("Creating connection to {}:{}".format(self.address, self.server.port))
          #self.sockOut.connect((self.address, self.server.port))
          self.server.controllers[serial] = {'in': self.sockIn, 'out': self.sockOut}
          robot = False
        else:
          self.debug("Too many controllers")
        self.server.conLock.release()
      else:
        self.debug("Serial not recognised:", serial)

      self.logname = serial
      cmdOut.command = fbcp.Command.A_GRANT_ACCESS if accepted else fbcp.Command.A_DENY_ACCESS
      s = cmdOut.write()
      self.sockIn.send(s.encode("UTF-8"))
      self.debug("Sent:", s)
      if not accepted:
        self.stop()
        return
    
    while not self.server._stopped:
      buf = ""
      while not self.server._stopped:
        self.sockIn.settimeout(10)
        try:
          c = self.sockIn.recv(1).decode("UTF-8")
        except socket.timeout:
          self.debug("Disconnected")
          self.stop()
          return
        buf = buf + c
        if c == '\n':
          break
      if self.server._stopped:
        self.stop()
        return
      self.debug("Received:", buf)
      if not cmdIn.parse(buf):
        self.debug("Can't understand message")
        cmdOut.command = fbcp.Command.A_ERROR
        s = cmdOut.write()
        self.sockIn.send(s.encode("UTF-8"))
        self.debug("Sent:", s)
        continue
      
      if robot:
        pass
      else:
        index = list(self.server.controllers).index(serial)
        cmdOut = fbcp.CommandLine()
        if len(self.server.robots) > index:
          list(self.server.robots)[index]['out'].send(buf.encode("UTF-8"))
          cmdOut.command = fbcp.Command.A_ACCEPT
        else:
          self.debug("No related robot connected yet")
          cmdOut.command = fbcp.Command.A_REFUSE

        s = cmdOut.write()
        self.sockIn.send(s.encode("UTF-8"))
        self.debug("Sent:", s)
    
    self.stop()

class Server (Thread):
  def __init__(self, port):
    self.port = port
    self.robots = OD()
    self.controllers = OD()
    self.robLock = Lock()
    self.conLock = Lock()
    self._stopped = False
    super(Server, self).__init__()
  
  def stop(self):
    self._stopped = True
  
  def run(self):
    serversocket = socket.socket(socket.AF_INET, socket.SOCK_STREAM)
    serversocket.bind(("0.0.0.0", self.port))
    serversocket.listen(10)
    serversocket.settimeout(0.5)
    print("Server started on 0.0.0.0:{}".format(self.port))
    while not self._stopped:
      try:
        sock, address = serversocket.accept()
      except socket.timeout:
          continue
      print("New connection: {}".format(address))
      ct = ClientThread(self, sock, address[0], address[1])
      ct.start()
    print("Server stopped")
    serversocket.close()
    
