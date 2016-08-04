#ifndef FBCP_H
#define FBCP_H

#include "fbcp.command.h"

namespace fbcp
{
  extern fbcp::string serial;
  
  const char BOARD_PREFIX[] = "FlipperBot-Board-";
  const char ROBOT_PREFIX[] = "FlipperBot-Robot-";
  const char CONTR_PREFIX[] = "FlipperBot-Controller-";
  
  const int HARD_TIMEOUT = 10000;
  const int SOFT_TIMEOUT = 1000;
  const int FAKE_TIMEOUT = 100;
  
  bool parseCommand(const fbcp::string& line, COMMAND_LINE& result);
  fbcp::string  writeCommand(COMMAND_LINE& cmd);
  COMMAND* findQCommand(const fbcp::string& line);
  COMMAND* findACommand(const fbcp::string& line);
  COMMAND* findCommand(const fbcp::string& line);
}

#endif // FBCP_H
