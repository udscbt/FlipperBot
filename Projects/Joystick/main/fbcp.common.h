/*************************************
 * Auto-generated ScheMo source file *
 *************************************
 * Original file name: main/fbcp.common.h
 * New file name: ./main/fbcp.common.h
 * Identified jobs: 0
 *   Named: 0
 *   Unnamed: 0
 * Identified tasks: 0
 *   Named: 0
 *   Unnamed: 0
 */

#ifndef FBCP_COMMON_H
#define FBCP_COMMON_H

#include "fbcp.h"

namespace fbcp
{
  namespace common
  {
    bool handleRequest(fbcp::COMMAND_LINE& in, fbcp::COMMAND_LINE& out);
    bool handleNotFound(fbcp::string in, fbcp::COMMAND_LINE& out);
  }
}

#endif 