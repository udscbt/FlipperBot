#include "schemo.h"
#include "fbcp.common.h"
#include "FBNet.h"
#include "motors.h"

#include <ESP8266WiFi.h>

typedef unsigned char byte;

const unsigned char L_MOTOR = 14;
const unsigned char R_MOTOR = 16;
const unsigned char HIT     = 05;
const unsigned char BUZ     = 04; //GREEN
//const unsigned char LED     = 13; //RED
const unsigned char LED     = 15; //RED

Motor leftMotor(Motor::RIGHT | Motor::NEW | Motor::pin(L_MOTOR));
Motor rightMotor(Motor::RIGHT | Motor::NEW | Motor::pin(R_MOTOR));

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
WiFiServer server(FBNet::PORT);

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

enum
{
  FREQ_OFF  = -1,
  FREQ_ON   = 0,
  FREQ_SLOW = 2,
  FREQ_FAST  = 10
};

bool hit = false;
fbcp::string hitCmd;
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
    @VAR(i:int)
    @VAR(n:int)
    @VAR(connected:bool)

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
      Serial.println(F("Scan start"));
      mode = MODE_SCAN;
      @VAR(connected) = false;

      WiFi.scanNetworks(true);
      @WHILE ((@VAR(n) = WiFi.scanComplete()) == WIFI_SCAN_RUNNING) {}
      if (@VAR(n) == WIFI_SCAN_FAILED)
      {
        Serial.println(F("Scan failed"));
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
        Serial.println(F(" networks found"));
        @VAR(i) = -1;
        @VAR(connected) = false;
        @WHILE (++@VAR(i) < @VAR(n) && !@VAR(connected))
        {
          // Print SSID and RSSI for each network found
          Serial.print(@VAR(i) + 1);
          Serial.print(F(": "));
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
          }
          else
          {
            @CONTINUE
          }

          /*******************
           * tryConnect START *
           ********************/
          mode = MODE_NETCON;
          Serial.print(F("Connecting to "));
          Serial.println(ssid.c_str());
          
          @VAR(t1) = millis();

          @WHILE (status != WL_CONNECTED && millis() - @VAR(t1) < fbcp::HARD_TIMEOUT)
          {
            status = WiFi.begin(ssid.c_str());
            @CALL(wait;500):null;
            Serial.print(F("."));
          }
          Serial.println(F(""));

          @VAR(connected) = false;
          switch (status)
          {
            case WL_CONNECTED:
              Serial.println(F("WiFi connected"));
              @VAR(connected) = true;
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
          }
          status = WL_IDLE_STATUS;
          
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

          @VAR(connected) = sockOut.connect(gateway, FBNet::PORT);
            
          if (!@VAR(connected))
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
          sockOut.print(s.c_str());
          
          @CALL(read_command;&sockOut;&@VAR(cmd);fbcp::HARD_TIMEOUT):understood;
          @VAR(connected) = false;
          if (understood == READ_FAIL)
          {
            Serial.println(F("Couldn't understand server response"));
          }
          else if (understood = READ_TIMEOUT)
          {
            Serial.println(F("Server timed out"));
          }
          else if (@VAR(cmd).command->code == fbcp::A_GRANT_ACCESS.code)
          {
            Serial.println(F("Server allowed connection"));
            @VAR(connected) = true;
          }
          else if (@VAR(cmd).command->code == fbcp::A_DENY_ACCESS.code)
          {
            Serial.println(F("Server refused connection"));
          }
          else
          {
            Serial.println(F("Server answered something strange :S"));
          }
          /*******************
           * tryConnect STOP  *
           ********************/
  
          if (@VAR(connected))
          {
            Serial.println(F("Connection estabilished"));
            mode = MODE_GAME;
          }
          else
          {
            Serial.println(F("Connection failed"));
          }
        }
      }
      Serial.println(F(""));
      /////////////////

      if (!@VAR(connected))
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

