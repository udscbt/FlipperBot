/*************************************
 * Auto-generated ScheMo source file *
 *************************************
 * Original file name: main/main.ino
 * New file name: ./main/main.ino
 * Identified jobs: 3
 *   Named: 3
 *   Unnamed: 0
 * Identified tasks: 48
 *   Named: 2
 *   Unnamed: 46
 */

#include "schemo.h"
#include "fbcp.common.h"
#include "FBNet.h"

#include <ESP8266WiFi.h>

const char L_MOTOR = 16;
const char R_MOTOR = 14;
const char HIT     = 05;
fbcp::string fbcp::serial;
const char serial[] = "00001";
fbcp::string ssid;
bool AP = true;

const IPAddress host(
  FBNet::GATEWAY[0],
  FBNet::GATEWAY[1],
  FBNet::GATEWAY[2],
  FBNet::GATEWAY[3]
);
const IPAddress gateway(
  FBNet::GATEWAY[0],
  FBNet::GATEWAY[1],
  FBNet::GATEWAY[2],
  FBNet::GATEWAY[3]
);
const IPAddress submask(
  FBNet::NETMASK[0],
  FBNet::NETMASK[1],
  FBNet::NETMASK[2],
  FBNet::NETMASK[3]
);

WiFiClient sockIn;
WiFiClient sockOut;
WiFiServer server(FBNet::PORT);

enum
{
  IDLE,
  GAME,
  STANDALONE
} mode = IDLE;

