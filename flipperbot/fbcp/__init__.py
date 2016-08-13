from os.path import dirname, abspath

BOARD_PREFIX = "FlipperBot-Board-"
ROBOT_PREFIX = "FlipperBot-Robot-"
CONTR_PREFIX = "FlipperBot-Controller-"

HARD_TIMEOUT = 10000
SOFT_TIMEOUT = 1000
FAKE_TIMEOUT = 100

class Command:
  code = 0
  
  Q = []
  A = []
  list = []
  
  def __init__ (self, id=None, params=None):
    self.code = self.__class__.code
    self.__class__.code = self.__class__.code + 1
    self.id = id if id is not None else ""
    self.params = params if params is not None else []
  
  def __eq__ (self, cmd):
    if isinstance(cmd, self.__class__):
      return self.code == cmd.code
    elif isinstance(cmd, str):
      return self.id == cmd
    else:
      return False
  
  def __str__ (self):
    return self.id
  
  def __repr__ (self):
    return "<%s.%s code=%d id='%s' params=%s>" % (self.__class__.__module__, self.__class__.__name__, self.code, self.id, repr(self.params))

Command.NULL = Command()

class Param:
  code = 0
  
  STD = []
  
  def __init__ (self, id):
    self.code = self.__class__.code
    self.__class__.code = self.__class__.code + 1
    self.id = id
  
  def __eq__ (self, par):
    if isinstance(par, self.__class__):
      return self.code == par.code
    elif isinstance(par, str):
      return self.id == par
    else:
      return False
  
  def __repr__ (self):
    return "<%s.%s code=%d id='%s' >" % (self.__class__.__module__, self.__class__.__name__, self.code, self.id)


class ProtocolException (Exception):
  def __init__(self, msg="", extra=""):
    self.msg = msg
    self.extra = extra


class CommandLine:  
  def clear (self):
    self.command = Command.NULL
    self.params = {}
    self.other = ""
  
  def __init__ (self, command=None, params=None, other=None):
    self.clear()
    if command is not None:
      self.command = command
    if params is not None:
      self.params = params
    if other is not None:
      self.other = other
    
  def parse (self, msg):
    self.clear()

    if msg.endswith('\n'):
      msg = msg[:-1]
    else:
      raise ProtocolException("Missing separator", "Expected \\n at end of message")
    for cmd in Command.Q+Command.A:
      if msg.startswith(cmd.id):
        cmdlen = len(cmd.id)
        if (len(msg) > cmdlen and msg[cmdlen] != " " and msg[cmdlen] != "\n"):
          continue
        self.command = cmd
        s = msg[cmdlen:]
        params = [x for x in s.split(" ") if x != ""]
        npar = len(cmd.params)
        if len(params) < npar:
          raise ProtocolException("Missing parameter", "Expected %d parameters, got %d. In command '%s'" % (npar, len(params), cmd.id))
        for i in range(npar):
          self.params[cmd.params[i]] = params[i]
        self.other = " ".join(params[npar:])
        return True
    self.command = Command.NULL
    return False
  
  def write (self):
    if self.command == Command.NULL:
      raise ProtocolException("Nonexistent command")
    s = self.command.id
    for par in self.command.params:
      if (not par in self.params.keys()):
        raise ProtocolException("Missing parameter", "Unspecified value for parameter '%s' in command '%s'" % (par, self.command.id))
      s += " %s" % self.params[par]
    if self.other != "":
      s += " %s" % self.other
    return s+"\n"
  
  def __str__ (self):
    return self.write()
  
  def __repr__ (self):
    return "<%s.%s command='%s' params=%s other='%s'>" % (self.__class__.__module__, self.__class__.__name__, self.command, repr(self.params), self.other)

def loadCommands(filename=dirname(abspath(__file__))+"/fbcp.command.list"):
  f = open(filename, "r")
  content = f.read()
  f.close()

  for line in content.split("\n"):
    line_s = line.strip()
    if line_s.startswith("/"):
      continue
    if line_s == "":
      continue
    tokens = line.split("|")
    cmd = tokens[0].strip().replace(" ", "_")
    id  = tokens[1].strip().replace(" ", "_")
    try:
      params = tokens[2].strip().replace(" ", "_")
      params = params.split(",")
    except:
      params = []
    command = Command(id, params)
    setattr(Command, cmd, command)
    if cmd.startswith("Q_"):
      Command.Q.append(command)
      Command.list.append(command)
    elif cmd.startswith("A_"):
      Command.A.append(command)
      Command.list.append(command)
    else:
      param = Param(id)
      setattr(Param, cmd, param)
      Param.STD.append(param)
