#ifndef FBCP_TYPES_H
#define FBCP_TYPES_H

#include <vector>
#include <string>
#include <map>

namespace fbcp
{
  typedef struct
  {
    const int code;
    const std::string id;
    std::vector<std::string> params;
  } COMMAND_t;
  
  typedef const COMMAND_t COMMAND;
  
  typedef struct
  {
    const int code;
    std::string str;
  } PARAM_t;
  
  typedef const PARAM_t PARAM;
  
  typedef struct
  {
    COMMAND* command;
    std::map<std::string, std::string> params;
    std::string other;
  } COMMAND_LINE;
  
  
  extern COMMAND* NULL_COMMAND;
}

#endif // FBCP_TYPES_H