SCHEMO_DECLARE_TASK(readCommand)
SCHEMO_DECLARE_TASK(__schemo_task_0)
SCHEMO_DECLARE_TASK(__schemo_task_1)
SCHEMO_DECLARE_TASK(__schemo_task_2)
SCHEMO_DECLARE_TASK(__schemo_task_3)
SCHEMO_DECLARE_TASK(__schemo_task_4)
SCHEMO_DECLARE_TASK(__schemo_task_5)
SCHEMO_DECLARE_TASK(tryConnect)
SCHEMO_DECLARE_TASK(__schemo_task_6)
SCHEMO_DECLARE_TASK(__schemo_task_7)
SCHEMO_DECLARE_TASK(__schemo_task_8)
SCHEMO_DECLARE_TASK(__schemo_task_9)
SCHEMO_DECLARE_TASK(__schemo_task_10)
SCHEMO_DECLARE_TASK(__schemo_task_11)
SCHEMO_DECLARE_TASK(__schemo_task_12)
SCHEMO_DECLARE_TASK(__schemo_task_13)
SCHEMO_DECLARE_TASK(__schemo_task_14)
SCHEMO_DECLARE_TASK(__schemo_task_15)
SCHEMO_DECLARE_TASK(__schemo_task_16)
SCHEMO_DECLARE_TASK(__schemo_task_17)
SCHEMO_DECLARE_TASK(__schemo_task_18)
SCHEMO_DECLARE_TASK(__schemo_task_19)
SCHEMO_DECLARE_TASK(__schemo_task_20)
SCHEMO_DECLARE_TASK(__schemo_task_21)
SCHEMO_DECLARE_TASK(__schemo_task_22)
SCHEMO_DECLARE_TASK(__schemo_task_23)
SCHEMO_DECLARE_TASK(__schemo_task_24)
SCHEMO_DECLARE_TASK(__schemo_task_25)
SCHEMO_DECLARE_TASK(__schemo_task_26)
SCHEMO_DECLARE_TASK(__schemo_task_27)
SCHEMO_DECLARE_TASK(__schemo_task_28)
SCHEMO_DECLARE_TASK(__schemo_task_29)
SCHEMO_DECLARE_TASK(__schemo_task_30)
SCHEMO_DECLARE_TASK(__schemo_task_31)
SCHEMO_DECLARE_TASK(__schemo_task_32)
SCHEMO_DECLARE_TASK(__schemo_task_33)
SCHEMO_DECLARE_TASK(__schemo_task_34)
SCHEMO_DECLARE_TASK(__schemo_task_35)
SCHEMO_DECLARE_TASK(__schemo_task_36)
SCHEMO_DECLARE_TASK(__schemo_task_37)
SCHEMO_DECLARE_TASK(__schemo_task_38)
SCHEMO_DECLARE_TASK(__schemo_task_39)
SCHEMO_DECLARE_TASK(__schemo_task_40)
SCHEMO_DECLARE_TASK(__schemo_task_41)
SCHEMO_DECLARE_TASK(__schemo_task_42)
SCHEMO_DECLARE_TASK(__schemo_task_43)
SCHEMO_DECLARE_TASK(__schemo_task_44)
SCHEMO_DECLARE_TASK(__schemo_task_45)
SCHEMO_DECLARE_JOB(job_client)
SCHEMO_DECLARE_JVAR(read, job_client, bool)
SCHEMO_DECLARE_JVAR(cmd, job_client, fbcp::COMMAND_LINE)
SCHEMO_DECLARE_JVAR(end, job_client, bool)
SCHEMO_DECLARE_JVAR(t, job_client, unsigned long)
SCHEMO_DECLARE_JOB(job_server)
SCHEMO_DECLARE_JVAR(cmd, job_server, fbcp::COMMAND_LINE)
SCHEMO_DECLARE_JOB(job_network)
SCHEMO_DECLARE_JVAR(i, job_network, int)
SCHEMO_DECLARE_JVAR(connected, job_network, bool)
SCHEMO_DECLARE_JVAR(n, job_network, int)
SCHEMO_DECLARE_PARAM_STACK(fbcp::string,ssid,tryConnect)
SCHEMO_DECLARE_FVAR(nc, tryConnect, int)
SCHEMO_DECLARE_FVAR(c, tryConnect, char)
SCHEMO_DECLARE_FVAR(cmd, tryConnect, fbcp::COMMAND_LINE)
SCHEMO_DECLARE_FVAR(t1, tryConnect, unsigned long)
SCHEMO_DECLARE_FVAR(msg, tryConnect, fbcp::string)
SCHEMO_DECLARE_FVAR(i, tryConnect, int)
SCHEMO_DECLARE_FVAR(end, tryConnect, bool)
SCHEMO_DECLARE_FVAR(t2, tryConnect, unsigned long)
SCHEMO_DECLARE_RETURN_STACK(bool,tryConnect)
SCHEMO_DECLARE_GETBACK_STACK(tryConnect)
SCHEMO_DECLARE_PARAM_STACK(WiFiClient*,sock,readCommand)
SCHEMO_DECLARE_PARAM_STACK(fbcp::COMMAND_LINE*,cmd,readCommand)
SCHEMO_DECLARE_PARAM_STACK(unsigned long,timeout,readCommand)
SCHEMO_DECLARE_FVAR(nc, readCommand, int)
SCHEMO_DECLARE_FVAR(c, readCommand, char)
SCHEMO_DECLARE_FVAR(t, readCommand, unsigned long)
SCHEMO_DECLARE_FVAR(end, readCommand, bool)
SCHEMO_DECLARE_FVAR(msg, readCommand, fbcp::string)
SCHEMO_DECLARE_FVAR(i, readCommand, int)
SCHEMO_DECLARE_RETURN_STACK(bool,readCommand)
SCHEMO_DECLARE_GETBACK_STACK(readCommand)


SCHEMO_FUNCTION(readCommand)





