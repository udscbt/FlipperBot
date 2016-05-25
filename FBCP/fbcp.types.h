#ifndef FBCP_TYPES_H
#define FBCP_TYPES_H

#include <vector>
#include <string>
#include <map>

namespace fbcp
{
  typedef const struct
  {
    const std::string id;
    std::vector<std::string> params;
  } COMMAND;
  
  typedef const struct
  {
    std::string str;
  } PARAM;
  
  typedef struct
  {
    COMMAND* command;
    std::map<std::string, std::string> params;
  } COMMAND_LINE;
  COMMAND* NULL_COMMAND = (COMMAND*) 0;
}

#endif // FBCP_TYPES_H
