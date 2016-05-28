#include "fbcp.h"
#include "fbcp.common.h"
#include <iostream>

void debugCommand(fbcp::COMMAND_LINE& cmd)
{
  std::cout << "Command: " << cmd.command->id << "\n";
  std::cout << "Params:\n";
  for (std::vector<std::string>::const_iterator it = cmd.command->params.begin(); it != cmd.command->params.end(); ++it)
  {
    std::map<std::string, std::string>::iterator found = cmd.params.find(*it);
    std::cout << "  [" << *it << "] = " << (found==cmd.params.end()?"<missing>":found->second) << "\n";
  }
  std::cout << "Other:\n  " << cmd.other << std::endl;
}

int main()
{
  /*
  fbcp::COMMAND_LINE cmd;
  cmd.command = &fbcp::Q_LIST;
  cmd.params["type"] = fbcp::LIST_CMD.str;
  
  std::string msg = fbcp::writeCommand(cmd);
  std::cout << msg;
  
  std::cout << "---" << std::endl;
  
  if (fbcp::parseCommand(msg, cmd))
  {
    msg = fbcp::writeCommand(cmd);
    
    std::cout << msg;
  }
  else
  {
    std::cout << "Can't interpret message" << std::endl;
  }
  
  fbcp::COMMAND_LINE cmd2;
  if (fbcp::common::handleRequest(cmd, cmd2))
  {
    std::cout << fbcp::writeCommand(cmd2) << std::endl;
  }
  else
  {
    std::cout << "Ahi ahi ahi" << std::endl;
  }
  
  debugCommand(cmd);
  debugCommand(cmd2);
  */
  std::string msg;
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
  }

  return 0;
}
