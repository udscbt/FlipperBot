#!/usr/bin/env python
import sys

ext_in = ".command.list"
ext_out = ".command.h"

for arg in sys.argv[1:]:
  if not arg.endswith(ext_in):
    print("Ignoring '%s'" % arg)
    print("Only '%s' files can be processed" % ext_in)
    print()
    continue
  print("Processing '%s'" % arg)
  f = open(arg, "r")
  content = f.read()
  f.close()

  Q_COMMANDS = []
  A_COMMANDS = []
  PARAMS     = []

  output = ""
  for line in content.split("\n"):
    line_s = line.strip()
    output = output + "  "
    if line_s.startswith("/"):
      output = output + line + "\n"
      continue
    if line_s == "":
      output = output + line + "\n"
      continue
    tokens = line.split("|")
    cmd = tokens[0].strip()
    id  = '"'+tokens[1].strip()+'"'
    try:
      params = tokens[2].strip()
      params = '"'+'","'.join(params.split(","))+'"'
    except:
      params = ""
    params = "{%s}" % params
    if cmd.startswith("Q_"):
      Q_COMMANDS.append(cmd)
    elif cmd.startswith("A_"):
      A_COMMANDS.append(cmd)
    else:
      PARAMS.append(cmd)
      output = output + "PARAM %s = {%s};\n" % (cmd, id)
      continue
    output = output + "COMMAND %s = {%s, %s};\n" % (cmd, id, params)

  output = output + "  COMMAND* Q_COMMANDS[] = {\n    &" + ",\n    &".join(Q_COMMANDS) + "\n  };\n"
  output = output + "  COMMAND* A_COMMANDS[] = {\n    &" + ",\n    &".join(A_COMMANDS) + "\n  };\n"
  output = output + "  PARAM* STD_PARAMS[] = {\n    &" + ",\n    &".join(PARAMS) + "\n  };\n"

  f = open("fbcp.template.command.h", "r")
  before = f.read()
  f.close()
  name = arg.split("/")[-1]
  name = name.replace(ext_in, ext_out)
  f = open(name, "w")
  name = name.upper().replace(".", "_")
  f.write(before % (name, name, output, name))
  f.close()
