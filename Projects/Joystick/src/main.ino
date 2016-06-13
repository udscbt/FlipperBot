#include "schemo.h"
#include "fbcp.common.h"
#include "FBNet.h"

#include <ESP8266WiFi.h>

const int INTER_AXIS_INTERVAL = 10;
const int INTER_READ_INTERVAL = INTER_AXIS_INTERVAL;
const int WAIT_AFTER_REFUSED  = 1000;

const char inPin = A0;
const char selPin = 12;
const char ledPin = 15;

int horizontalValue;
int verticalValue;

char lOut;
char rOut;
char uOut;
char dOut;

enum
{
  SEND_INIT,
  SEND_BUSY,
  SEND_FAIL,
  SEND_NEW,
  SEND_OK
} sendStatus = SEND_INIT;

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

@DECLARE

@FUNCTION(readCommand)
@PARAM(sock:WiFiClient*)
@PARAM(cmd:fbcp::COMMAND_LINE*)
@PARAM(timeout:unsigned long)
@RETURN(bool)
{
  @MEMORY
  {
    @VAR(msg:fbcp::string)
    @VAR(end:bool)
    @VAR(nc:int)
    @VAR(c:char)
    @VAR(i:int)
    @VAR(t:unsigned long)
  }  
  
  @VAR(msg) = "";
  @VAR(end) = false;
  @VAR(t) = millis();
  @WHILE (!@VAR(end) && millis() - @VAR(t) < @PARAM(timeout))
  {
    @VAR(nc) = @PARAM(sock)->available();
    @VAR(i) = 0;
    @WHILE (!@VAR(end) && @VAR(i) < @VAR(nc))
    {
      @VAR(c) = @PARAM(sock)->read();
      @VAR(msg) += @VAR(c);
      
      if (@VAR(c) == '\n' or @VAR(c) == '\0')
      {
        @VAR(end) = true;
      }
      
      ++@VAR(i);
    }
  }
  
  @IF (@VAR(c) == '\0')
  {
    Serial.println("Remote host said something REALLY strange :S");
    @RETURN(false);
  }

  @IF (millis() - @VAR(t) >= @PARAM(timeout))
  {
    Serial.println("Timeout");
    @RETURN(false);
  }
  
  @RETURN(fbcp::parseCommand(@VAR(msg), *@PARAM(cmd)));
}

@FUNCTION(wait)
@PARAM(dt:unsigned int)
@RETURN(char)
{
  @MEMORY
  {
    @VAR(t0:unsigned int)
  }
  @VAR(t0) = millis();
  @WHILE (millis() - @VAR(t0) < @PARAM(dt)) {}
  @RETURN(0);
}

