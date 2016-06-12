/*************************************
 * Auto-generated ScheMo source file *
 *************************************
 * Original file name: main/main.ino
 * New file name: ./main/main.ino
 * Identified jobs: 4
 *   Named: 4
 *   Unnamed: 0
 * Identified tasks: 55
 *   Named: 2
 *   Unnamed: 53
 */

#include "schemo.h"
#include "fbcp.common.h"
#include "FBNet.h"

#include <ESP8266WiFi.h>

const int INTER_AXIS_INTERVAL = 10;
const int INTER_READ_INTERVAL = INTER_AXIS_INTERVAL;

const char inPin = A0;
const char selPin = 12;
const char ledPin = 15;

int horizontalValue;
int verticalValue;

char lOut;
char rOut;
char uOut;
char dOut;

fbcp::string fbcp::serial;
const char serial[] = "00001";
fbcp::string ssid;
int status = WL_IDLE_STATUS;
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

WiFiClient sockOut;

enum
{
  MODE_IDLE,
  MODE_SCAN,
  MODE_NETCON,
  MODE_SERCON,
  MODE_GAME,
  MODE_STANDALONE
} mode = MODE_IDLE;

float ledFreq = 0;
bool ledAuto = true;

enum
{
  FREQ_OFF  = -1,
  FREQ_ON   = 0,
  FREQ_SLOW = 2,
  FREQ_FAST  = 10
};

SCHEMO_SET_NUM_JOBS(4)
SCHEMO_DECLARE_TASK(readCommand)
SCHEMO_DECLARE_TASK(__schemo_task_0)
SCHEMO_DECLARE_TASK(__schemo_task_1)
SCHEMO_DECLARE_TASK(__schemo_task_2)
SCHEMO_DECLARE_TASK(__schemo_task_3)
SCHEMO_DECLARE_TASK(__schemo_task_4)
SCHEMO_DECLARE_TASK(__schemo_task_5)
SCHEMO_DECLARE_TASK(__schemo_task_6)
SCHEMO_DECLARE_TASK(__schemo_task_7)
SCHEMO_DECLARE_TASK(wait)
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
SCHEMO_DECLARE_TASK(__schemo_task_46)
SCHEMO_DECLARE_TASK(__schemo_task_47)
SCHEMO_DECLARE_TASK(__schemo_task_48)
SCHEMO_DECLARE_TASK(__schemo_task_49)
SCHEMO_DECLARE_TASK(__schemo_task_50)
SCHEMO_DECLARE_TASK(__schemo_task_51)
SCHEMO_DECLARE_TASK(__schemo_task_52)
SCHEMO_DECLARE_JOB(avoid_breaking)
SCHEMO_DECLARE_JOB(job_client)
SCHEMO_DECLARE_JVAR(end, job_client, bool)
SCHEMO_DECLARE_JVAR(cmd, job_client, fbcp::COMMAND_LINE)
SCHEMO_DECLARE_JVAR(t, job_client, unsigned long)
SCHEMO_DECLARE_JVAR(read, job_client, bool)
SCHEMO_DECLARE_JOB(job_network)
SCHEMO_DECLARE_JVAR(c, job_network, char)
SCHEMO_DECLARE_JVAR(robot, job_network, bool)
SCHEMO_DECLARE_JVAR(t1, job_network, unsigned long)
SCHEMO_DECLARE_JVAR(end, job_network, bool)
SCHEMO_DECLARE_JVAR(t2, job_network, unsigned long)
SCHEMO_DECLARE_JVAR(nc, job_network, int)
SCHEMO_DECLARE_JVAR(board, job_network, bool)
SCHEMO_DECLARE_JVAR(n, job_network, int)
SCHEMO_DECLARE_JVAR(connected, job_network, bool)
SCHEMO_DECLARE_JVAR(i, job_network, int)
SCHEMO_DECLARE_JVAR(msg, job_network, fbcp::string)
SCHEMO_DECLARE_JVAR(cmd, job_network, fbcp::COMMAND_LINE)
SCHEMO_DECLARE_JVAR(j, job_network, int)
SCHEMO_DECLARE_JOB(job_link)
SCHEMO_DECLARE_JVAR(on, job_link, bool)
SCHEMO_DECLARE_JVAR(frame, job_link, unsigned char)
SCHEMO_DECLARE_JVAR(t, job_link, unsigned long)
SCHEMO_DECLARE_PARAM_STACK(WiFiClient*,sock,readCommand)
SCHEMO_DECLARE_PARAM_STACK(fbcp::COMMAND_LINE*,cmd,readCommand)
SCHEMO_DECLARE_PARAM_STACK(unsigned long,timeout,readCommand)
SCHEMO_DECLARE_FVAR(c, readCommand, char)
SCHEMO_DECLARE_FVAR(i, readCommand, int)
SCHEMO_DECLARE_FVAR(t, readCommand, unsigned long)
SCHEMO_DECLARE_FVAR(end, readCommand, bool)
SCHEMO_DECLARE_FVAR(nc, readCommand, int)
SCHEMO_DECLARE_FVAR(msg, readCommand, fbcp::string)
SCHEMO_DECLARE_RETURN_STACK(bool,readCommand)
SCHEMO_DECLARE_GETBACK_STACK(readCommand)
SCHEMO_DECLARE_PARAM_STACK(unsigned int,dt,wait)
SCHEMO_DECLARE_FVAR(t0, wait, unsigned int)
SCHEMO_DECLARE_RETURN_STACK(char,wait)
SCHEMO_DECLARE_GETBACK_STACK(wait)