{
  SCHEMO_INIT_FVAR(nc, readCommand, int)
SCHEMO_INIT_FVAR(c, readCommand, char)
SCHEMO_INIT_FVAR(t, readCommand, unsigned long)
SCHEMO_INIT_FVAR(end, readCommand, bool)
SCHEMO_INIT_FVAR(msg, readCommand, fbcp::string)
SCHEMO_INIT_FVAR(i, readCommand, int)
  
  
  SCHEMO_FVAR(msg, readCommand) = "";
  SCHEMO_FVAR(end, readCommand) = false;
  SCHEMO_FVAR(t, readCommand) = millis();
  SCHEMO_WHILE(__schemo_task_0, __schemo_task_1, (!SCHEMO_FVAR(end, readCommand) && millis() - SCHEMO_FVAR(t, readCommand) < SCHEMO_PARAM(timeout, readCommand)))

    SCHEMO_FVAR(nc, readCommand) = SCHEMO_PARAM(sock, readCommand)->available();
    SCHEMO_FVAR(i, readCommand) = 0;
    SCHEMO_WHILE(__schemo_task_2, __schemo_task_3, (!SCHEMO_FVAR(end, readCommand) && SCHEMO_FVAR(i, readCommand) < SCHEMO_FVAR(nc, readCommand)))

      SCHEMO_FVAR(c, readCommand) = SCHEMO_PARAM(sock, readCommand)->read();
      SCHEMO_FVAR(msg, readCommand) += SCHEMO_FVAR(c, readCommand);
      
      if (SCHEMO_FVAR(c, readCommand) == '\n' or SCHEMO_FVAR(c, readCommand) == '\0')
      {
        SCHEMO_FVAR(end, readCommand) = true;
      }
      
      ++SCHEMO_FVAR(i, readCommand);
    SCHEMO_LOOPBACK(__schemo_task_2, __schemo_task_3)

  SCHEMO_LOOPBACK(__schemo_task_0, __schemo_task_1)

  
  SCHEMO_IF(__schemo_task_5, __schemo_task_4, (SCHEMO_FVAR(c, readCommand) == '\0'))

    Serial.println("Remote host said something REALLY strange :S");
    SCHEMO_GETBACK(false, readCommand)
SCHEMO_DELETE_PARAM(sock,readCommand)
SCHEMO_DELETE_PARAM(cmd,readCommand)
SCHEMO_DELETE_PARAM(timeout,readCommand)
SCHEMO_DELETE_FVAR(nc,readCommand)
SCHEMO_DELETE_FVAR(c,readCommand)
SCHEMO_DELETE_FVAR(t,readCommand)
SCHEMO_DELETE_FVAR(end,readCommand)
SCHEMO_DELETE_FVAR(msg,readCommand)
SCHEMO_DELETE_FVAR(i,readCommand)
SCHEMO_RETURN
  SCHEMO_ENDIF(__schemo_task_4)

  
  SCHEMO_GETBACK(fbcp::parseCommand(SCHEMO_FVAR(msg, readCommand), *SCHEMO_PARAM(cmd, readCommand)), readCommand)
SCHEMO_DELETE_PARAM(sock,readCommand)
SCHEMO_DELETE_PARAM(cmd,readCommand)
SCHEMO_DELETE_PARAM(timeout,readCommand)
SCHEMO_DELETE_FVAR(nc,readCommand)
SCHEMO_DELETE_FVAR(c,readCommand)
SCHEMO_DELETE_FVAR(t,readCommand)
SCHEMO_DELETE_FVAR(end,readCommand)
SCHEMO_DELETE_FVAR(msg,readCommand)
SCHEMO_DELETE_FVAR(i,readCommand)
SCHEMO_RETURN
}

SCHEMO_FUNCTION(tryConnect)