@JOB (job_network)
{
  @MEMORY
  {
    @VAR(i:int)
    @VAR(n:int)
    @VAR(connected:bool)

    @VAR(board:bool)
    @VAR(robot:bool)

    //tryConnect
    @VAR(t1:unsigned long)
    @VAR(t2:unsigned long)
    
    @VAR(cmd:fbcp::COMMAND_LINE)
    
    @VAR(j:int)
    @VAR(nc:int)
    @VAR(c:char)
    @VAR(end:bool)
    @VAR(msg:fbcp::string)
  }

  @WHILE (mode == MODE_IDLE)
  {
    /////////////////
    // WiFi discovery:
    //WiFi.mode(WIFI_STA);
    //WiFi.disconnect();  
    Serial.println("Scan start");
    mode = MODE_SCAN;

    WiFi.scanNetworks(true);
    @WHILE ((@VAR(n) = WiFi.scanComplete()) == WIFI_SCAN_RUNNING) {}
    if (@VAR(n) == WIFI_SCAN_FAILED)
    {
      Serial.println("Scan failed");
    }
    else
    {
      Serial.println("Scan done");
    }
    if (@VAR(n) == 0)
    {
      Serial.println("No networks found");
    }
    @IF (@VAR(n) > 0)
    {
      Serial.print(@VAR(n));
      Serial.println(" networks found");
      @VAR(i) = 0;
      @VAR(connected) = false;
      @WHILE (@VAR(i) < @VAR(n) && !@VAR(connected))
      {
        // Print SSID and RSSI for each network found
        Serial.print(@VAR(i) + 1);
        Serial.print(": ");
        ssid = WiFi.SSID(@VAR(i)).c_str();
        Serial.print(ssid.c_str());
        Serial.print(" (");
        Serial.print(WiFi.RSSI(@VAR(i)));
        Serial.print(")");
        bool noenc = (WiFi.encryptionType(@VAR(i)) == ENC_TYPE_NONE);
        Serial.println(noenc?" ":"*");
        // Search for suitable network
        @IF (noenc)
        {
          @VAR(board) = false;
          @VAR(robot) = false;
          @IF (ssid.startsWith(fbcp::BOARD_PREFIX))
          {
            Serial.println("Found suitable board network");
            @VAR(board) = true;
            @VAR(connected) = true;
          }

          @IF (ssid.startsWith(fbcp::ROBOT_PREFIX))
          {
            Serial.println("Found suitable robot network");
            @VAR(robot) = true;
            @VAR(connected) = true;
          }

          @IF (@VAR(connected))
          {
            /*******************
             * tryConnect START *
             ********************/
            mode = MODE_NETCON;
            Serial.print("Connecting to ");
            Serial.println(ssid.c_str());
            
            @VAR(t1) = millis();

            @WHILE (status != WL_CONNECTED && millis() - @VAR(t1) < 100000)
            {
              status = WiFi.begin(ssid.c_str());
              @CALL(wait;500):null;
              Serial.print(".");
            }
            Serial.println("");

            @VAR(connected) = false;
            switch (status)
            {
              case WL_CONNECTED:
                Serial.println("WiFi connected");
                @VAR(connected) = true;
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
            
            @IF (@VAR(connected))
            {
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

              @VAR(connected) = sockOut.connect(gateway, FBNet::PORT);
                
              if (!@VAR(connected))
              {
                Serial.println("Can't connect to server");
              }

              @IF (@VAR(connected))
              {
                Serial.println("Connected");
                
                @VAR(cmd).command = &fbcp::Q_SINGLE_PRESENTATION;
                @VAR(cmd).params["serial"] = fbcp::serial;
                fbcp::string s = fbcp::writeCommand(@VAR(cmd));
                Serial.print("Sent: ");
                Serial.println(s.c_str());
                sockOut.print(s.c_str());
                
                @CALL(readCommand;&sockOut;&@VAR(cmd);2000):understood;
                @VAR(connected) = false;
                if (!understood)
                {
                  Serial.println("Couldn't understand server response");
                }
                else if (@VAR(cmd).command->code == fbcp::A_GRANT_ACCESS.code)
                {
                  Serial.println("Server allowed connection");
                  @VAR(connected) = true;
                }
                else if (@VAR(cmd).command->code == fbcp::A_DENY_ACCESS.code)
                {
                  Serial.println("Server refused connection");
                }
                else
                {
                  Serial.println("Server answered something strange :S");
                }
              }
            }
            /*******************
             * tryConnect STOP  *
             ********************/
          }
  
          if (@VAR(connected))
          {
            Serial.println("Connection estabilished");
            
            if (@VAR(board))
            {
              mode = MODE_GAME;
              Serial.println("Getting into Game mode");
            }
            if (@VAR(robot))
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
        }

        
        ++@VAR(i);
      }
    }
    Serial.println("");
    /////////////////

    @IF (!@VAR(connected))
    {
      mode = MODE_IDLE;
      @CALL(wait;10000):null;
    }
  }
}

@JOB(job_client)
{
  @MEMORY
  {
    @VAR(cmd:fbcp::COMMAND_LINE)
    @VAR(t:unsigned long)
    @VAR(end:bool)
    @VAR(read:bool)
  }
  
  @WHILE (sockOut.connected())
  {
    @VAR(t) = millis();
    @VAR(cmd).command = &fbcp::Q_ROBOT_COMMAND;

    digitalWrite(selPin, LOW);
    horizontalValue = analogRead(inPin);
    @CALL(wait;INTER_AXIS_INTERVAL):null;
    digitalWrite(selPin, HIGH);
    verticalValue = analogRead(inPin);
    lOut_old = lOut;
    rOut_old = rOut;
    uOut_old = uOut;
    dOut_old = dOut;
    
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
        @VAR(cmd).params["direction"] = fbcp::DIRECTION_FORWARD_LEFT.str;
      }
      else if (rOut)
      {
        @VAR(cmd).params["direction"] = fbcp::DIRECTION_FORWARD_RIGHT.str;
      }
      else
      {
        @VAR(cmd).params["direction"] = fbcp::DIRECTION_FORWARD.str;
      }
    }
    else if (dOut)
    {
      if (lOut)
      {
        @VAR(cmd).params["direction"] = fbcp::DIRECTION_BACKWARD_LEFT.str;
      }
      else if (rOut)
      {
        @VAR(cmd).params["direction"] = fbcp::DIRECTION_BACKWARD_RIGHT.str;
      }
      else
      {
        @VAR(cmd).params["direction"] = fbcp::DIRECTION_BACKWARD.str;
      }
    }
    else
    {
      if (lOut)
      {
        @VAR(cmd).params["direction"] = fbcp::DIRECTION_LEFT.str;
      }
      else if (rOut)
      {
        @VAR(cmd).params["direction"] = fbcp::DIRECTION_RIGHT.str;
      }
      else
      {
        @VAR(cmd).params["direction"] = fbcp::DIRECTION_STOP.str;
      }
    }

    if (
      lOut != lOut_old ||
      rOut != rOut_old ||
      uOut != uOut_old ||
      dOut != uOut_old
    )
    {
      sendStatus = SEND_NEW;
    }

    @IF (sendStatus != SEND_INIT || sendStatus == SEND_FAIL || sendStatus == SEND_NEW)
    {
      Serial.print("Sent: ");
      fbcp::string s = fbcp::writeCommand(@VAR(cmd));
      Serial.println(s.c_str());
      sockOut.print(s.c_str());

      @CALL(readCommand;&sockOut;&@VAR(cmd);INTER_READ_INTERVAL):understood;
      if (understood)
      {
        if (@VAR(cmd).command->code == fbcp::A_ACCEPT.code)
        {
          Serial.println("Command was accepted");
          sendStatus = SEND_OK;
        }
        else if (@VAR(cmd).command->code == fbcp::A_REFUSE.code)
        {
          Serial.println("Command was refused");
          sendStatus = SEND_BUSY;
        }
        else if (@VAR(cmd).command->code == fbcp::A_ERROR.code)
        {
          Serial.println("Server didn't understand command");
          sendStatus = SEND_FAIL;
        }
        else
        {
          Serial.println("Server answered something strange");
          sendStatus = SEND_FAIL;
        }
      }
      else
      {
        sendStatus = SEND_FAIL;
      }
    }
    @CALL(wait;@VAR(t)+INTER_READ_INTERVAL-millis()):null;

    @IF (sendStatus == SEND_BUSY)
    {
      @CALL(wait;WAIT_AFTER_REFUSED):null;
      sendStatus = SEND_OK;
    }
  }
  
  mode = MODE_IDLE;
}

