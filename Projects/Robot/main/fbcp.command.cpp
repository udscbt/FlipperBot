/*************************************
 * Auto-generated ScheMo source file *
 *************************************
 * Original file name: main/fbcp.command.cpp
 * New file name: ./main/fbcp.command.cpp
 * Identified jobs: 0
 *   Named: 0
 *   Unnamed: 0
 * Identified tasks: 0
 *   Named: 0
 *   Unnamed: 0
 */

#include "fbcp.command.h"

namespace fbcp
{
  COMMAND Q_RAW_COMMAND = {1, "RAW", {}};
  
  COMMAND Q_MODE_SELECT = {2, "MODE", {"mode"}};
  COMMAND A_MODE_ACCEPT = {3, "OK", {}};
  COMMAND A_MODE_DENY = {4, "DUNNO", {}};
  
  COMMAND Q_OPTION = {5, "OPT", {"option","value"}};
  PARAM OPTION_SET = {6, "ON"};
  PARAM OPTION_UNSET = {7, "OFF"};
  COMMAND A_OPTION_ACCEPT = {8, "OK", {}};
  COMMAND A_OPTION_DENY = {9, "DUNNO", {}};
  COMMAND A_ACCEPT = {10, "ON_IT!", {}};
  COMMAND A_REFUSE = {11, "NOT_NOW", {}};
  COMMAND A_ERROR = {12, "WHAT?", {}};
  COMMAND A_ALIKE = {13, "MAYBE", {"command"}};
  COMMAND Q_LIST = {14, "LIST", {"type"}};
  PARAM LIST_OPT = {15, "OPT"};
  PARAM LIST_CMD = {16, "CMD"};
  COMMAND A_DATA = {17, "DATA", {}};
  
  COMMAND Q_HELP = {18, "EXPLAIN", {"command"}};
  COMMAND Q_LOG = {19, "LOG", {}};
  COMMAND Q_SINGLE_PRESENTATION = {20, "I'M", {"serial"}};
  COMMAND Q_MULTI_PRESENTATION = {21, "WE'RE", {"serial","friend"}};
  
  COMMAND A_GRANT_ACCESS = {22, "WELCOME", {}};
  COMMAND A_DENY_ACCESS = {23, "BUSY", {}};
  
  COMMAND Q_HEARTBEAT = {24, "STILL_THERE?", {}};
  COMMAND A_HEARTBEAT = {25, "YEP", {}};
  
  COMMAND Q_CLEAN = {26, "GOODBYE", {}};
  COMMAND A_CLEAN = {27, "BYE", {}};
  
  COMMAND A_REQUEST_PRESENTATION = {28, "WHO?", {}};
  
  COMMAND Q_CHANGE_NET = {29, "MOVE_TO", {"net"}};
  COMMAND A_CHANGE_ACCEPT = {30, "OK", {}};
  COMMAND A_CHANGE_DENY = {31, "NO", {}};
  COMMAND Q_MOTOR_COMMAND = {32, "MOTOR", {"motor","direction"}};
  PARAM MOTOR_LEFT = {33, "ML"};
  PARAM MOTOR_RIGHT = {34, "MR"};
  PARAM MOTOR_BOTH = {35, "MLR"};
  PARAM DIRECTION_FORWARD = {36, "FW"};
  PARAM DIRECTION_BACKWARD = {37, "BW"};
  PARAM DIRECTION_STOP = {38, "STOP"};
  
  COMMAND Q_ROBOT_COMMAND = {39, "BOT", {"direction"}};
    
  PARAM DIRECTION_FORWARD_LEFT = {40, "FL"};
  PARAM DIRECTION_FORWARD_RIGHT = {41, "FR"};
  PARAM DIRECTION_BACKWARD_LEFT = {42, "BL"};
  PARAM DIRECTION_BACKWARD_RIGHT = {43, "BR"};
  PARAM DIRECTION_LEFT = {44, "SL"};
  PARAM DIRECTION_RIGHT = {45, "SR"};
  
  COMMAND Q_HIT = {46, "OUCH", {}};
  
  COMMAND* Q_COMMANDS[] = {
    &Q_RAW_COMMAND,
    &Q_MODE_SELECT,
    &Q_OPTION,
    &Q_LIST,
    &Q_HELP,
    &Q_LOG,
    &Q_SINGLE_PRESENTATION,
    &Q_MULTI_PRESENTATION,
    &Q_HEARTBEAT,
    &Q_CLEAN,
    &Q_CHANGE_NET,
    &Q_MOTOR_COMMAND,
    &Q_ROBOT_COMMAND,
    &Q_HIT
  };
  COMMAND* A_COMMANDS[] = {
    &A_MODE_ACCEPT,
    &A_MODE_DENY,
    &A_OPTION_ACCEPT,
    &A_OPTION_DENY,
    &A_ACCEPT,
    &A_REFUSE,
    &A_ERROR,
    &A_ALIKE,
    &A_DATA,
    &A_GRANT_ACCESS,
    &A_DENY_ACCESS,
    &A_HEARTBEAT,
    &A_CLEAN,
    &A_REQUEST_PRESENTATION,
    &A_CHANGE_ACCEPT,
    &A_CHANGE_DENY
  };
  PARAM* STD_PARAMS[] = {
    &OPTION_SET,
    &OPTION_UNSET,
    &LIST_OPT,
    &LIST_CMD,
    &MOTOR_LEFT,
    &MOTOR_RIGHT,
    &MOTOR_BOTH,
    &DIRECTION_FORWARD,
    &DIRECTION_BACKWARD,
    &DIRECTION_STOP,
    &DIRECTION_FORWARD_LEFT,
    &DIRECTION_FORWARD_RIGHT,
    &DIRECTION_BACKWARD_LEFT,
    &DIRECTION_BACKWARD_RIGHT,
    &DIRECTION_LEFT,
    &DIRECTION_RIGHT
  };

}