SCHEMO_FUNCTION(readCommand)





{
  SCHEMO_INIT_FVAR(c, readCommand, char)
SCHEMO_INIT_FVAR(i, readCommand, int)
SCHEMO_INIT_FVAR(t, readCommand, unsigned long)
SCHEMO_INIT_FVAR(end, readCommand, bool)
SCHEMO_INIT_FVAR(nc, readCommand, int)
SCHEMO_INIT_FVAR(msg, readCommand, fbcp::string)
  
  
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
SCHEMO_DELETE_FVAR(c,readCommand)
SCHEMO_DELETE_FVAR(i,readCommand)
SCHEMO_DELETE_FVAR(t,readCommand)
SCHEMO_DELETE_FVAR(end,readCommand)
SCHEMO_DELETE_FVAR(nc,readCommand)
SCHEMO_DELETE_FVAR(msg,readCommand)
SCHEMO_RETURN
  SCHEMO_ENDIF(__schemo_task_4)


  SCHEMO_IF(__schemo_task_7, __schemo_task_6, (millis() - SCHEMO_FVAR(t, readCommand) >= SCHEMO_PARAM(timeout, readCommand)))

    Serial.println("Timeout");
    SCHEMO_GETBACK(false, readCommand)
SCHEMO_DELETE_PARAM(sock,readCommand)
SCHEMO_DELETE_PARAM(cmd,readCommand)
SCHEMO_DELETE_PARAM(timeout,readCommand)
SCHEMO_DELETE_FVAR(c,readCommand)
SCHEMO_DELETE_FVAR(i,readCommand)
SCHEMO_DELETE_FVAR(t,readCommand)
SCHEMO_DELETE_FVAR(end,readCommand)
SCHEMO_DELETE_FVAR(nc,readCommand)
SCHEMO_DELETE_FVAR(msg,readCommand)
SCHEMO_RETURN
  SCHEMO_ENDIF(__schemo_task_6)

  
  SCHEMO_GETBACK(fbcp::parseCommand(SCHEMO_FVAR(msg, readCommand), *SCHEMO_PARAM(cmd, readCommand)), readCommand)
SCHEMO_DELETE_PARAM(sock,readCommand)
SCHEMO_DELETE_PARAM(cmd,readCommand)
SCHEMO_DELETE_PARAM(timeout,readCommand)
SCHEMO_DELETE_FVAR(c,readCommand)
SCHEMO_DELETE_FVAR(i,readCommand)
SCHEMO_DELETE_FVAR(t,readCommand)
SCHEMO_DELETE_FVAR(end,readCommand)
SCHEMO_DELETE_FVAR(nc,readCommand)
SCHEMO_DELETE_FVAR(msg,readCommand)
SCHEMO_RETURN
}

SCHEMO_FUNCTION(wait)



{
  SCHEMO_INIT_FVAR(t0, wait, unsigned int)

  SCHEMO_FVAR(t0, wait) = millis();
  SCHEMO_WHILE(__schemo_task_8, __schemo_task_9, (millis() - SCHEMO_FVAR(t0, wait) < SCHEMO_PARAM(dt, wait)))
SCHEMO_LOOPBACK(__schemo_task_8, __schemo_task_9)

  SCHEMO_GETBACK(0, wait)
SCHEMO_DELETE_PARAM(dt,wait)
SCHEMO_DELETE_FVAR(t0,wait)
SCHEMO_RETURN
}