{
  SCHEMO_INIT_FVAR(nc, tryConnect, int)
SCHEMO_INIT_FVAR(c, tryConnect, char)
SCHEMO_INIT_FVAR(cmd, tryConnect, fbcp::COMMAND_LINE)
SCHEMO_INIT_FVAR(t1, tryConnect, unsigned long)
SCHEMO_INIT_FVAR(msg, tryConnect, fbcp::string)
SCHEMO_INIT_FVAR(i, tryConnect, int)
SCHEMO_INIT_FVAR(end, tryConnect, bool)
SCHEMO_INIT_FVAR(t2, tryConnect, unsigned long)

  
    Serial.print("Connecting to ");
  Serial.println(SCHEMO_PARAM(ssid, tryConnect).c_str());
  
  WiFi.begin(SCHEMO_PARAM(ssid, tryConnect).c_str());
  SCHEMO_FVAR(t1, tryConnect) = millis();
  SCHEMO_WHILE(__schemo_task_6, __schemo_task_7, (WiFi.status() != WL_CONNECTED && millis() - SCHEMO_FVAR(t1, tryConnect) < 10000))

    SCHEMO_FVAR(t2, tryConnect) = millis();
    SCHEMO_WHILE(__schemo_task_8, __schemo_task_9, (millis() - SCHEMO_FVAR(t2, tryConnect) < 500))
SCHEMO_LOOPBACK(__schemo_task_8, __schemo_task_9)

    Serial.print(".");
  SCHEMO_LOOPBACK(__schemo_task_6, __schemo_task_7)


  
  Serial.println("");
  SCHEMO_IF(__schemo_task_11, __schemo_task_10, (WiFi.status() != WL_CONNECTED))

    Serial.println("Timeout");
    SCHEMO_GETBACK(false, tryConnect)
SCHEMO_DELETE_PARAM(ssid,tryConnect)
SCHEMO_DELETE_FVAR(nc,tryConnect)
SCHEMO_DELETE_FVAR(c,tryConnect)
SCHEMO_DELETE_FVAR(cmd,tryConnect)
SCHEMO_DELETE_FVAR(t1,tryConnect)
SCHEMO_DELETE_FVAR(msg,tryConnect)
SCHEMO_DELETE_FVAR(i,tryConnect)
SCHEMO_DELETE_FVAR(end,tryConnect)
SCHEMO_DELETE_FVAR(t2,tryConnect)
SCHEMO_RETURN
  SCHEMO_ENDIF(__schemo_task_10)

  Serial.println("WiFi connected");  
  Serial.print("IP address: ");
  Serial.println(WiFi.localIP());
  
  SCHEMO_TBREAK(__schemo_task_18)
  
  SCHEMO_IF(__schemo_task_13, __schemo_task_12, (!sockOut.connect(gateway, FBNet::PORT)))

    Serial.println("Can't connect to server");
    SCHEMO_GETBACK(false, tryConnect)
SCHEMO_DELETE_PARAM(ssid,tryConnect)
SCHEMO_DELETE_FVAR(nc,tryConnect)
SCHEMO_DELETE_FVAR(c,tryConnect)
SCHEMO_DELETE_FVAR(cmd,tryConnect)
SCHEMO_DELETE_FVAR(t1,tryConnect)
SCHEMO_DELETE_FVAR(msg,tryConnect)
SCHEMO_DELETE_FVAR(i,tryConnect)
SCHEMO_DELETE_FVAR(end,tryConnect)
SCHEMO_DELETE_FVAR(t2,tryConnect)
SCHEMO_RETURN
  SCHEMO_ENDIF(__schemo_task_12)

  
  SCHEMO_FVAR(cmd, tryConnect).command = &fbcp::Q_SINGLE_PRESENTATION;
  SCHEMO_FVAR(cmd, tryConnect).params["serial"] = fbcp::serial;
  sockOut.print(fbcp::writeCommand(SCHEMO_FVAR(cmd, tryConnect)).c_str());
  
  SCHEMO_PUSH_PARAM(sock,readCommand,&sockOut)
SCHEMO_PUSH_PARAM(cmd,readCommand,&SCHEMO_FVAR(cmd, tryConnect))
SCHEMO_PUSH_PARAM(timeout,readCommand,2000)
SCHEMO_CALL(bool, readCommand, __schemo_task_42, understood)

  if (!understood)
  {
    Serial.println("Couldn't understand server response");
    SCHEMO_GETBACK(false, tryConnect)
SCHEMO_DELETE_PARAM(ssid,tryConnect)
SCHEMO_DELETE_FVAR(nc,tryConnect)
SCHEMO_DELETE_FVAR(c,tryConnect)
SCHEMO_DELETE_FVAR(cmd,tryConnect)
SCHEMO_DELETE_FVAR(t1,tryConnect)
SCHEMO_DELETE_FVAR(msg,tryConnect)
SCHEMO_DELETE_FVAR(i,tryConnect)
SCHEMO_DELETE_FVAR(end,tryConnect)
SCHEMO_DELETE_FVAR(t2,tryConnect)
SCHEMO_RETURN
  }
  SCHEMO_IF(__schemo_task_15, __schemo_task_14, (SCHEMO_FVAR(cmd, tryConnect).command->code == fbcp::A_GRANT_ACCESS.code))

    Serial.println("Server allowed connection");
    SCHEMO_GETBACK(true, tryConnect)
SCHEMO_DELETE_PARAM(ssid,tryConnect)
SCHEMO_DELETE_FVAR(nc,tryConnect)
SCHEMO_DELETE_FVAR(c,tryConnect)
SCHEMO_DELETE_FVAR(cmd,tryConnect)
SCHEMO_DELETE_FVAR(t1,tryConnect)
SCHEMO_DELETE_FVAR(msg,tryConnect)
SCHEMO_DELETE_FVAR(i,tryConnect)
SCHEMO_DELETE_FVAR(end,tryConnect)
SCHEMO_DELETE_FVAR(t2,tryConnect)
SCHEMO_RETURN
  SCHEMO_ENDIF(__schemo_task_14)

  SCHEMO_IF(__schemo_task_17, __schemo_task_16, (SCHEMO_FVAR(cmd, tryConnect).command->code == fbcp::A_DENY_ACCESS.code))

    Serial.println("Server refused connection");
    SCHEMO_GETBACK(false, tryConnect)
SCHEMO_DELETE_PARAM(ssid,tryConnect)
SCHEMO_DELETE_FVAR(nc,tryConnect)
SCHEMO_DELETE_FVAR(c,tryConnect)
SCHEMO_DELETE_FVAR(cmd,tryConnect)
SCHEMO_DELETE_FVAR(t1,tryConnect)
SCHEMO_DELETE_FVAR(msg,tryConnect)
SCHEMO_DELETE_FVAR(i,tryConnect)
SCHEMO_DELETE_FVAR(end,tryConnect)
SCHEMO_DELETE_FVAR(t2,tryConnect)
SCHEMO_RETURN
  SCHEMO_ENDIF(__schemo_task_16)

  Serial.println("Server answered something strange :S");
  SCHEMO_GETBACK(false, tryConnect)
SCHEMO_DELETE_PARAM(ssid,tryConnect)
SCHEMO_DELETE_FVAR(nc,tryConnect)
SCHEMO_DELETE_FVAR(c,tryConnect)
SCHEMO_DELETE_FVAR(cmd,tryConnect)
SCHEMO_DELETE_FVAR(t1,tryConnect)
SCHEMO_DELETE_FVAR(msg,tryConnect)
SCHEMO_DELETE_FVAR(i,tryConnect)
SCHEMO_DELETE_FVAR(end,tryConnect)
SCHEMO_DELETE_FVAR(t2,tryConnect)
SCHEMO_RETURN
}

