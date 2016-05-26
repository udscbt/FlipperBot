#ifndef FBCP_COMMON_H
#define FBCP_COMMON_H

#include "fbcp.h"

namespace fbcp
{
  namespace common
  {
    bool handleRequest(COMMAND_LINE& in, COMMAND_LINE& out);
    void handleNotFound(COMMAND_LINE& in, COMMAND_LINE& out);
  }
}

#endif // FBCP_COMMON_H