@JOB (job_link)
{
  @MEMORY
  {
    @VAR(on:bool)
    @VAR(t:unsigned long)
    @VAR(frame:unsigned char)
  }

  @VAR(on) = false;
  @VAR(frame) = 0;

  @WHILE
  {
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
          @VAR(frame)++;
          switch (@VAR(frame))
          {
            case 0:
            case 2:
              ledFreq = FREQ_SLOW;
              break;
            case 1:
              ledFreq = FREQ_FAST;
              break;
            default:
              @VAR(frame) = 0;
              ledFreq = FREQ_SLOW;            
          }
          break;
        case MODE_STANDALONE:
          @VAR(frame)++;
          switch (@VAR(frame))
          {
            case 0:
            case 1:
              ledFreq = FREQ_FAST;
              break;
            case 2:
              ledFreq = FREQ_SLOW;
              break;
            default:
              @VAR(frame) = 0;
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
    @IF (ledFreq != FREQ_ON && ledFreq != FREQ_OFF)
    {
      @CALL(wait;1000/ledFreq):null;
      @VAR(on) = !@VAR(on);
      digitalWrite(ledPin, @VAR(on));
    }
  }
}

@JOB (avoid_breaking)
{
  @WHILE {yield();}
}

void setup()
{
  // Initialise serial communication
  Serial.begin(115200);
  delay(10);

  Serial.println("Starting");

  // Initialise pins
  pinMode(selPin, OUTPUT);
  pinMode(ledPin, OUTPUT);
  
  // FBCP
  Serial.println("FBCP");
  fbcp::serial = fbcp::CONTR_PREFIX;
  fbcp::serial += serial;
  mode = MODE_IDLE;
  
  //ScheMo
  Serial.println("ScheMo");
  @INIT

  //WiFi
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
