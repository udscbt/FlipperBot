#include "fbcp.h"
#include <iostream>

int main()
{
  fbcp::COMMAND_LINE cmd;
  cmd.command = &fbcp::Q_MOTOR_COMMAND;
  cmd.params["direction"] = fbcp::DIRECTION_FORWARD.str;
  cmd.params["motor"] = fbcp::MOTOR_LEFT.str;
  
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
  
  return 0;
}
