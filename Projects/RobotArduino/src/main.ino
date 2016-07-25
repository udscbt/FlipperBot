#include "schemo.h"
#include "fbcp.common.h"
#include "FBNet.h"

#include "WiFiEsp.h"

// Emulate Serial1 on pins 7/6 if not present
#ifndef HAVE_HWSERIAL1
#include "SoftwareSerial.h"
SoftwareSerial Serial1(6, 7); // RX, TX
#endif

const char L_MOTOR = 8;
const char R_MOTOR = 9;
const char HIT     = 10;
const char BUZ     = 11; //GREEN
const char LED     = 13; //RED

enum
{
  SEND_INIT,
  SEND_BUSY,
  SEND_FAIL,
  SEND_NEW,
  SEND_OK
} sendStatus = SEND_INIT;

const int WAIT_AFTER_REFUSED  = 1000;

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

WiFiEspClient sockOut;
WiFiEspServer server(FBNet::PORT);

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

bool hit = false;
fbcp::string hitCmd;

@DECLARE

@FUNCTION(readCommand)
@PARAM(sock:WiFiEspClient*)
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
  
  Serial.print("Received: '");
  Serial.print(@VAR(msg).c_str());
  Serial.println("'");
  
  @IF (millis() - @VAR(t) >= @PARAM(timeout))
  {
    Serial.println("Timeout");
    @RETURN(false);
  }
  
  @IF (@VAR(c) == '\0')
  {
    Serial.println("Remote host said something REALLY strange :S");
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
    
    @VAR(cmd:fbcp::COMMAND_LINE)
  }

  @WHILE
  {
    @IF (mode == MODE_IDLE)
    {
      /////////////////
      // WiFi discovery:
      //WiFi.mode(WIFI_STA);
      //WiFi.disconnect();  
      Serial.println("Scan start");
      mode = MODE_SCAN;

      WiFi.scanNetworks();
      Serial.println("Scan done");
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
          ssid = WiFi.SSID(@VAR(i));
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
                status = WiFi.begin(ssid.c_str(), "");
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
                case WL_CONNECT_FAILED:
                  Serial.println("Connection failed for all the attempts");
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
              //~ schemo::schedule_job(job_client);
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
        Serial.print("Starting AccessPoint: ");
        ssid = fbcp::serial;
        Serial.println(ssid.c_str());
        WiFi.softAP(ssid.c_str());
        WiFi.softAPConfig(host, gateway, submask);
        Serial.print("IP address: ");
        Serial.println(WiFi.softAPIP());
        mode = MODE_STANDALONE;
      }
    }
  }
}

//~ @JOB(job_client)
//~ {
  //~ @MEMORY
  //~ {
    //~ @VAR(cmd:fbcp::COMMAND_LINE)
  //~ }

  //~ @WHILE
  //~ {
    //~ @WHILE (sockOut.connected())
    //~ {
      //~ @VAR(cmd).command = &fbcp::Q_ROBOT_COMMAND;

      //~ if (digitalRead(HIT))
      //~ {
        //~ if (!hit)
        //~ {
          //~ hit = true;
          //~ sendStatus = SEND_NEW;
        //~ }
      //~ }
      //~ else
      //~ {
        //~ hit = false;
      //~ }

      //~ @IF (sendStatus == SEND_FAIL || sendStatus == SEND_NEW)
      //~ {
        //~ Serial.print("Sent: ");
        //~ Serial.println(hitCmd.c_str());
        //~ sockOut.print(hitCmd.c_str());

        //~ @CALL(readCommand;&sockOut;&@VAR(cmd);1000):understood;
        //~ if (understood)
        //~ {
          //~ if (@VAR(cmd).command->code == fbcp::A_ACCEPT.code)
          //~ {
            //~ Serial.println("Command was accepted");
            //~ sendStatus = SEND_OK;
          //~ }
          //~ else if (@VAR(cmd).command->code == fbcp::A_REFUSE.code)
          //~ {
            //~ Serial.println("Command was refused");
            //~ sendStatus = SEND_BUSY;
          //~ }
          //~ else if (@VAR(cmd).command->code == fbcp::A_ERROR.code)
          //~ {
            //~ Serial.println("Server didn't understand command");
            //~ sendStatus = SEND_FAIL;
          //~ }
          //~ else
          //~ {
            //~ Serial.println("Server answered something strange");
            //~ sendStatus = SEND_FAIL;
          //~ }
        //~ }
        //~ else
        //~ {
          //~ sockOut.stop();
          //~ sendStatus = SEND_FAIL;
        //~ }
      //~ }

      //~ @IF (sendStatus == SEND_BUSY)
      //~ {
        //~ @CALL(wait;WAIT_AFTER_REFUSED):null;
        //~ sendStatus = SEND_OK;
      //~ }
    //~ }

    //~ if (mode != MODE_IDLE && !sockOut.connected())
    //~ {
      //~ Serial.println("Disconnected");
      //~ mode = MODE_IDLE;
    //~ }
  //~ }
