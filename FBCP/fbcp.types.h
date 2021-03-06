#ifndef FBCP_TYPES_H
#define FBCP_TYPES_H

#include "fbcp.string.h"
#include <vector>
#include <map>

namespace fbcp
{
  typedef struct
  {
    const int code;
    const fbcp::string id;
    std::vector<fbcp::string> params;
  } COMMAND_t;
  
  typedef const COMMAND_t COMMAND;
  
  typedef struct
  {
    const int code;
    fbcp::string str;
  } PARAM_t;
  
  typedef const PARAM_t PARAM;
  
  typedef struct
  {
    COMMAND* command;
    std::map<fbcp::string, fbcp::string> params;
    fbcp::string other;
  } COMMAND_LINE;
  
  
  extern COMMAND* NULL_COMMAND;
}

#endif // FBCP_TYPES_H