SCHEMO_JOB(job_network,__schemo_task_10)
{
  

  SCHEMO_WHILE(__schemo_task_11, __schemo_task_12, (mode == MODE_IDLE))
                    Serial.println("Scan start");
    mode = MODE_SCAN;

    WiFi.scanNetworks(true);
    SCHEMO_WHILE(__schemo_task_13, __schemo_task_14, ((SCHEMO_JVAR(n, job_network) = WiFi.scanComplete()) == WIFI_SCAN_RUNNING))SCHEMO_LOOPBACK(__schemo_task_13, __schemo_task_14)
    if (SCHEMO_JVAR(n, job_network) == WIFI_SCAN_FAILED)
    {
      Serial.println("Scan failed");
    }
    else
    {
      Serial.println("Scan done");
    }
    if (SCHEMO_JVAR(n, job_network) == 0)
    {
      Serial.println("No networks found");
    }
    SCHEMO_IF(__schemo_task_20, __schemo_task_19, (SCHEMO_JVAR(n, job_network) > 0))
      Serial.print(SCHEMO_JVAR(n, job_network));
      Serial.println(" networks found");
      SCHEMO_JVAR(i, job_network) = 0;
      SCHEMO_JVAR(connected, job_network) = false;
      SCHEMO_WHILE(__schemo_task_15, __schemo_task_16, (SCHEMO_JVAR(i, job_network) < SCHEMO_JVAR(n, job_network) && !SCHEMO_JVAR(connected, job_network)))
                Serial.print(SCHEMO_JVAR(i, job_network) + 1);
        Serial.print(": ");
        ssid = WiFi.SSID(SCHEMO_JVAR(i, job_network)).c_str();
        Serial.print(ssid.c_str());
        Serial.print(" (");
        Serial.print(WiFi.RSSI(SCHEMO_JVAR(i, job_network)));
        Serial.print(")");
        bool noenc = (WiFi.encryptionType(SCHEMO_JVAR(i, job_network)) == ENC_TYPE_NONE);
        Serial.println(noenc?" ":"*");
                SCHEMO_IF(__schemo_task_22, __schemo_task_21, (noenc))
          SCHEMO_JVAR(board, job_network) = false;
          SCHEMO_JVAR(robot, job_network) = false;
          SCHEMO_IF(__schemo_task_24, __schemo_task_23, (ssid.startsWith(fbcp::BOARD_PREFIX)))
            Serial.println("Found suitable board network");
            SCHEMO_JVAR(board, job_network) = true;
            SCHEMO_JVAR(connected, job_network) = true;
          SCHEMO_ENDIF(__schemo_task_23)

          SCHEMO_IF(__schemo_task_26, __schemo_task_25, (ssid.startsWith(fbcp::ROBOT_PREFIX)))
            Serial.println("Found suitable robot network");
            SCHEMO_JVAR(robot, job_network) = true;
            SCHEMO_JVAR(connected, job_network) = true;
          SCHEMO_ENDIF(__schemo_task_25)

          SCHEMO_IF(__schemo_task_28, __schemo_task_27, (SCHEMO_JVAR(connected, job_network)))
            
            mode = MODE_NETCON;
            Serial.print("Connecting to ");
            Serial.println(ssid.c_str());
            
            SCHEMO_JVAR(t1, job_network) = millis();

            SCHEMO_WHILE(__schemo_task_17, __schemo_task_18, (status != WL_CONNECTED && millis() - SCHEMO_JVAR(t1, job_network) < 100000))
              status = WiFi.begin(ssid.c_str());
              SCHEMO_PUSH_PARAM(dt,wait,500)
SCHEMO_CALL(char, wait, __schemo_task_46, null)

              Serial.print(".");
            SCHEMO_LOOPBACK(__schemo_task_17, __schemo_task_18)
            Serial.println("");

            SCHEMO_JVAR(connected, job_network) = false;
            switch (status)
            {
              case WL_CONNECTED:
                Serial.println("WiFi connected");
                SCHEMO_JVAR(connected, job_network) = true;
                break;
              case WL_NO_SHIELD:
                Serial.println("No WiFi shield is present");
                break;
              case WL_IDLE_STATUS:
                Serial.println("Timeout");
                break;
              case WL_NO_SSID_AVAIL:
                Serial.println("No SSID are available");
                break;
              case WL_SCAN_COMPLETED:
                Serial.println("Scan networks is completed");
                break;
              case WL_CONNECT_FAILED:
                Serial.println("Connection failed for all the attempts");
                break;
              case WL_CONNECTION_LOST:
                Serial.println("Connection lost");
                break;
              case WL_DISCONNECTED:
                Serial.println("Disconnected from a network");
                break;
            }
            
            SCHEMO_IF(__schemo_task_30, __schemo_task_29, (SCHEMO_JVAR(connected, job_network)))
              Serial.print("IP address: ");
              Serial.println(WiFi.localIP());

              mode = MODE_SERCON;
              Serial.print("Trying to connect to ");
              Serial.print(FBNet::GATEWAY[0]);
              Serial.print(".");
              Serial.print(FBNet::GATEWAY[1]);
              Serial.print(".");
              Serial.print(FBNet::GATEWAY[2]);
              Serial.print(".");
              Serial.print(FBNet::GATEWAY[3]);
              Serial.print(":");
              Serial.println(FBNet::PORT);

              SCHEMO_JVAR(connected, job_network) = sockOut.connect(gateway, FBNet::PORT);
                
              if (!SCHEMO_JVAR(connected, job_network))
              {
                Serial.println("Can't connect to server");
              }

              SCHEMO_IF(__schemo_task_32, __schemo_task_31, (SCHEMO_JVAR(connected, job_network)))
                Serial.println("Connected");
                
                SCHEMO_JVAR(cmd, job_network).command = &fbcp::Q_SINGLE_PRESENTATION;
                SCHEMO_JVAR(cmd, job_network).params["serial"] = fbcp::serial;
                fbcp::string s = fbcp::writeCommand(SCHEMO_JVAR(cmd, job_network));
                Serial.print("Sent: ");
                Serial.println(s.c_str());
                sockOut.print(s.c_str());
                
                SCHEMO_PUSH_PARAM(sock,readCommand,&sockOut)
SCHEMO_PUSH_PARAM(cmd,readCommand,&SCHEMO_JVAR(cmd, job_network))
SCHEMO_PUSH_PARAM(timeout,readCommand,2000)
SCHEMO_CALL(bool, readCommand, __schemo_task_47, understood)

                SCHEMO_JVAR(connected, job_network) = false;
                if (!understood)
                {
                  Serial.println("Couldn't understand server response");
                }
                else if (SCHEMO_JVAR(cmd, job_network).command->code == fbcp::A_GRANT_ACCESS.code)
                {
                  Serial.println("Server allowed connection");
                  SCHEMO_JVAR(connected, job_network) = true;
                }
                else if (SCHEMO_JVAR(cmd, job_network).command->code == fbcp::A_DENY_ACCESS.code)
                {
                  Serial.println("Server refused connection");
                }
                else
                {
                  Serial.println("Server answered something strange :S");
                }
              SCHEMO_ENDIF(__schemo_task_31)
            SCHEMO_ENDIF(__schemo_task_29)
            
          SCHEMO_ENDIF(__schemo_task_27)
  
          if (SCHEMO_JVAR(connected, job_network))
          {
            Serial.println("Connection estabilished");
            
            if (SCHEMO_JVAR(board, job_network))
            {
              mode = MODE_GAME;
              Serial.println("Getting into Game mode");
            }
            if (SCHEMO_JVAR(robot, job_network))
            {
              mode = MODE_STANDALONE;
              Serial.println("Getting into Stand Alone mode");
            }
            schemo::schedule_job(job_client);
          }
          else
          {
            Serial.println("Connection failed");
          }
        SCHEMO_ENDIF(__schemo_task_21)

        
        ++SCHEMO_JVAR(i, job_network);
      SCHEMO_LOOPBACK(__schemo_task_15, __schemo_task_16)
    SCHEMO_ENDIF(__schemo_task_19)
    Serial.println("");
    
    SCHEMO_IF(__schemo_task_34, __schemo_task_33, (!SCHEMO_JVAR(connected, job_network)))
      mode = MODE_IDLE;
      SCHEMO_PUSH_PARAM(dt,wait,10000)
SCHEMO_CALL(char, wait, __schemo_task_48, null)

    SCHEMO_ENDIF(__schemo_task_33)
  SCHEMO_LOOPBACK(__schemo_task_11, __schemo_task_12)
}

