import socket
from threading import Thread, Lock
from time import time, sleep, strftime
from .. import fbcp
from collections import OrderedDict as OD
from .shared import SharedVariable, ThreadEx
from .robot import Robot
from .controller import Controller

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

class ClientThread (ThreadEx):
  INDEX = 0
  def __init__(self, server, sock, address, port):
    self.server = server
    self.sockIn = sock
    self.address = address
    self.port = port
    self.index = ClientThread.INDEX
    ClientThread.INDEX = ClientThread.INDEX + 1
    self.logname = "Client #{}".format(self.index)
    self.connected = False
    super(ClientThread, self).__init__(name="client[{}]".format(self.index))

  def debug(self, *args, **kwargs):
    #print(
    #  "[{}] {}{}:{} ".format(
    #    strftime("%H:%M:%S"),
    #    termColors['BOLD'],
    #    self.logname,
    #    termColors['END']
    #  ), *args, **kwargs
    #)
    pass
  
  def remove(self):
    if self.robot:
      self.server.game.robots[self.index].active = False
      self.server.robots.pop(self.serial)
    else:
      self.server.game.controllers[self.index].active = False
      self.server.controllers.pop(self.serial)
  
  def cleanup(self):
    cmd = fbcp.CommandLine()
    cmd.command = fbcp.Command.Q_CLEAN
    s = cmd.write()
    try:
      self.sockOut.send(s.encode("UTF-8"))
      self.debug("Sent:", s)
    except socket.error:
      pass
    self.sockIn.close()
    self.sockOut.close()
  
  def setup(self):
    self.debug("Handling {}:{}".format(self.address, self.port))
    self.cmdIn = fbcp.CommandLine()
    self.cmdOut = fbcp.CommandLine()
    self.debug("Waiting for data...")
  
  def loop(self):
    if not self.connected:
      buf = self.sockIn.recv(256).decode("UTF-8")
      self.debug("Received:", buf)
      if not self.cmdIn.parse(buf):
        self.debug("Can't understand message")
        self.cmdOut.command = fbcp.Command.A_ERROR
        s = self.cmdOut.write()
        self.sockIn.send(s.encode("UTF-8"))
        self.debug("Sent:", s)
        return
      if self.cmdIn.command == fbcp.Command.Q_SINGLE_PRESENTATION:
        self.debug("Client asked for access")
        self.serial = self.cmdIn.params['serial']
        accepted = False
        if self.serial.startswith("FlipperBot-Robot-"):
          self.server.robLock.acquire()
          
          old = False
          if self.serial in self.server.robots:
            self.debug("Warning: replacing robot")
            accepted = True
            self.server.robots[self.serial]['thread'].stop()
            old = True
          
          if old or len(self.server.robots) < 2:          
            self.debug("Robot connected:", self.serial)
            accepted = True
            self.sockOut = socket.socket(socket.AF_INET, socket.SOCK_STREAM)
            self.debug("Creating connection to {}:{}".format(self.address, self.server.port))
            self.sockOut.connect((self.address, self.server.port))
            self.server.robots[self.serial] = {'thread': self, 'in': self.sockIn, 'out': self.sockOut}
            self.index = list(self.server.robots).index(self.serial)
            self.server.game.robots[self.index].active = True
            rt = RobotThread(self.server, self.sockOut, self.index)
            self.addChild(rt)
            self.robot = True
          else:
            self.debug("Too many robots")
          self.server.robLock.release()
        
        elif self.serial.startswith("FlipperBot-Controller-"):
          self.server.conLock.acquire()
          
          old = False
          if self.serial in self.server.controllers:
            self.debug("Warning: replacing controller")
            accepted = True
            self.server.controllers[self.serial]['in'].close()
            self.server.controllers[self.serial]['out'].close()
            old = True
          
          if old or len(self.server.controllers) < 2:          
            self.debug("Controller connected:", self.serial)
            accepted = True
            self.sockOut = socket.socket(socket.AF_INET, socket.SOCK_STREAM)
            #self.debug("Creating connection to {}:{}".format(self.address, self.server.port))
            #self.sockOut.connect((self.address, self.server.port))
            self.server.controllers[self.serial] = {'thread': self, 'in': self.sockIn, 'out': self.sockOut}
            self.index = list(self.server.controllers).index(self.serial)
            self.server.game.controllers[self.index].active = True
            self.robot = False
          else:
            self.debug("Too many controllers")
          self.server.conLock.release()
        else:
          self.debug("Serial not recognised:", self.serial)

        self.logname = self.serial
        self.cmdOut.command = fbcp.Command.A_GRANT_ACCESS if accepted else fbcp.Command.A_DENY_ACCESS
        s = self.cmdOut.write()
        self.sockIn.send(s.encode("UTF-8"))
        self.debug("Sent:", s)
        if accepted:
          self.connected = True
          return
    else:
      buf = ""
      while not self._stopped:
        #self.sockIn.settimeout(10)
        self.sockIn.settimeout(1000)
        try:
          c = self.sockIn.recv(1).decode("UTF-8")
        except socket.timeout:
          self.debug("Disconnected")
          self.remove()
          self.stop()
          return
        buf = buf + c
        if c == '\n':
          break
      if self._stopped:
        return
      self.debug("Received:", buf)
      if not self.cmdIn.parse(buf):
        self.debug("Can't understand message")
        self.cmdOut.command = fbcp.Command.A_ERROR
        s = self.cmdOut.write()
        self.sockIn.send(s.encode("UTF-8"))
        self.debug("Sent:", s)
        return
      
      if self.robot:
        pass
      else:
        self.cmdOut = fbcp.CommandLine()
        self.cmdIn = fbcp.CommandLine()
        if not self.cmdIn.parse(buf):
          self.debug("Can't understand message")
          self.cmdOut.command = fbcp.Command.A_ERROR
          s = self.cmdOut.write()
          self.sockIn.send(s.encode("UTF-8"))
          self.debug("Sent:", s)
          return
        elif len(self.server.robots) > self.index:
          self.cmdOut.command = fbcp.Command.A_ACCEPT
        else:
          self.debug("No related robot connected yet")
          #self.cmdOut.command = fbcp.Command.A_REFUSE
          self.cmdOut.command = fbcp.Command.A_ACCEPT
        s = self.cmdOut.write()
        self.sockIn.send(s.encode("UTF-8"))
        self.debug("Sent:", s)
        if self.cmdIn.command == fbcp.Command.Q_ROBOT_COMMAND:
          if self.cmdIn.params['direction'] == fbcp.Param.DIRECTION_BACKWARD:
            self.server.game.controllers[self.index].direction = Controller.Direction.BACKWARD
          elif self.cmdIn.params['direction'] == fbcp.Param.DIRECTION_BACKWARD_LEFT:
            self.server.game.controllers[self.index].direction = Controller.Direction.BACKWARD_LEFT
          elif self.cmdIn.params['direction'] == fbcp.Param.DIRECTION_BACKWARD_RIGHT:
            self.server.game.controllers[self.index].direction = Controller.Direction.BACKWARD_RIGHT
          elif self.cmdIn.params['direction'] == fbcp.Param.DIRECTION_FORWARD:
            self.server.game.controllers[self.index].direction = Controller.Direction.FORWARD
          elif self.cmdIn.params['direction'] == fbcp.Param.DIRECTION_FORWARD_LEFT:
            self.server.game.controllers[self.index].direction = Controller.Direction.FORWARD_LEFT
          elif self.cmdIn.params['direction'] == fbcp.Param.DIRECTION_FORWARD_RIGHT:
            self.server.game.controllers[self.index].direction = Controller.Direction.FORWARD_RIGHT
          elif self.cmdIn.params['direction'] == fbcp.Param.DIRECTION_LEFT:
            self.server.game.controllers[self.index].direction = Controller.Direction.LEFT
          elif self.cmdIn.params['direction'] == fbcp.Param.DIRECTION_RIGHT:
            self.server.game.controllers[self.index].direction = Controller.Direction.RIGHT
          elif self.cmdIn.params['direction'] == fbcp.Param.DIRECTION_STOP:
            self.server.game.controllers[self.index].direction = Controller.Direction.STOP

