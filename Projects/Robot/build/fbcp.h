/*************************************
 * Auto-generated ScheMo source file *
 *************************************
 * Original file name: main/fbcp.h
 * New file name: ./main/fbcp.h
 * Identified jobs: 0
 *   Named: 0
 *   Unnamed: 0
 * Identified tasks: 0
 *   Named: 0
 *   Unnamed: 0
 */

#ifndef FBCP_H
#define FBCP_H

#include "fbcp.command.h"

namespace fbcp
{
  extern fbcp::string serial;
  
  const char BOARD_PREFIX[] = "FlipperBot-Board-";
  const char ROBOT_PREFIX[] = "FlipperBot-Robot-";
  const char CONTR_PREFIX[] = "FlipperBot-Controller-";
  
  bool parseCommand(const fbcp::string& line, COMMAND_LINE& result);
  fbcp::string  writeCommand(COMMAND_LINE& cmd);
  COMMAND* findQCommand(const fbcp::string& line);
  COMMAND* findACommand(const fbcp::string& line);
  COMMAND* findCommand(const fbcp::string& line);
}

#endif 
