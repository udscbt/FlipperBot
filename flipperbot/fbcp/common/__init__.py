from .. import *

def handleRequest(req):
  out = CommandLine()
  if req.command == Command.Q_LIST:
    if req.params["type"] == Param.LIST_OPT:
      other = ""
    elif req.params["type"] == Param.LIST_CMD:
      other = ", ".join(map(lambda cmd: cmd.id, Command.Q))
    else:
      return CommandLine()
    return CommandLine(Command.A_DATA, {}, other)
  elif req.command == Command.Q_HELP:
    for command in Command.list:
      if command == req.params['command']:
        return CommandLine(Command.A_DATA, {}, ("Usage: %s" % command.id) + (" " if len(command.params) > 0 else "") + (" ".join(["<%s>" % p for p in command.params])))
    else:
      return CommandLine()
  elif req.command == Command.Q_HEARTBEAT:
    return CommandLine(Command.A_HEARTBEAT)
  else:
    return CommandLine()

def handleNotFound(req):
  return CommandLine(Command.A_ERROR)