SCHEMO_JOB(job_client,__schemo_task_35)
{
  
  
  SCHEMO_WHILE(__schemo_task_36, __schemo_task_37, (sockOut.connected()))
    SCHEMO_JVAR(cmd, job_client).command = &fbcp::Q_ROBOT_COMMAND;

    digitalWrite(selPin, LOW);
    horizontalValue = analogRead(inPin);
    SCHEMO_PUSH_PARAM(dt,wait,INTER_AXIS_INTERVAL)
SCHEMO_CALL(char, wait, __schemo_task_49, null)

    digitalWrite(selPin, HIGH);
    verticalValue = analogRead(inPin);
    lOut = LOW;
    rOut = LOW;
    uOut = LOW;
    dOut = LOW;
    if (horizontalValue < 256)
    {
      lOut = HIGH;
    }
    else if (horizontalValue > 768)
    {
      rOut = HIGH;
    }

    if (verticalValue < 156)
    {
      dOut = HIGH;
    }
    else if (verticalValue > 768)
    {
      uOut = HIGH;
    }

    Serial.print("Horizontal value: ");
    Serial.print(horizontalValue);
    Serial.print(" | ");
    Serial.println(lOut?"LEFT":rOut?"RIGHT":"CENTRE");
    Serial.print("Vertical value: ");
    Serial.print(verticalValue);
    Serial.print(" | ");
    Serial.println(uOut?"UP":dOut?"DOWN":"CENTRE");

    if (uOut)
    {
      if (lOut)
      {
        SCHEMO_JVAR(cmd, job_client).params["direction"] = fbcp::DIRECTION_FORWARD_LEFT.str;
      }
      else if (rOut)
      {
        SCHEMO_JVAR(cmd, job_client).params["direction"] = fbcp::DIRECTION_FORWARD_RIGHT.str;
      }
      else
      {
        SCHEMO_JVAR(cmd, job_client).params["direction"] = fbcp::DIRECTION_FORWARD.str;
      }
    }
    else if (dOut)
    {
      if (lOut)
      {
        SCHEMO_JVAR(cmd, job_client).params["direction"] = fbcp::DIRECTION_BACKWARD_LEFT.str;
      }
      else if (rOut)
      {
        SCHEMO_JVAR(cmd, job_client).params["direction"] = fbcp::DIRECTION_BACKWARD_RIGHT.str;
      }
      else
      {
        SCHEMO_JVAR(cmd, job_client).params["direction"] = fbcp::DIRECTION_BACKWARD.str;
      }
    }
    else
    {
      if (lOut)
      {
        SCHEMO_JVAR(cmd, job_client).params["direction"] = fbcp::DIRECTION_LEFT.str;
      }
      else if (rOut)
      {
        SCHEMO_JVAR(cmd, job_client).params["direction"] = fbcp::DIRECTION_RIGHT.str;
      }
      else
      {
        SCHEMO_JVAR(cmd, job_client).params["direction"] = fbcp::DIRECTION_STOP.str;
      }
    }

    Serial.print("Sent: ");
    fbcp::string s = fbcp::writeCommand(SCHEMO_JVAR(cmd, job_client));
    Serial.println(s.c_str());
    sockOut.print(s.c_str());

    SCHEMO_JVAR(t, job_client) = millis();
    SCHEMO_PUSH_PARAM(sock,readCommand,&sockOut)
SCHEMO_PUSH_PARAM(cmd,readCommand,&SCHEMO_JVAR(cmd, job_client))
SCHEMO_PUSH_PARAM(timeout,readCommand,INTER_READ_INTERVAL)
SCHEMO_CALL(bool, readCommand, __schemo_task_50, understood)

    if (understood)
    {
      if (SCHEMO_JVAR(cmd, job_client).command->code == fbcp::A_ACCEPT.code)
      {
        Serial.println("Command was accepted");
      }
      else if (SCHEMO_JVAR(cmd, job_client).command->code == fbcp::A_REFUSE.code)
      {
        Serial.println("Command was refused");
      }
      else if (SCHEMO_JVAR(cmd, job_client).command->code == fbcp::A_ERROR.code)
      {
        Serial.println("Server didn't understand command");
      }
      else
      {
        Serial.println("Server answered something strange");
      }
    }
    SCHEMO_PUSH_PARAM(dt,wait,SCHEMO_JVAR(t, job_client)+INTER_READ_INTERVAL-millis())
SCHEMO_CALL(char, wait, __schemo_task_51, null)

  SCHEMO_LOOPBACK(__schemo_task_36, __schemo_task_37)
  
  mode = MODE_IDLE;
}

