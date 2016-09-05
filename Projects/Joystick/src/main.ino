#include "schemo.h"
#include "fbcp.common.h"
#include "FBNet.h"

#include <ESP8266WiFi.h>

typedef unsigned char byte;
#include "ssidheap.h"

const int INTER_AXIS_INTERVAL = 10;
const int INTER_READ_INTERVAL = INTER_AXIS_INTERVAL;
const int WAIT_AFTER_REFUSED  = 1000;

const char inPin = A0;
const char selPin = 12;
const char ledPin = 15;
const char btnPin = 16;

int horizontalValue;
int verticalValue;

char lOut;
char rOut;
char uOut;
char dOut;

char lOut_old;
char rOut_old;
char uOut_old;
char dOut_old;

int lThreshold = 256;
int rThreshold = 768;

int dThreshold = 256;
int uThreshold = 768;

bool pressed = false;

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

fbcp::string kalCmd;

typedef enum
{
  READ_TIMEOUT,
  READ_SUCCESS,
  READ_FAIL
} READ_RESULT;

@DECLARE

@FUNCTION(read_command)
@PARAM(sock:WiFiClient*)
@PARAM(cmd:fbcp::COMMAND_LINE*)
@PARAM(timeout:unsigned long)
@RETURN(READ_RESULT)
{
  @MEMORY
  {
    @VAR(msg:fbcp::string)
    @VAR(t:unsigned long)
  }  
  
  @VAR(msg) = "";
  @VAR(t) = millis();
  @WHILE (millis() - @VAR(t) < @PARAM(timeout))
  {
    int nc = @PARAM(sock)->available();
    for (int i = 0; i < nc; ++i)
    {
      char c = @PARAM(sock)->read();
      @VAR(msg) += c;
      
      if (c == '\n' or c == '\0')
      {
        @BREAK
      }
    }
  }
  
  Serial.print(F("Received: '"));
  Serial.print(@VAR(msg).c_str());
  Serial.println(F("'"));
  
  @IF (millis() - @VAR(t) >= @PARAM(timeout))
  {
    Serial.println(F("Timeout"));
    @RETURN(READ_TIMEOUT);
  }
  
  @IF (@VAR(msg)[@VAR(msg).length()-1] == '\0')
  {
    Serial.println(F("Remote host said something REALLY strange :S"));
    @RETURN(READ_FAIL);
  }
  
  @RETURN(fbcp::parseCommand(@VAR(msg), *@PARAM(cmd))?READ_SUCCESS:READ_FAIL);
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
    @VAR(i:byte)
    @VAR(n:int)

    @VAR(boardrobot:bool)

    @VAR(t1:unsigned long)
    
    @VAR(cmd:fbcp::COMMAND_LINE)
  }

  @WHILE
  {
    @IF (mode == MODE_IDLE)
    {
      Serial.println(F("Scan start"));
      mode = MODE_SCAN;

      WiFi.scanNetworks(true);
      @WHILE ((@VAR(n) = WiFi.scanComplete()) == WIFI_SCAN_RUNNING) {}
      if (@VAR(n) == WIFI_SCAN_FAILED)
      {
        Serial.println(F("Scan failed"));
      }
      else
      {
        Serial.println(F("Scan done"));
      }
      if (@VAR(n) == 0)
      {
        Serial.println(F("No networks found"));
      }
      @IF (@VAR(n) > 0)
      {
        Serial.print(@VAR(n));
        Serial.println(F(" networks found"));
        @VAR(i) = 0;
        clearSsidHeap();
        @WHILE (@VAR(i) < @VAR(n))
        {
          insertSsid(@VAR(i));
          ++@VAR(i);
        }
        @WHILE ( (@VAR(i)=popSsid()) < @VAR(n))
        {
          // Print SSID and RSSI for each network found
          Serial.print(F("- "));
          ssid = WiFi.SSID(@VAR(i)).c_str();
          Serial.print(ssid.c_str());
          Serial.print(F(" ("));
          Serial.print(WiFi.RSSI(@VAR(i)));
          Serial.print(F(")"));
          bool noenc = (WiFi.encryptionType(@VAR(i)) == ENC_TYPE_NONE);
          Serial.println(noenc?F(" "):F("*"));
          // Search for suitable network
          if (!noenc)
          {
            @CONTINUE
          }
          
          if (ssid.startsWith(fbcp::BOARD_PREFIX))
          {
            Serial.println(F("Found suitable board network"));
            @VAR(boardrobot) = true;
          }
          else if (ssid.startsWith(fbcp::ROBOT_PREFIX))
          {
            Serial.println(F("Found suitable robot network"));
            @VAR(boardrobot) = false;
          }
          else
          {
            @CONTINUE
          }

          mode = MODE_NETCON;
          Serial.print(F("Connecting to "));
          Serial.println(ssid.c_str());
          
          @VAR(t1) = millis();

          @WHILE (status != WL_CONNECTED && millis() - @VAR(t1) < 100000)
          {
            status = WiFi.begin(ssid.c_str());
            @CALL(wait;500):null;
            Serial.print(F("."));
          }
          Serial.println("");

          switch (status)
          {
            case WL_CONNECTED:
              Serial.println(F("WiFi connected"));
              break;
            case WL_NO_SHIELD:
              Serial.println(F("No WiFi shield is present"));
              @CONTINUE
            case WL_IDLE_STATUS:
              Serial.println(F("Timeout"));
              @CONTINUE
            case WL_NO_SSID_AVAIL:
              Serial.println(F("No SSID are available"));
              @CONTINUE
            case WL_SCAN_COMPLETED:
              Serial.println(F("Scan networks is completed"));
              @CONTINUE
            case WL_CONNECT_FAILED:
              Serial.println(F("Connection failed for all the attempts"));
              @CONTINUE
            case WL_CONNECTION_LOST:
              Serial.println(F("Connection lost"));
              @CONTINUE
            case WL_DISCONNECTED:
              Serial.println(F("Disconnected from a network"));
              @CONTINUE
            default:
              @CONTINUE
          }
          
          Serial.print(F("IP address: "));
          Serial.println(WiFi.localIP());

          mode = MODE_SERCON;
          Serial.print(F("Trying to connect to "));
          Serial.print(FBNet::GATEWAY[0]);
          Serial.print(F("."));
          Serial.print(FBNet::GATEWAY[1]);
          Serial.print(F("."));
          Serial.print(FBNet::GATEWAY[2]);
          Serial.print(F("."));
          Serial.print(FBNet::GATEWAY[3]);
          Serial.print(F(":"));
          Serial.println(FBNet::PORT);
            
          if (!sockOut.connect(gateway, FBNet::PORT))
          {
            Serial.println(F("Can't connect to server"));
            @CONTINUE
          }

          Serial.println(F("Connected"));
          
          @VAR(cmd).command = &fbcp::Q_SINGLE_PRESENTATION;
          @VAR(cmd).params["serial"] = fbcp::serial;
          fbcp::string s = fbcp::writeCommand(@VAR(cmd));
          Serial.print(F("Sent: "));
          Serial.println(s.c_str());
          sockOut.flush();
          sockOut.print(s.c_str());
          
          @CALL(read_command;&sockOut;&@VAR(cmd);fbcp::HARD_TIMEOUT):understood;
          if (understood == READ_FAIL)
          {
            Serial.println(F("Couldn't understand server response"));
          }
          else if (understood == READ_TIMEOUT)
          {
            Serial.println(F("Server timed out"));
          }
          else if (@VAR(cmd).command->code == fbcp::A_GRANT_ACCESS.code)
          {
            Serial.println(F("Server allowed connection"));
            Serial.println(F("Connection estabilished"));
            
            if (@VAR(boardrobot))
            {
              mode = MODE_GAME;
              Serial.println(F("Getting into Game mode"));
            }
            else
            {
              mode = MODE_STANDALONE;
              Serial.println(F("Getting into Stand Alone mode"));
            }
            @BREAK
          }
          else if (@VAR(cmd).command->code == fbcp::A_DENY_ACCESS.code)
          {
            Serial.println(F("Server refused connection"));
          }
          else
          {
            Serial.println(F("Server answered something strange :S"));
          }

          sockOut.stop();
          Serial.println(F("Connection failed"));
        }
      }
      Serial.println(F(""));

      @IF (mode != MODE_GAME && mode != MODE_STANDALONE)
      {
        mode = MODE_IDLE;
        @CALL(wait;10000):null;
      }
    }
  }
}

