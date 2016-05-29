#ifndef FBCP_H
#define FBCP_H

#include "fbcp.command.h"

namespace fbcp
{
  extern const fbcp::string SERIAL;
  
  bool parseCommand(const fbcp::string& line, COMMAND_LINE& result);
  fbcp::string  writeCommand(COMMAND_LINE& cmd);
  COMMAND* findQCommand(const fbcp::string& line);
  COMMAND* findACommand(const fbcp::string& line);
  COMMAND* findCommand(const fbcp::string& line);
}

#endif // FBCP_H