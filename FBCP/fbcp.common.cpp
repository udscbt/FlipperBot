#include "fbcp.common.h"

/*
 * Write makefile
 * Add anonymous parameters
 */

bool fbcp::common::handleRequest(COMMAND_LINE& in, COMMAND_LINE& out)
{
  switch (in.command)
  {
    case fbcp::Q_LIST:
      
      return true;
    default:
      return false;
  }
}

bool fbcp::common::handleNotFound(std::string in, COMMAND_LINE& out)
{
  out.command = fbcp::A_ERROR;
  return true;
}
