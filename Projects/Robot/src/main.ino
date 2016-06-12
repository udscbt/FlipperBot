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
  
  @RETURN(fbcp::parseCommand(@VAR(msg), *@PARAM(cmd)));
}

@FUNCTION(tryConnect)
@PARAM(ssid:fbcp::string)
@RETURN(bool)
{
  @MEMORY
  {
    @VAR(t1:unsigned long)
    @VAR(t2:unsigned long)
    
    @VAR(cmd:fbcp::COMMAND_LINE)
    
    @VAR(i:int)
    @VAR(nc:int)
    @VAR(c:char)
    @VAR(end:bool)
    @VAR(msg:fbcp::string)
  }
  
  // Connect to network
  Serial.print("Connecting to ");
  Serial.println(@PARAM(ssid).c_str());
  
  WiFi.begin(@PARAM(ssid).c_str());
  @VAR(t1) = millis();
  @WHILE (WiFi.status() != WL_CONNECTED && millis() - @VAR(t1) < 10000) {
    @VAR(t2) = millis();
    @WHILE (millis() - @VAR(t2) < 500) {}
    Serial.print(".");
  }

  
  Serial.println("");
  @IF (WiFi.status() != WL_CONNECTED)
  {
    Serial.println("Timeout");
    @RETURN(false);
  }
  Serial.println("WiFi connected");  
  Serial.print("IP address: ");
  Serial.println(WiFi.localIP());
  
  @TBREAK
  
  @IF (!sockOut.connect(gateway, FBNet::PORT))
  {
    Serial.println("Can't connect to server");
    @RETURN(false);
  }
  
  @VAR(cmd).command = &fbcp::Q_SINGLE_PRESENTATION;
  @VAR(cmd).params["serial"] = fbcp::serial;
  sockOut.print(fbcp::writeCommand(@VAR(cmd)).c_str());
  
  @CALL(readCommand;&sockOut;&@VAR(cmd);2000):understood;
  if (!understood)
  {
    Serial.println("Couldn't understand server response");
    @RETURN(false);
  }
  @IF (@VAR(cmd).command->code == fbcp::A_GRANT_ACCESS.code)
  {
    Serial.println("Server allowed connection");
    @RETURN(true);
  }
  @IF (@VAR(cmd).command->code == fbcp::A_DENY_ACCESS.code)
  {
    Serial.println("Server refused connection");
    @RETURN(false);
  }
  Serial.println("Server answered something strange :S");
  @RETURN(false);
}

@JOB (job_network)
{
  @MEMORY
  {
    @VAR(i:int)
    @VAR(n:int)
    @VAR(connected:bool)
  }

  /////////////////
  // WiFi discovery:
  //WiFi.mode(WIFI_STA);
  //WiFi.disconnect();  
  Serial.println("Scan start");

  @VAR(n) = WiFi.scanNetworks();
  Serial.println("Scan done");
  @IF (@VAR(n) == 0)
  {
    Serial.println("No networks found");
  }
  @IF (@VAR(n) != 0)
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
      @IF (noenc && ssid.startsWith(fbcp::BOARD_PREFIX))
      {
        Serial.println("Found suitable network");
        @CALL(tryConnect;ssid):connected;
        @VAR(connected) = connected;
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
      }
      
      ++@VAR(i);
    }
  }
  Serial.println("");
  /////////////////
  
  @IF (!@VAR(connected))
  { // Create network
    ssid = fbcp::serial;
    Serial.print("Starting AccessPoint: ");
    Serial.println(ssid);
    WiFi.softAP(ssid);
    WiFi.softAPConfig(host, gateway, submask);
    Serial.print("IP address: ");
    Serial.println(WiFi.softAPIP());
    mode = STANDALONE;
    Serial.println("Getting into Stand Alone mode");
  }

  // Start server
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