SCHEMO_JOB(job_network,__schemo_task_19)
{
  

          Serial.println("Scan start");

  SCHEMO_JVAR(n, job_network) = WiFi.scanNetworks();
  Serial.println("Scan done");
  SCHEMO_IF(__schemo_task_23, __schemo_task_22, (SCHEMO_JVAR(n, job_network) == 0))
    Serial.println("No networks found");
  SCHEMO_ENDIF(__schemo_task_22)
  SCHEMO_IF(__schemo_task_25, __schemo_task_24, (SCHEMO_JVAR(n, job_network) != 0))
    Serial.print(SCHEMO_JVAR(n, job_network));
    Serial.println(" networks found");
    SCHEMO_JVAR(i, job_network) = 0;
    SCHEMO_JVAR(connected, job_network) = false;
    SCHEMO_WHILE(__schemo_task_20, __schemo_task_21, (SCHEMO_JVAR(i, job_network) < SCHEMO_JVAR(n, job_network) && !SCHEMO_JVAR(connected, job_network)))
            Serial.print(SCHEMO_JVAR(i, job_network) + 1);
      Serial.print(": ");
      ssid = WiFi.SSID(SCHEMO_JVAR(i, job_network)).c_str();
      Serial.print(ssid.c_str());
      Serial.print(" (");
      Serial.print(WiFi.RSSI(SCHEMO_JVAR(i, job_network)));
      Serial.print(")");
      bool noenc = (WiFi.encryptionType(SCHEMO_JVAR(i, job_network)) == ENC_TYPE_NONE);
      Serial.println(noenc?" ":"*");
            SCHEMO_IF(__schemo_task_27, __schemo_task_26, (noenc && ssid.startsWith(fbcp::BOARD_PREFIX)))
        Serial.println("Found suitable network");
        SCHEMO_PUSH_PARAM(ssid,tryConnect,ssid)
SCHEMO_CALL(bool, tryConnect, __schemo_task_43, connected)

        SCHEMO_JVAR(connected, job_network) = connected;
        if (connected)
        {
          mode = GAME;
          Serial.println("Connection estabilished");
          Serial.println("Getting into Game mode");
          
          schemo::schedule_job(job_client);
        }
        else
        {
          Serial.println("Connection failed");
        }
      SCHEMO_ENDIF(__schemo_task_26)
      
      ++SCHEMO_JVAR(i, job_network);
    SCHEMO_LOOPBACK(__schemo_task_20, __schemo_task_21)
  SCHEMO_ENDIF(__schemo_task_24)
  Serial.println("");
    
  SCHEMO_IF(__schemo_task_29, __schemo_task_28, (!SCHEMO_JVAR(connected, job_network)))     ssid = fbcp::serial;
    Serial.print("Starting AccessPoint: ");
    Serial.println(ssid);
    WiFi.softAP(ssid);
    WiFi.softAPConfig(host, gateway, submask);
    Serial.print("IP address: ");
    Serial.println(WiFi.softAPIP());
    mode = STANDALONE;
    Serial.println("Getting into Stand Alone mode");
  SCHEMO_ENDIF(__schemo_task_28)

    Serial.print("Starting server on port ");
  Serial.println(FBNet::PORT);
  server.begin();
  
  schemo::schedule_job(job_server);
}

