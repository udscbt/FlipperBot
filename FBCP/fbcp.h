#ifndef FBCP_H
#define FBCP_H

#include "fbcp.command.h"

namespace fbcp
{
  extern const std::string SERIAL;
  extern COMMAND* Q_COMMANDS[];
  extern COMMAND* A_COMMANDS[];
  extern PARAM* STD_PARAMS[];
  
  bool parseCommand(const std::string line, COMMAND_LINE& result);
  std::string  writeCommand(COMMAND_LINE& cmd);
  COMMAND* findQCommand(const std::string line);
  COMMAND* findACommand(const std::string line);
  COMMAND* findCommand(const std::string line);
}

#endif // FBCP_H