@JOB(job_server)
{
  @MEMORY
  {
    @VAR(cmd:fbcp::COMMAND_LINE)
  }

  @WHILE (mode == IDLE) {}
  sockIn = server.available();
  @WHILE (sockIn.connected())
  {
    @CALL(readCommand;&sockIn;&@VAR(cmd);2000):understood;
    if (understood)
    {
    Serial.print("Received: ");
    Serial.println(fbcp::writeCommand(@VAR(cmd)).c_str());
    if (@VAR(cmd).command->code == fbcp::Q_ROBOT_COMMAND.code)
    {
      if (@VAR(cmd).params["direction"] == fbcp::DIRECTION_FORWARD.str)
      {
        leftMotor(1);
        rightMotor(1);
      }
      else if (@VAR(cmd).params["direction"] == fbcp::DIRECTION_BACKWARD.str)
      {
        leftMotor(-1);
        rightMotor(-1);
      }
      else if (@VAR(cmd).params["direction"] == fbcp::DIRECTION_STOP.str)
      {
        leftMotor(0);
        rightMotor(0);
      }
      else if (@VAR(cmd).params["direction"] == fbcp::DIRECTION_FORWARD_LEFT.str)
      {
        leftMotor(0);
        rightMotor(1);
      }
      else if (@VAR(cmd).params["direction"] == fbcp::DIRECTION_BACKWARD_LEFT.str)
      {
        leftMotor(0);
        rightMotor(-1);
      }
      else if (@VAR(cmd).params["direction"] == fbcp::DIRECTION_LEFT.str)
      {
        leftMotor(-1);
        rightMotor(1);
      }
      else if (@VAR(cmd).params["direction"] == fbcp::DIRECTION_FORWARD_RIGHT.str)
      {
        leftMotor(1);
        rightMotor(0);
      }
      else if (@VAR(cmd).params["direction"] == fbcp::DIRECTION_BACKWARD_RIGHT.str)
      {
        leftMotor(-1);
        rightMotor(0);
      }
      else if (@VAR(cmd).params["direction"] == fbcp::DIRECTION_RIGHT.str)
      {
        leftMotor(1);
        rightMotor(-1);
      }
      else
      {
        understood = false;
      }
    }
    else if (@VAR(cmd).command->code == fbcp::Q_MOTOR_COMMAND.code)
    {
      int dir;
      if (@VAR(cmd).params["direction"] == fbcp::DIRECTION_FORWARD.str)
      {
        dir = 1;
      }
      else if (@VAR(cmd).params["direction"] == fbcp::DIRECTION_RIGHT.str)
      {
        dir = -1;
      }
      else if (@VAR(cmd).params["direction"] == fbcp::DIRECTION_RIGHT.str)
      {
        dir = 0;
      }
      else
      {
        understood = false;
      }
      
      if (understood)
      {
        if (@VAR(cmd).params["motor"] == fbcp::MOTOR_LEFT.str)
        {
          leftMotor(dir);
        }
        else if (@VAR(cmd).params["motor"] == fbcp::MOTOR_RIGHT.str)
        {
          rightMotor(dir);
        }
        else if (@VAR(cmd).params["motor"] == fbcp::MOTOR_BOTH.str)
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
    @VAR(cmd).command = &fbcp::A_ACCEPT;
    @VAR(cmd).other = "";
  }
  else
  {
    @VAR(cmd).command = &fbcp::A_ERROR;
    @VAR(cmd).other = "";
  }
  Serial.print("Sent: ");
  fbcp::string s = fbcp::writeCommand(@VAR(cmd));
  Serial.println(s.c_str());
  sockIn.print(s.c_str());
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
  
  @WHILE (sockIn.connected())
  {
    @VAR(read) = digitalRead(HIT);
    @VAR(end) = false;
    @IF (@VAR(read))
    {
      @WHILE (!@VAR(end) && millis() - @VAR(t) < 2000)
      {
        @VAR(cmd).command = &fbcp::Q_HIT;
        @VAR(cmd).other = "";
        
        Serial.print("Sent: ");
        fbcp::string s = fbcp::writeCommand(@VAR(cmd));
        Serial.println(s.c_str());
        sockOut.print(s.c_str());

        @CALL(readCommand;&sockOut;&@VAR(cmd);2000):understood;
        if (understood && @VAR(cmd).command->code == fbcp::A_ACCEPT.code)
        {
          @VAR(end) = true;
        }
        else
        {
          Serial.println("Resending...");
        }
      }
    }
  }

  mode = IDLE;
}

void setup()
{
  // Initialise serial communication
  Serial.begin(115200);
  delay(10);

  // Initialise pins
  analogWriteRange(100);
  pinMode(R_MOTOR, OUTPUT);
  analogWrite(R_MOTOR, 0);
  pinMode(L_MOTOR, OUTPUT);
  analogWrite(L_MOTOR, 0);
  
  // FBCP
  fbcp::serial = fbcp::ROBOT_PREFIX;
  fbcp::serial += serial;
  mode = IDLE;
  
  //ScheMo
  @INIT
  
  schemo::schedule_job(job_network);
  
  schemo::start_cycle();
}

void loop()
{
  
}