//~ }

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
    analogWrite(R_MOTOR, 0);
  }
}

@JOB(job_server)
{
  @MEMORY
  {
    @VAR(client:WiFiEspClient)
    @VAR(cmd:fbcp::COMMAND_LINE)
  }
  server.begin();

  @WHILE
  {
    @IF (mode == MODE_GAME)
    {
      @WHILE (mode == MODE_GAME)
      {
        @VAR(client) = server.available();
        @IF (@VAR(client))
        {
          @WHILE (@VAR(client).connected())
          {
            @CALL(readCommand;&@VAR(client);&@VAR(cmd);1000):understood;
            fbcp::COMMAND_LINE cmd;
            cmd.command = &fbcp::A_ACCEPT;
            if (!understood)
            {
              cmd.command = &fbcp::A_ERROR;
              Serial.println("Couldn't understand message");
            }
            else if (@VAR(cmd).command->code == fbcp::Q_ROBOT_COMMAND.code)
            {
              Serial.println("Change direction");
              if (@VAR(cmd).params["direction"] == fbcp::DIRECTION_FORWARD_LEFT.str)
              {
                leftMotor(0);
                rightMotor(1);
              }
              else if (@VAR(cmd).params["direction"] == fbcp::DIRECTION_FORWARD.str)
              {
                leftMotor(1);
                rightMotor(1);
              }
              else if (@VAR(cmd).params["direction"] == fbcp::DIRECTION_FORWARD_RIGHT.str)
              {
                leftMotor(1);
                rightMotor(0);
              }
              else if (@VAR(cmd).params["direction"] == fbcp::DIRECTION_LEFT.str)
              {
                leftMotor(-1);
                rightMotor(1);
              }
              else if (@VAR(cmd).params["direction"] == fbcp::DIRECTION_STOP.str)
              {
                leftMotor(0);
                rightMotor(0);
              }
              else if (@VAR(cmd).params["direction"] == fbcp::DIRECTION_RIGHT.str)
              {
                leftMotor(1);
                rightMotor(-1);
              }
              else if (@VAR(cmd).params["direction"] == fbcp::DIRECTION_BACKWARD_LEFT.str)
              {
                leftMotor(0);
                rightMotor(-1);
              }
              else if (@VAR(cmd).params["direction"] == fbcp::DIRECTION_BACKWARD.str)
              {
                leftMotor(-1);
                rightMotor(-1);
              }
              else if (@VAR(cmd).params["direction"] == fbcp::DIRECTION_BACKWARD_RIGHT.str)
              {
                leftMotor(-1);
                rightMotor(0);
              }
            }
            else if (@VAR(cmd).command->code == fbcp::Q_MOTOR_COMMAND.code)
            {
              Serial.println("Change motor speed");
              bool left = false;
              bool right = false;
              if (@VAR(cmd).params["motor"] == fbcp::MOTOR_LEFT.str) left = true;
              if (@VAR(cmd).params["motor"] == fbcp::MOTOR_RIGHT.str) right = true;
              if (@VAR(cmd).params["motor"] == fbcp::MOTOR_BOTH.str) {left = true; right=true;}
              if (@VAR(cmd).params["direction"] == fbcp::DIRECTION_FORWARD.str)
              {
                if (left) leftMotor(1);
                if (right) rightMotor(1);
              }
              else if (@VAR(cmd).params["direction"] == fbcp::DIRECTION_STOP.str)
              {
                if (left) leftMotor(0);
                if (right) rightMotor(0);
              }
              else if (@VAR(cmd).params["direction"] == fbcp::DIRECTION_BACKWARD.str)
              {
                if (left) leftMotor(-1);
                if (right) rightMotor(-1);
              }
            }
            else
            {
              cmd.command = &fbcp::A_ERROR;
              Serial.println("Board said something strange :S");
            }
            fbcp::string s = fbcp::writeCommand(cmd);
            Serial.print("Sent: ");
            Serial.println(s.c_str());
            @VAR(client).print(s.c_str());
          }
          Serial.println("Disconnected");
        }
      }
      
      if (@VAR(client))
      {
        @VAR(client).stop();
      }
    }
    @IF (mode == MODE_STANDALONE)
    {
    }
  }
}