SCHEMO_JOB(job_link,__schemo_task_38)
{
  

  SCHEMO_JVAR(on, job_link) = false;
  SCHEMO_JVAR(frame, job_link) = 0;

  SCHEMO_WHILE(__schemo_task_39, __schemo_task_40, (true))
    if (ledAuto)
    {
      switch (mode)
      {
        case MODE_IDLE:
          ledFreq = FREQ_OFF;
          break;
        case MODE_SCAN:
          ledFreq = FREQ_SLOW;
          break;
        case MODE_NETCON:
          ledFreq = FREQ_FAST;
          break;
        case MODE_SERCON:
          ledFreq = FREQ_FAST;
          break;
        case MODE_GAME:
          SCHEMO_JVAR(frame, job_link)++;
          switch (SCHEMO_JVAR(frame, job_link))
          {
            case 0:
            case 2:
              ledFreq = FREQ_SLOW;
              break;
            case 1:
              ledFreq = FREQ_FAST;
              break;
            default:
              SCHEMO_JVAR(frame, job_link) = 0;
              ledFreq = FREQ_SLOW;            
          }
          break;
        case MODE_STANDALONE:
          SCHEMO_JVAR(frame, job_link)++;
          switch (SCHEMO_JVAR(frame, job_link))
          {
            case 0:
            case 1:
              ledFreq = FREQ_FAST;
              break;
            case 2:
              ledFreq = FREQ_SLOW;
              break;
            default:
              SCHEMO_JVAR(frame, job_link) = 0;
              ledFreq = FREQ_FAST;            
          }
          break;
        default:
          ledFreq = FREQ_OFF;          
      }
    }
    
    if (ledFreq == FREQ_ON)
    {
      digitalWrite(ledPin, HIGH);
    }
    if (ledFreq == FREQ_OFF)
    {
      digitalWrite(ledPin, LOW);
    }
    SCHEMO_IF(__schemo_task_42, __schemo_task_41, (ledFreq != FREQ_ON && ledFreq != FREQ_OFF))
      SCHEMO_PUSH_PARAM(dt,wait,1000/ledFreq)
SCHEMO_CALL(char, wait, __schemo_task_52, null)

      SCHEMO_JVAR(on, job_link) = !SCHEMO_JVAR(on, job_link);
      digitalWrite(ledPin, SCHEMO_JVAR(on, job_link));
    SCHEMO_ENDIF(__schemo_task_41)
  SCHEMO_LOOPBACK(__schemo_task_39, __schemo_task_40)
}

