#include "fbcp.h"

#include <iostream>

fbcp::string fbcp::writeCommand(COMMAND_LINE& cmdline)
{
  COMMAND* cmd = cmdline.command;
  
  if (cmd == NULL_COMMAND)
    return "";
  
  fbcp::string result(cmd->id);
  for (int i = 0; i < cmd->params.size(); i++)
  {
    std::map<fbcp::string,fbcp::string>::iterator it = cmdline.params.find(cmd->params[i]);
    if (it == cmdline.params.end())
      return "";
    result += " ";
    result += it->second;
  }
  if (!cmdline.other.empty())
  {
    result += " " + cmdline.other;
  }
  result += "\n";
  return result;
}

bool fbcp::parseCommand(const fbcp::string& line, COMMAND_LINE& result)
{
  result.command = findCommand(line);
  
  if (result.command == NULL_COMMAND)
    return false;
  
  size_t pos_old ;
  size_t pos_new = result.command->id.length();
  for (std::vector<fbcp::string>::const_iterator it = result.command->params.begin(); it != result.command->params.end(); ++it)
  {
    pos_old = pos_new+1;
    pos_new = line.find(" ", pos_old);
    if (pos_new == fbcp::string::npos)
    {
      pos_new = line.find("\n", pos_old);
      if (pos_new == fbcp::string::npos)
      {
        return false;
      }
    }
    if (pos_new == pos_old)
      return false;
    
    result.params[*it] = line.substr(pos_old, pos_new-pos_old);
  }
  if (line[pos_new] != '\n')
  {
    result.other = line.substr(pos_new+1);
  }
  return true;
}

bool startsWith(const fbcp::string big, const fbcp::string small)
{
  fbcp::string::const_iterator it1, it2;
  for (it1 = big.begin(), it2 = small.begin(); it1 != big.end() && it2 != small.end() && *it1 == *it2; ++it1, ++it2);
  if (it2 == small.end() && (*it1 == ' ' || *it1 == '\n'))
    return true;
  return false;
}

fbcp::COMMAND* fbcp::findQCommand(const fbcp::string& line)
{
  for (int i=0; i < sizeof(Q_COMMANDS)/sizeof(COMMAND*); i++)
  {
    if (startsWith(line, Q_COMMANDS[i]->id))
      return Q_COMMANDS[i];
  }
  return NULL_COMMAND;
}

fbcp::COMMAND* fbcp::findACommand(const fbcp::string& line)
{
  for (int i=0; i < sizeof(A_COMMANDS)/sizeof(COMMAND*); i++)
  {
    if (startsWith(line, A_COMMANDS[i]->id))
      return A_COMMANDS[i];
  }
  return NULL_COMMAND;
}

fbcp::COMMAND* fbcp::findCommand(const fbcp::string& line)
{
  COMMAND* result = findQCommand(line);
  if (result == NULL_COMMAND)
    result = findACommand(line);
  return result;
}
