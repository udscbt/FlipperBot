#include "fbcp.common.h"

bool fbcp::common::handleRequest(fbcp::COMMAND_LINE& in, fbcp::COMMAND_LINE& out)
{
  if (in.command->code ==  fbcp::Q_LIST.code)
  {
    out.other = "";
    if (in.params["type"].compare(fbcp::LIST_OPT.str) == 0)
    {
    }
    else if (in.params["type"].compare(fbcp::LIST_CMD.str) == 0)
    {
      for (int i = 0; i < sizeof(fbcp::Q_COMMANDS)/sizeof(fbcp::COMMAND*); i++)
        out.other += (i?", ":"") + fbcp::Q_COMMANDS[i]->id;
    }
    else
    {
      return false;
    }
    out.command = &fbcp::A_DATA;
    return true;
  }
  else if (in.command->code == fbcp::Q_HELP.code)
  {
    fbcp::COMMAND_LINE command;
    fbcp::COMMAND* cmd;
    cmd = fbcp::findCommand(in.params["command"]+"\n");
    if (cmd == fbcp::NULL_COMMAND)
    {
      return false;
    }
    out.command = &fbcp::A_DATA;
    out.other = "Usage: " + cmd->id;
    for (std::vector<fbcp::string>::const_iterator it = cmd->params.begin(); it != cmd->params.end(); ++it)
    {
      out.other += " <" + *it + ">";
    }
    return true;
  }
  else
  {
    return false;
  }
}

bool fbcp::common::handleNotFound(fbcp::string in, fbcp::COMMAND_LINE& out)
{
  out.command = &fbcp::A_ERROR;
  return true;
}