SCHEMO_JOB(avoid_breaking,__schemo_task_43)
{
  SCHEMO_WHILE(__schemo_task_44, __schemo_task_45, (true))yield();SCHEMO_LOOPBACK(__schemo_task_44, __schemo_task_45)
}

void setup()
{
    Serial.begin(115200);
  delay(10);

  Serial.println("Starting");

    pinMode(selPin, OUTPUT);
  pinMode(ledPin, OUTPUT);
  
    Serial.println("FBCP");
  fbcp::serial = fbcp::CONTR_PREFIX;
  fbcp::serial += serial;
  mode = MODE_IDLE;
  
    Serial.println("ScheMo");
  SCHEMO_INIT
SCHEMO_INIT_JOB(avoid_breaking,__schemo_task_43)
SCHEMO_INIT_JOB(job_client,__schemo_task_35)
SCHEMO_INIT_JOB(job_network,__schemo_task_10)
SCHEMO_INIT_JOB(job_link,__schemo_task_38)


    WiFi.persistent(false);

  ledFreq = FREQ_OFF;
  schemo::schedule_job(job_link);
  schemo::schedule_job(job_network);
  schemo::schedule_job(avoid_breaking);
  Serial.println("Job scheduled");

  schemo::start_cycle();
}

void loop()
{
}