@JOB(job_client)
{
  @MEMORY
  {
    @VAR(cmd:fbcp::COMMAND_LINE)
    @VAR(t:unsigned long)
    @VAR(last:unsigned long)
  }
  @WHILE
  {
    @VAR(last) = millis();
    @VAR(t) = 0;
    @WHILE ((mode == MODE_GAME || mode == MODE_STANDALONE) && sockOut.connected())
    {
      //Everything Button
      bool old = pressed;
      pressed = digitalRead(btnPin);
      @IF (old != pressed)
      {
        if (pressed)
        {
          @VAR(cmd).command = &fbcp::Q_EVERYTHING_OFF;
        }
        else
        {
          @VAR(cmd).command = &fbcp::Q_EVERYTHING_ON;
        }
        Serial.print(F("Sent: "));
        fbcp::string s = fbcp::writeCommand(@VAR(cmd));
        Serial.println(s.c_str());
        sockOut.flush();
        sockOut.print(s.c_str());

        @CALL(read_command;&sockOut;&@VAR(cmd);fbcp::SOFT_TIMEOUT):understood;
        if (understood == READ_SUCCESS)
        {
          @VAR(last) = millis();
          if (@VAR(cmd).command->code == fbcp::A_ACCEPT.code)
          {
            Serial.println(F("Command was accepted"));
          }
          else if (@VAR(cmd).command->code == fbcp::A_REFUSE.code)
          {
            Serial.println(F("Command was refused"));
          }
          else if (@VAR(cmd).command->code == fbcp::A_ERROR.code)
          {
            Serial.println(F("Server didn't understand command"));
          }
          else
          {
            Serial.println(F("Server answered something strange"));
          }
        }
      }

      //Direction
      unsigned int t = millis();
      int dt;
      if (INTER_READ_INTERVAL > t-@VAR(t))
      {
        dt = @VAR(t)+INTER_READ_INTERVAL-t;
      }
      else
      {
        dt = 0;
      }
      @CALL(wait;dt):null;
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
      if (horizontalValue < lThreshold)
      {
        lOut = HIGH;
      }
      else if (horizontalValue > rThreshold)
      {
        rOut = HIGH;
      }

      if (verticalValue < dThreshold)
      {
        dOut = HIGH;
      }
      else if (verticalValue > uThreshold)
      {
        uOut = HIGH;
      }

      Serial.print(F("Horizontal value: "));
      Serial.print(horizontalValue);
      Serial.print(F(" | "));
      Serial.println(lOut?F("LEFT"):rOut?F("RIGHT"):F("CENTRE"));
      Serial.print(F("Vertical value: "));
      Serial.print(verticalValue);
      Serial.print(F(" | "));
      Serial.println(uOut?F("UP"):dOut?F("DOWN"):F("CENTRE"));

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
        dOut != dOut_old
      )
      {
        sendStatus = SEND_NEW;
      }

      @IF (sendStatus == SEND_INIT || sendStatus == SEND_FAIL || sendStatus == SEND_NEW)
      {
        Serial.print(F("Sent: "));
        fbcp::string s = fbcp::writeCommand(@VAR(cmd));
        Serial.println(s.c_str());
        sockOut.flush();
        sockOut.print(s.c_str());

        @CALL(read_command;&sockOut;&@VAR(cmd);fbcp::SOFT_TIMEOUT):understood;
        if (understood == READ_SUCCESS)
        {
          @VAR(last) = millis();
          if (@VAR(cmd).command->code == fbcp::A_ACCEPT.code)
          {
            Serial.println(F("Command was accepted"));
            sendStatus = SEND_OK;
          }
          else if (@VAR(cmd).command->code == fbcp::A_REFUSE.code)
          {
            Serial.println(F("Command was refused"));
            sendStatus = SEND_BUSY;
          }
          else if (@VAR(cmd).command->code == fbcp::A_ERROR.code)
          {
            Serial.println(F("Server didn't understand command"));
            sendStatus = SEND_FAIL;
          }
          else
          {
            Serial.println(F("Server answered something strange"));
            sendStatus = SEND_FAIL;
          }
        }
        else
        {
          sockOut.stop();
          sendStatus = SEND_INIT;
          @BREAK
        }
      }
      
      @IF (millis() - @VAR(last) > fbcp::HARD_TIMEOUT/2)
      {
        Serial.print(F("Sent: "));
        Serial.println(kalCmd.c_str());
        sockOut.flush();
        sockOut.print(kalCmd.c_str());
        @CALL(read_command;&sockOut;&@VAR(cmd);fbcp::SOFT_TIMEOUT):understood;
        if (understood == READ_SUCCESS)
        {
          @VAR(last) = millis();
        }
        else if (understood == READ_TIMEOUT)
        {
          sockOut.stop();
          sendStatus = SEND_INIT;
          @BREAK
        }
      }

      @IF (sendStatus == SEND_BUSY)
      {
        @CALL(wait;WAIT_AFTER_REFUSED):null;
        sendStatus = SEND_OK;
      }
    }

    if (mode == MODE_GAME || mode == MODE_STANDALONE)
    {
      Serial.println(F("Disconnected"));
      mode = MODE_IDLE;
    }
  }
}

