#ifndef FBCP_COMMON_H
#define FBCP_COMMON_H

#include "fbcp.h"

namespace fbcp
{
  namespace common
  {
    bool handleRequest(fbcp::COMMAND_LINE& in, fbcp::COMMAND_LINE& out);
    bool handleNotFound(std::string in, fbcp::COMMAND_LINE& out);
  }
}

#endif // FBCP_COMMON_H
