#include "schemo.h"
#include "fbcp.h"
#include "fbcp.common.h"
#include <vector>
#include <map>

const int led1 = 15;
const int led2 = 4;

@DECLARE

@JOB
{
  @MEMORY
  {
    @VAR(i:bool)
    @VAR(old:unsigned int)
  }
  
  @VAR(i) = false;

  @VAR(old) = millis();
  @WHILE
  {
    @WHILE (millis() - @VAR(old) < 500) {}
    
    @VAR(old) = millis();
    @VAR(i) = !@VAR(i);
    digitalWrite(led1, @VAR(i)?HIGH:LOW);
  }
}

char buf[100];

@JOB
{
  Serial.begin(115200);
  
  @WHILE
  {
    digitalWrite(led2, HIGH);
    @WHILE(Serial.available() < 1) {}
    digitalWrite(led2, LOW);
    
    Serial.readBytesUntil('\n', buf, 100);
    fbcp::COMMAND_LINE q_cmd, a_cmd;
    if (!fbcp::parseCommand(buf, q_cmd) or !fbcp::common::handleRequest(q_cmd, a_cmd))
    {
      fbcp::common::handleNotFound(buf, a_cmd);
    }
    Serial.print(fbcp::writeCommand(a_cmd).c_str());
  }
}

void setup()
{
  @INIT
  @SCHEDULE_ALL

  pinMode(led1, OUTPUT);
  pinMode(led2, OUTPUT);
  
  schemo::start_cycle();
}

void loop()
{
}