@JOB (job_link)
{
  @MEMORY
  {
    @VAR(led:bool)
    @VAR(buz:bool)
  }

  @VAR(led) = false;
  @VAR(buz) = false;

  @WHILE
  {
    ledFreq = FREQ_FAST;
    
    switch (mode)
    {
      case MODE_IDLE:
      case MODE_SCAN:
      case MODE_NETCON:
      case MODE_SERCON:
      case MODE_STANDALONE:
        @VAR(led) = true;
        break;
      case MODE_GAME:
        @VAR(led) = !@VAR(led);
        break;
      default:
        @VAR(led) = false;       
    }

    switch (mode)
    {
      case MODE_IDLE:
        @VAR(buz) = false;
        break;
      case MODE_SCAN:
        @VAR(buz) = !@VAR(buz);
        ledFreq = FREQ_SLOW;
        break;
      case MODE_NETCON:
        @VAR(buz) = !@VAR(buz);
        ledFreq = FREQ_FAST;
        break;
      case MODE_SERCON:
        @VAR(buz) = !@VAR(buz);
        ledFreq = FREQ_FAST;
        break;
      case MODE_STANDALONE:
        @VAR(buz) = true;
        break;
      case MODE_GAME:
        @VAR(buz) = !@VAR(led);
        ledFreq = FREQ_SLOW;
        break;
      default:
        @VAR(buz) = false;       
    }

    digitalWrite(LED, @VAR(led));
    digitalWrite(BUZ, @VAR(buz));
    
    @CALL(wait;1000/ledFreq):null;
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
  Serial1.begin(9600);
  WiFi.init(&Serial1);
  delay(10);

  Serial.println("Starting");

  // Initialise pins
  pinMode(LED, OUTPUT);
  pinMode(BUZ, OUTPUT);
  pinMode(HIT, INPUT);

  analogWriteRange(100);
  pinMode(R_MOTOR, OUTPUT);
  analogWrite(R_MOTOR, 0);
  pinMode(L_MOTOR, OUTPUT);
  analogWrite(L_MOTOR, 0);
  pinMode(HIT, INPUT);
  
  // FBCP
  Serial.println("FBCP");
  fbcp::serial = fbcp::ROBOT_PREFIX;
  fbcp::serial += serial;
  mode = MODE_IDLE;
  fbcp::COMMAND_LINE cmd;
  cmd.command = &fbcp::Q_HIT;
  hitCmd = fbcp::writeCommand(cmd);
  
  //ScheMo
  Serial.println("ScheMo");
  @INIT

  //WiFi
  WiFi.persistent(false);

  schemo::schedule_job(job_link);
  schemo::schedule_job(job_network);
  schemo::schedule_job(job_server);
  schemo::schedule_job(avoid_breaking);
  Serial.println("Job scheduled");

  schemo::start_cycle();
}

void loop()
{
}
