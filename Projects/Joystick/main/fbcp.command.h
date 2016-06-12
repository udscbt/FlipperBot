/*************************************
 * Auto-generated ScheMo source file *
 *************************************
 * Original file name: main/fbcp.command.h
 * New file name: ./main/fbcp.command.h
 * Identified jobs: 0
 *   Named: 0
 *   Unnamed: 0
 * Identified tasks: 0
 *   Named: 0
 *   Unnamed: 0
 */

#ifndef FBCP_COMMAND_H
#define FBCP_COMMAND_H

#include "fbcp.types.h"

namespace fbcp
{
  extern COMMAND Q_RAW_COMMAND;
  
  extern COMMAND Q_MODE_SELECT;
  extern COMMAND A_MODE_ACCEPT;
  extern COMMAND A_MODE_DENY;
  
  extern COMMAND Q_OPTION;
  extern PARAM OPTION_SET;
  extern PARAM OPTION_UNSET;
  extern COMMAND A_OPTION_ACCEPT;
  extern COMMAND A_OPTION_DENY;
  extern COMMAND A_ACCEPT;
  extern COMMAND A_REFUSE;
  extern COMMAND A_ERROR;
  extern COMMAND A_ALIKE;
  extern COMMAND Q_LIST;
  extern PARAM LIST_OPT;
  extern PARAM LIST_CMD;
  extern COMMAND A_DATA;
  
  extern COMMAND Q_HELP;
  extern COMMAND Q_LOG;
  extern COMMAND Q_SINGLE_PRESENTATION;
  extern COMMAND Q_MULTI_PRESENTATION;
  
  extern COMMAND A_GRANT_ACCESS;
  extern COMMAND A_DENY_ACCESS;
  
  extern COMMAND Q_HEARTBEAT;
  extern COMMAND A_HEARTBEAT;
  
  extern COMMAND Q_CLEAN;
  extern COMMAND A_CLEAN;
  
  extern COMMAND A_REQUEST_PRESENTATION;
  
  extern COMMAND Q_CHANGE_NET;
  extern COMMAND A_CHANGE_ACCEPT;
  extern COMMAND A_CHANGE_DENY;
  extern COMMAND Q_MOTOR_COMMAND;
  extern PARAM MOTOR_LEFT;
  extern PARAM MOTOR_RIGHT;
  extern PARAM MOTOR_BOTH;
  extern PARAM DIRECTION_FORWARD;
  extern PARAM DIRECTION_BACKWARD;
  extern PARAM DIRECTION_STOP;
  
  extern COMMAND Q_ROBOT_COMMAND;
    
  extern PARAM DIRECTION_FORWARD_LEFT;
  extern PARAM DIRECTION_FORWARD_RIGHT;
  extern PARAM DIRECTION_BACKWARD_LEFT;
  extern PARAM DIRECTION_BACKWARD_RIGHT;
  extern PARAM DIRECTION_LEFT;
  extern PARAM DIRECTION_RIGHT;
  
  extern COMMAND Q_HIT;
  
  extern COMMAND* Q_COMMANDS[14];
  extern COMMAND* A_COMMANDS[16];
  extern PARAM* STD_PARAMS[16];

}

#endif 