class RobotThread (ThreadEx):
  def __init__(self, server, sock, index):
    self.server = server
    self.sockOut = sock
    self.index = index
    self.direction = self.server.game.robots[index].direction
    super(RobotThread, self).__init__(name="client[robot{}]".format(self.index))
  
  def loop(self):
    d = self.server.game.robots[index].direction
    if d != self.direction:
      cmd = fbcp.CommandLine()
      cmd.command = fbcp.Command.Q_ROBOT_COMMAND
      if d == Robot.Direction.BACKWARD:
        cmd.params['direction'] = fbcp.Param.DIRECTION_BACKWARD
      elif d == Robot.Direction.BACKWARD_LEFT:
        cmd.params['direction'] = fbcp.Param.DIRECTION_BACKWARD_LEFT
      elif d == Robot.Direction.BACKWARD_RIGHT:
        cmd.params['direction'] = fbcp.Param.DIRECTION_BACKWARD_RIGHT
      elif d == Robot.Direction.FORWARD:
        cmd.params['direction'] = fbcp.Param.DIRECTION_FORWARD
      elif d == Robot.Direction.FORWARD_LEFT:
        cmd.params['direction'] = fbcp.Param.DIRECTION_FORWARD_LEFT
      elif d == Robot.Direction.FORWARD_RIGHT:
        cmd.params['direction'] = fbcp.Param.DIRECTION_FORWARD_RIGHT
      elif d == Robot.Direction.LEFT:
        cmd.params['direction'] = fbcp.Param.DIRECTION_LEFT
      elif d == Robot.Direction.RIGHT:
        cmd.params['direction'] = fbcp.Param.DIRECTION_RIGHT
      elif d == Robot.Direction.STOP:
        cmd.params['direction'] = fbcp.Param.DIRECTION_STOP
      self.server.game.robots[index].direction = d
      sock.send(cmd.write().encode("UTF-8"))
  
  
class Server (ThreadEx):
  def __init__(self, game):
    self.port = 10000
    self.game = game
    self.robots = OD()
    self.controllers = OD()
    self.robLock = Lock()
    self.conLock = Lock()
    super(Server, self).__init__(name="server")
  
  def setup(self):
    self.serversocket = socket.socket(socket.AF_INET, socket.SOCK_STREAM)
    self.serversocket.bind(("192.168.1.1", self.port))
    self.serversocket.listen(10)
    self.serversocket.settimeout(0.5)
    print("Server started on 192.168.1.1:{}".format(self.port))
  
  def loop(self):
    try:
      sock, address = self.serversocket.accept()
    except socket.timeout:
        return
    print("New connection: {}".format(address))
    ct = ClientThread(self, sock, address[0], address[1])
    self.addChild(ct)
    ct.start()
  
  def cleanup(self):
    self.serversocket.close()
    