@JOB(job_client)
{
  @MEMORY
  {
    @VAR(cmd:fbcp::COMMAND_LINE)
    @VAR(t:unsigned long)
  }
  
  @WHILE
  {
    @VAR(t) = millis();
    @WHILE (mode == MODE_GAME && sockOut.connected())
    {
      @VAR(cmd).command = &fbcp::Q_ROBOT_COMMAND;

      if (digitalRead(HIT))
      {
        if (!hit)
        {
          hit = true;
          sendStatus = SEND_NEW;
        }
      }
      else
      {
        hit = false;
      }
      
      @IF (sendStatus == SEND_BUSY || sendStatus == SEND_NEW)
      {
        Serial.print("Sent: ");
        Serial.println(hitCmd.c_str());
        sockOut.print(hitCmd.c_str());

        @CALL(read_command;&sockOut;&@VAR(cmd);fbcp::SOFT_TIMEOUT):understood;
        if (understood == READ_SUCCESS)
        {
          @VAR(t) = millis();
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
        else if (understood == READ_FAIL)
        {
          Serial.println("Server answered something strange");
          sendStatus = SEND_FAIL;
        }
        else if (understood == READ_TIMEOUT)
        {
          sockOut.stop();
          sendStatus = SEND_FAIL;
        }
      }
      
      @IF (millis() - @VAR(t) > fbcp::HARD_TIMEOUT/2)
      {
        Serial.print("Sent: ");
        Serial.println(kalCmd.c_str());
        sockOut.print(kalCmd.c_str());
        @CALL(read_command;&sockOut;&@VAR(cmd);fbcp::SOFT_TIMEOUT):understood;
        if (understood == READ_SUCCESS)
        {
          @VAR(t) = millis();
        }
        else if (understood == READ_TIMEOUT)
        {
          sockOut.stop();
          sendStatus = SEND_FAIL;
          @BREAK
        }
      }
    }

    if (mode == MODE_GAME && !sockOut.connected())
    {
      Serial.println("Disconnected");
      mode = MODE_IDLE;
    }
  }
}

@JOB (job_server)
{
  @MEMORY
  {
    @VAR(client:WiFiClient)
    @VAR(cmd:fbcp::COMMAND_LINE)
    @VAR(controller:bool)
  }
  server.begin();
  @VAR(controller) = false;

  @WHILE
  {
    @WHILE (mode == MODE_STANDALONE && !@VAR(controller))
    {
      @VAR(client) = server.available();
      @IF (@VAR(client))
      {
        @WHILE(@VAR(client).connected() && !@VAR(controller))
        {
          @CALL(read_command;&@VAR(client);&@VAR(cmd);fbcp::HARD_TIMEOUT):understood;
          fbcp::COMMAND_LINE cmd;
          cmd.command = &fbcp::A_GRANT_ACCESS;
          if (understood == READ_FAIL)
          {
            cmd.command = &fbcp::A_ERROR;
            Serial.println(F("Couldn't understand message"));
          }
          else if (understood == READ_TIMEOUT)
          {
            cmd.command = NULL;
            Serial.println(F("Client timed out"));
          }
          else if (@VAR(cmd).command->code == fbcp::Q_SINGLE_PRESENTATION.code)
          {
            Serial.print(F("Controller connected: "));
            Serial.println(@VAR(cmd).params["serial"]);
            @VAR(controller) = true;
          }
          
          if (cmd.command == NULL)
          {
            Serial.println(F("Disconnecting"));
            @VAR(client).stop();
          }
          else
          {
            fbcp::string s = fbcp::writeCommand(cmd);
            Serial.print(F("Sent: "));
            Serial.println(s.c_str());
            @VAR(client).print(s.c_str());
          }
        }
      }
    }
    
    @IF (mode == MODE_GAME || (mode == MODE_STANDALONE && @VAR(controller)))
    {
      @IF (mode == MODE_GAME)
      {
        @VAR(client) = server.available();
      }
      @IF (@VAR(client))
      {
        @WHILE (@VAR(client).connected())
        {
          @CALL(read_command;&@VAR(client);&@VAR(cmd);fbcp::HARD_TIMEOUT):understood;
          fbcp::COMMAND_LINE cmd;
          cmd.command = &fbcp::A_ACCEPT;

          if (understood == READ_FAIL)
          {
            cmd.command = &fbcp::A_ERROR;
            Serial.println(F("Couldn't understand message"));
          }
          else if (understood == READ_TIMEOUT)
          {
            cmd.command = NULL;
            Serial.println(F("Client timed out"));
          }
          else if (fbcp::common::handleRequest(@VAR(cmd), cmd))
          {
            Serial.println("Library managed");
          }
          else if (@VAR(cmd).command->code == fbcp::Q_ROBOT_COMMAND.code)
          {
            Serial.println(F("Change direction"));
            if (@VAR(cmd).params["direction"] == fbcp::DIRECTION_FORWARD_LEFT.str)
            {
              leftMotor.value(0);
              rightMotor.value(1);
            }
            else if (@VAR(cmd).params["direction"] == fbcp::DIRECTION_FORWARD.str)
            {
              leftMotor.value(1);
              rightMotor.value(1);
            }
            else if (@VAR(cmd).params["direction"] == fbcp::DIRECTION_FORWARD_RIGHT.str)
            {
              leftMotor.value(1);
              rightMotor.value(0);
            }
            else if (@VAR(cmd).params["direction"] == fbcp::DIRECTION_LEFT.str)
            {
              leftMotor.value(-1);
              rightMotor.value(1);
            }
            else if (@VAR(cmd).params["direction"] == fbcp::DIRECTION_STOP.str)
            {
              leftMotor.value(0);
              rightMotor.value(0);
            }
            else if (@VAR(cmd).params["direction"] == fbcp::DIRECTION_RIGHT.str)
            {
              leftMotor.value(1);
              rightMotor.value(-1);
            }
            else if (@VAR(cmd).params["direction"] == fbcp::DIRECTION_BACKWARD_LEFT.str)
            {
              leftMotor.value(0);
              rightMotor.value(-1);
            }
            else if (@VAR(cmd).params["direction"] == fbcp::DIRECTION_BACKWARD.str)
            {
              leftMotor.value(-1);
              rightMotor.value(-1);
            }
            else if (@VAR(cmd).params["direction"] == fbcp::DIRECTION_BACKWARD_RIGHT.str)
            {
              leftMotor.value(-1);
              rightMotor.value(0);
            }
          }
          else if (@VAR(cmd).command->code == fbcp::Q_MOTOR_COMMAND.code)
          {
            Serial.println(F("Change motor speed"));
            bool left = false;
            bool right = false;
            if (@VAR(cmd).params["motor"] == fbcp::MOTOR_LEFT.str) left = true;
            if (@VAR(cmd).params["motor"] == fbcp::MOTOR_RIGHT.str) right = true;
            if (@VAR(cmd).params["motor"] == fbcp::MOTOR_BOTH.str) {left = true; right=true;}
            if (@VAR(cmd).params["direction"] == fbcp::DIRECTION_FORWARD.str)
            {
              if (left) leftMotor.value(1);
              if (right) rightMotor.value(1);
            }
            else if (@VAR(cmd).params["direction"] == fbcp::DIRECTION_STOP.str)
            {
              if (left) leftMotor.value(0);
              if (right) rightMotor.value(0);
            }
            else if (@VAR(cmd).params["direction"] == fbcp::DIRECTION_BACKWARD.str)
            {
              if (left) leftMotor.value(-1);
              if (right) rightMotor.value(-1);
            }
          }
          else if (@VAR(cmd).command->code == fbcp::Q_CLEAN.code)
          {
            cmd.command = &fbcp::A_CLEAN;
            Serial.println(F("Client requested disconnection"));
            @VAR(controller) = false;
          }
          else
          {
            cmd.command = &fbcp::A_ERROR;
            Serial.println(F("Client said something strange :S"));
          }

          if (cmd.command == NULL)
          {
            Serial.println(F("Disconnecting"));
            @VAR(client).stop();
            @BREAK
          }
          else
          {
            fbcp::string s = fbcp::writeCommand(cmd);
            Serial.print(F("Sent: "));
            Serial.println(s.c_str());
            @VAR(client).print(s.c_str());
            if (cmd.command->code == fbcp::A_CLEAN.code)
            {
              @VAR(client).stop();
            }
          }
        }
        Serial.println(F("Disconnected"));

        mode = MODE_IDLE;
      }
    }
  }
}

@JOB (job_link_and_bumper)
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
    if (digitalRead(HIT))
    {
      bool left = false;
      bool right = false;
      if (leftMotor.getDirection() == Motor::Direction::DIR_FORWARD)
        left = true;
      if (rightMotor.getDirection() == Motor::Direction::DIR_FORWARD)
        right = true;
      if (left)
      {
        if (right)
        {
          leftMotor.stop();
          rightMotor.stop();
        }
        else
        {
          rightMotor.backward();
        }
      }
      else
      {
        if (right)
        {
          leftMotor.backward();
        }
      }
    }
    
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
  delay(10);

  Serial.println(F("Starting"));

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
  Serial.println(F("FBCP"));
  fbcp::serial = fbcp::ROBOT_PREFIX;
  fbcp::serial += serial;
  mode = MODE_IDLE;
  fbcp::COMMAND_LINE cmd;
  cmd.command = &fbcp::Q_HIT;
  hitCmd = fbcp::writeCommand(cmd);
  cmd.command = &fbcp::Q_HEARTBEAT;
  kalCmd = fbcp::writeCommand(cmd);
  
  //ScheMo
  Serial.println(F("ScheMo"));
  @INIT

  //WiFi
  WiFi.persistent(false);

  //~ schemo::schedule_job(job_link);
  //~ schemo::schedule_job(job_network);
  //~ schemo::schedule_job(job_server);
  //~ schemo::schedule_job(job_client);
  //~ schemo::schedule_job(avoid_breaking);
  @SCHEDULE_ALL
  Serial.println(F("Job scheduled"));

  schemo::start_cycle();
}

void loop()
{
}
