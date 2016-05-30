#include "fbcp.h"
#include "fbcp.common.h"
#include <iostream>

void debugCommand(fbcp::COMMAND_LINE& cmd)
{
  if (cmd.command == fbcp::NULL_COMMAND)
  {
    printf("NULL_COMMAND\n");
  }
  else
  {
    printf("Command: %s\n", cmd.command->id.c_str());
    printf("Params:\n");
    for (std::vector<fbcp::string>::const_iterator it = cmd.command->params.begin(); it != cmd.command->params.end(); ++it)
    {
      std::map<fbcp::string, fbcp::string>::iterator found = cmd.params.find(*it);
      printf("  [%s] = %s\n", it->c_str(), (found==cmd.params.end()?"<missing>":found->second.c_str()));
    }
    printf("Other:\n  %s\n", cmd.other.c_str());
  }
  fflush(stdout);
}

int main()
{
  setbuf(stdout, NULL);

  fbcp::COMMAND_LINE cmd;
  cmd.command = &fbcp::Q_LIST;
  cmd.params["type"] = fbcp::LIST_CMD.str;
  cmd.other = "CIAO";

  fbcp::string msg = fbcp::writeCommand(cmd);
  printf(msg);
  
  printf("---\n");
  
  if (fbcp::parseCommand(msg, cmd))
  {
    msg = fbcp::writeCommand(cmd);
    
    printf(msg);
  }
  else
  {
    printf("Can't interpret message\n");
  }
  
  fbcp::COMMAND_LINE cmd2;
  if (fbcp::common::handleRequest(cmd, cmd2))
  {
    printf("%s\n", fbcp::writeCommand(cmd2).c_str());
  }
  else
  {
    printf("Ahi ahi ahi\n");
  }
  
  debugCommand(cmd);
  debugCommand(cmd2);

  return 0;
}