void leftMotor(int dir)
{
  if (dir > 0)
  {
    analogWrite(L_MOTOR, 99);
  }
  else if (dir < 0)
  {
    analogWrite(L_MOTOR, 1);
  }
  else
  {
    analogWrite(L_MOTOR, 0);
  }
}

void rightMotor(int dir)
{
  if (dir > 0)
  {
    analogWrite(R_MOTOR, 1);
  }
  else if (dir < 0)
  {
    analogWrite(R_MOTOR, 99);
  }
  else
  {
    analogWrite(L_MOTOR, 0);
  }
}

SCHEMO_JOB(job_server,__schemo_task_30)
{
  

  SCHEMO_WHILE(__schemo_task_31, __schemo_task_32, (mode == IDLE))SCHEMO_LOOPBACK(__schemo_task_31, __schemo_task_32)
  sockIn = server.available();
  SCHEMO_WHILE(__schemo_task_33, __schemo_task_34, (sockIn.connected()))
    SCHEMO_PUSH_PARAM(sock,readCommand,&sockIn)
SCHEMO_PUSH_PARAM(cmd,readCommand,&SCHEMO_JVAR(cmd, job_server))
SCHEMO_PUSH_PARAM(timeout,readCommand,2000)
SCHEMO_CALL(bool, readCommand, __schemo_task_44, understood)

    if (understood)
    {
    Serial.print("Received: ");
    Serial.println(fbcp::writeCommand(SCHEMO_JVAR(cmd, job_server)).c_str());
    if (SCHEMO_JVAR(cmd, job_server).command->code == fbcp::Q_ROBOT_COMMAND.code)
    {
      if (SCHEMO_JVAR(cmd, job_server).params["direction"] == fbcp::DIRECTION_FORWARD.str)
      {
        leftMotor(1);
        rightMotor(1);
      }
      else if (SCHEMO_JVAR(cmd, job_server).params["direction"] == fbcp::DIRECTION_BACKWARD.str)
      {
        leftMotor(-1);
        rightMotor(-1);
      }
      else if (SCHEMO_JVAR(cmd, job_server).params["direction"] == fbcp::DIRECTION_STOP.str)
      {
        leftMotor(0);
        rightMotor(0);
      }
      else if (SCHEMO_JVAR(cmd, job_server).params["direction"] == fbcp::DIRECTION_FORWARD_LEFT.str)
      {
        leftMotor(0);
        rightMotor(1);
      }
      else if (SCHEMO_JVAR(cmd, job_server).params["direction"] == fbcp::DIRECTION_BACKWARD_LEFT.str)
      {
        leftMotor(0);
        rightMotor(-1);
      }
      else if (SCHEMO_JVAR(cmd, job_server).params["direction"] == fbcp::DIRECTION_LEFT.str)
      {
        leftMotor(-1);
        rightMotor(1);
      }
      else if (SCHEMO_JVAR(cmd, job_server).params["direction"] == fbcp::DIRECTION_FORWARD_RIGHT.str)
      {
        leftMotor(1);
        rightMotor(0);
      }
      else if (SCHEMO_JVAR(cmd, job_server).params["direction"] == fbcp::DIRECTION_BACKWARD_RIGHT.str)
      {
        leftMotor(-1);
        rightMotor(0);
      }
      else if (SCHEMO_JVAR(cmd, job_server).params["direction"] == fbcp::DIRECTION_RIGHT.str)
      {
        leftMotor(1);
        rightMotor(-1);
      }
      else
      {
        understood = false;
      }
    }
    else if (SCHEMO_JVAR(cmd, job_server).command->code == fbcp::Q_MOTOR_COMMAND.code)
    {
      int dir;
      if (SCHEMO_JVAR(cmd, job_server).params["direction"] == fbcp::DIRECTION_FORWARD.str)
      {
        dir = 1;
      }
      else if (SCHEMO_JVAR(cmd, job_server).params["direction"] == fbcp::DIRECTION_RIGHT.str)
      {
        dir = -1;
      }
      else if (SCHEMO_JVAR(cmd, job_server).params["direction"] == fbcp::DIRECTION_RIGHT.str)
      {
        dir = 0;
      }
      else
      {
        understood = false;
      }
      
      if (understood)
      {
        if (SCHEMO_JVAR(cmd, job_server).params["motor"] == fbcp::MOTOR_LEFT.str)
        {
          leftMotor(dir);
        }
        else if (SCHEMO_JVAR(cmd, job_server).params["motor"] == fbcp::MOTOR_RIGHT.str)
        {
          rightMotor(dir);
        }
        else if (SCHEMO_JVAR(cmd, job_server).params["motor"] == fbcp::MOTOR_BOTH.str)
        {
          leftMotor(dir);
          rightMotor(dir);
        }
        else
        {
          understood = false;
        }
      }
    }
    else
    {
      understood = false;
    }
  }
  
  if (understood)
  {
    SCHEMO_JVAR(cmd, job_server).command = &fbcp::A_ACCEPT;
    SCHEMO_JVAR(cmd, job_server).other = "";
  }
  else
  {
    SCHEMO_JVAR(cmd, job_server).command = &fbcp::A_ERROR;
    SCHEMO_JVAR(cmd, job_server).other = "";
  }
  Serial.print("Sent: ");
  fbcp::string s = fbcp::writeCommand(SCHEMO_JVAR(cmd, job_server));
  Serial.println(s.c_str());
  sockIn.print(s.c_str());
  SCHEMO_LOOPBACK(__schemo_task_33, __schemo_task_34)
}