@JOB (job_link)
{
  @MEMORY
  {
    @VAR(on:bool)
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
          ledFreq = FREQ_ON;
          break;
        default:
          ledFreq = FREQ_OFF;          
      }
    }
    
    if (ledFreq == FREQ_ON)
    {
      digitalWrite(ledPin, HIGH);
      @CONTINUE
    }
    if (ledFreq == FREQ_OFF)
    {
      digitalWrite(ledPin, LOW);
      @CONTINUE
    }
    
    @CALL(wait;1000/ledFreq):null;
    @VAR(on) = !@VAR(on);
    digitalWrite(ledPin, @VAR(on));
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

  Serial.println(F("Starting"));

  // Initialise pins
  pinMode(selPin, OUTPUT);
  pinMode(ledPin, OUTPUT);
  pinMode(btnPin, INPUT);
  
  // FBCP
  Serial.println(F("FBCP"));
  fbcp::serial = fbcp::CONTR_PREFIX;
  fbcp::serial += serial;
  mode = MODE_IDLE;
  fbcp::COMMAND_LINE cmd;
  cmd.command = &fbcp::Q_HEARTBEAT;
  kalCmd = fbcp::writeCommand(cmd);
  
  //ScheMo
  Serial.println(F("ScheMo"));
  @INIT

  //WiFi
  WiFi.persistent(false);

  ledFreq = FREQ_OFF;
  @SCHEDULE_ALL
  Serial.println(F("Job scheduled"));

  schemo::start_cycle();
}

void loop()
{
}
