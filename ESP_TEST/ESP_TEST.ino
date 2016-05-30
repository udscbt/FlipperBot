/*************************************
 * Auto-generated ScheMo source file *
 *************************************
 * Original file name: ESP_TEST.ino
 * New file name: ./__schemo_ESP_TEST.ino
 * Identified jobs: 2
 *   Named: 0
 *   Unnamed: 2
 * Identified tasks: 10
 *   Named: 0
 *   Unnamed: 10
 */

#include "schemo.h"
#include "fbcp.h"
#include "fbcp.common.h"

const int led1 = 15;
const int led2 = 4;

SCHEMO_DECLARE_TASK(__schemo_task_0)
SCHEMO_DECLARE_TASK(__schemo_task_1)
SCHEMO_DECLARE_TASK(__schemo_task_2)
SCHEMO_DECLARE_TASK(__schemo_task_3)
SCHEMO_DECLARE_TASK(__schemo_task_4)
SCHEMO_DECLARE_TASK(__schemo_task_5)
SCHEMO_DECLARE_TASK(__schemo_task_6)
SCHEMO_DECLARE_TASK(__schemo_task_7)
SCHEMO_DECLARE_TASK(__schemo_task_8)
SCHEMO_DECLARE_TASK(__schemo_task_9)
SCHEMO_DECLARE_JOB(__schemo_job_0)
SCHEMO_DECLARE_JVAR(i, __schemo_job_0, bool)
SCHEMO_DECLARE_JVAR(old, __schemo_job_0, unsigned int)
SCHEMO_DECLARE_JOB(__schemo_job_1)


SCHEMO_JOB(__schemo_job_0,__schemo_task_0)
{
  
  
  SCHEMO_JVAR(i, __schemo_job_0) = false;

  SCHEMO_JVAR(old, __schemo_job_0) = millis();
  SCHEMO_WHILE(__schemo_task_1, __schemo_task_2, (true))
    SCHEMO_WHILE(__schemo_task_3, __schemo_task_4, (millis() - SCHEMO_JVAR(old, __schemo_job_0) < 500))SCHEMO_LOOPBACK(__schemo_task_3, __schemo_task_4)
    
    SCHEMO_JVAR(old, __schemo_job_0) = millis();
    SCHEMO_JVAR(i, __schemo_job_0) = !SCHEMO_JVAR(i, __schemo_job_0);
    digitalWrite(led1, SCHEMO_JVAR(i, __schemo_job_0)?HIGH:LOW);
  SCHEMO_LOOPBACK(__schemo_task_1, __schemo_task_2)
}

char buf[100];

void debugCommand(fbcp::COMMAND_LINE& cmd)
{
  Serial.print("$ Interpreted as:\n");
  if (cmd.command == fbcp::NULL_COMMAND)
  {
    Serial.print("NULL_COMMAND");
  }
  else
  {
    Serial.print("Command: ");
    Serial.print(cmd.command->id.c_str());
    Serial.print("\nParams:\n");
    for (std::vector<fbcp::string>::const_iterator it = cmd.command->params.begin(); it != cmd.command->params.end(); ++it)
    {
      std::map<fbcp::string, fbcp::string>::iterator found = cmd.params.find(*it);
      Serial.print("  [");
      Serial.print((*it).c_str());
      Serial.print("] = ");
      Serial.print(found==cmd.params.end()?"<missing>":found->second.c_str());
      Serial.print("\n");
    }
    Serial.print("Other:\n  ");
    Serial.print(cmd.other.c_str());
  }
  Serial.print("\n\n");
  Serial.flush();
}

SCHEMO_JOB(__schemo_job_1,__schemo_task_5)
{
  Serial.begin(115200);
  
  SCHEMO_WHILE(__schemo_task_6, __schemo_task_7, (true))
    digitalWrite(led2, HIGH);
    SCHEMO_WHILE(__schemo_task_8, __schemo_task_9, (Serial.available() < 1))SCHEMO_LOOPBACK(__schemo_task_8, __schemo_task_9)
    digitalWrite(led2, LOW);
    
    int nbytes = Serial.readBytesUntil('\n', buf, 100);
    Serial.print("$ Received line:\n");
    buf[nbytes] = '\n';
    buf[nbytes+1] = '\0';
    Serial.print(buf);
    Serial.flush();
    
    fbcp::COMMAND_LINE q_cmd, a_cmd;
    bool valid = fbcp::parseCommand(buf, q_cmd);
    Serial.print("$ ");
    if (valid)
    {
      Serial.print("Understood\n\n");
      if (fbcp::common::handleRequest(q_cmd, a_cmd))
      {
        Serial.print("$ Handled");
      }
      else
      {
        Serial.print("$ Not handled");
        fbcp::common::handleNotFound(buf, a_cmd);
      }
    }
    else
    {
      Serial.print("Not understood");
      fbcp::common::handleNotFound(buf, a_cmd);
    }
    Serial.print("\n\n");
    
    debugCommand(q_cmd);

    Serial.print("$ Response:\n");
    Serial.print(fbcp::writeCommand(a_cmd).c_str());
    Serial.print("\n");

    Serial.flush();
  SCHEMO_LOOPBACK(__schemo_task_6, __schemo_task_7)
}

void setup()
{
  SCHEMO_INIT
SCHEMO_INIT_JOB(__schemo_job_0,__schemo_task_0)
SCHEMO_INIT_JOB(__schemo_job_1,__schemo_task_5)

  SCHEMO_SCHEDULE_JOB(__schemo_job_0)
SCHEMO_SCHEDULE_JOB(__schemo_job_1)


  pinMode(led1, OUTPUT);
  pinMode(led2, OUTPUT);
  
  schemo::start_cycle();
}

void loop()
{
}
