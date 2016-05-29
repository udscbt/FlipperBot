#include "fbcp.h"
#include "fbcp.common.h"
#include <iostream>

void debugCommand(fbcp::COMMAND_LINE& cmd)
{
  printf("Command: %s\n", &(cmd.command->id[0]));
  printf("Params:\n");
  for (std::vector<fbcp::string>::const_iterator it = cmd.command->params.begin(); it != cmd.command->params.end(); ++it)
  {
    std::map<fbcp::string, fbcp::string>::iterator found = cmd.params.find(*it);
    printf("  [%s] = %s\n", &((*it)[0]), (found==cmd.params.end()?"<missing>":&(found->second[0])));
  }
  printf("Other:\n  %s\n", &(cmd.other[0]));
  fflush(stdout);
}

int main()
{
  fbcp::COMMAND_LINE cmd;
  cmd.command = &fbcp::Q_LIST;
  cmd.params["type"] = fbcp::LIST_CMD.str;
  
  fbcp::string msg = fbcp::writeCommand(cmd);
  printf(&msg[0]);
  
  printf("---\n");
  
  if (fbcp::parseCommand(msg, cmd))
  {
    msg = fbcp::writeCommand(cmd);
    
    printf(&msg[0]);
  }
  else
  {
    printf("Can't interpret message\n");
  }
  
  fbcp::COMMAND_LINE cmd2;
  if (fbcp::common::handleRequest(cmd, cmd2))
  {
    printf("%s\n", &fbcp::writeCommand(cmd2)[0]);
  }
  else
  {
    printf("Ahi ahi ahi\n");
  }
  
  debugCommand(cmd);
  debugCommand(cmd2);
  
  /*fbcp::string msg;
  while (true)
  {
    fbcp::COMMAND_LINE cmd_q, cmd_a;
    std::cout << "> ";
    std::getline(std::cin, msg);
    fbcp::parseCommand(msg+"\n", cmd_q);
    if (cmd_q.command != fbcp::NULL_COMMAND)
    {
      debugCommand(cmd_q);
    }
    if (cmd_q.command == fbcp::NULL_COMMAND or !fbcp::common::handleRequest(cmd_q, cmd_a))
    {
      fbcp::common::handleNotFound(msg, cmd_a);
    }
    std::cout << fbcp::writeCommand(cmd_a);
    debugCommand(cmd_a);
  }*/

  return 0;
}