SCHEMO_JOB(job_client,__schemo_task_35)
{
  
  
  SCHEMO_WHILE(__schemo_task_36, __schemo_task_37, (sockIn.connected()))
    SCHEMO_JVAR(read, job_client) = digitalRead(HIT);
    SCHEMO_JVAR(end, job_client) = false;
    SCHEMO_IF(__schemo_task_41, __schemo_task_40, (SCHEMO_JVAR(read, job_client)))
      SCHEMO_WHILE(__schemo_task_38, __schemo_task_39, (!SCHEMO_JVAR(end, job_client) && millis() - SCHEMO_JVAR(t, job_client) < 2000))
        SCHEMO_JVAR(cmd, job_client).command = &fbcp::Q_HIT;
        SCHEMO_JVAR(cmd, job_client).other = "";
        
        Serial.print("Sent: ");
        fbcp::string s = fbcp::writeCommand(SCHEMO_JVAR(cmd, job_client));
        Serial.println(s.c_str());
        sockOut.print(s.c_str());

        SCHEMO_PUSH_PARAM(sock,readCommand,&sockOut)
SCHEMO_PUSH_PARAM(cmd,readCommand,&SCHEMO_JVAR(cmd, job_client))
SCHEMO_PUSH_PARAM(timeout,readCommand,2000)
SCHEMO_CALL(bool, readCommand, __schemo_task_45, understood)

        if (understood && SCHEMO_JVAR(cmd, job_client).command->code == fbcp::A_ACCEPT.code)
        {
          SCHEMO_JVAR(end, job_client) = true;
        }
        else
        {
          Serial.println("Resending...");
        }
      SCHEMO_LOOPBACK(__schemo_task_38, __schemo_task_39)
    SCHEMO_ENDIF(__schemo_task_40)
  SCHEMO_LOOPBACK(__schemo_task_36, __schemo_task_37)
}

void setup()
{
    Serial.begin(115200);
  delay(10);

    analogWriteRange(100);
  pinMode(R_MOTOR, OUTPUT);
  analogWrite(R_MOTOR, 0);
  pinMode(L_MOTOR, OUTPUT);
  analogWrite(L_MOTOR, 0);
  
    fbcp::serial = fbcp::ROBOT_PREFIX;
  fbcp::serial += serial;
  mode = IDLE;
  
    SCHEMO_INIT
SCHEMO_INIT_JOB(job_client,__schemo_task_35)
SCHEMO_INIT_JOB(job_server,__schemo_task_30)
SCHEMO_INIT_JOB(job_network,__schemo_task_19)

  
  schemo::schedule_job(job_network);
  
  schemo::start_cycle();
}

void loop()
{
  
}
