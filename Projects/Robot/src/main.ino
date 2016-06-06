#include "schemo.h"
#include "fbcp.common.h"
#include "FBNet.h"	

#include <ESP8266WiFi.h>

const char L_MOTOR = 16;
const char R_MOTOR = 14;
const fbcp::string fbcp::SERIAL;
const char serial[] = "1";
char ssid[100];
bool AP = true;

const IPAddress host(192,168,1,1);
const IPAddress gateway(192,168,1,1);
const IPAddress submask(255,255,255,0);
const unsigned int port = 10000;

WiFiClient sockIn;
WiFiClient sockOut;
WiFiServer server(port);

@DECLARE

@JOB (job_network)
{
  /////////////////
  // WiFi discovery:
  //WiFi.mode(WIFI_STA);
  //WiFi.disconnect();  
  Serial.println("Scan start");

  int n = WiFi.scanNetworks();
  Serial.println("Scan done");
  if (n == 0)
    Serial.println("No networks found");
  else
  {
    Serial.print(n);
    Serial.println(" networks found");
    for (int i = 0; i < n; ++i)
    {
      // Print SSID and RSSI for each network found
      Serial.print(i + 1);
      Serial.print(": ");
      ssid = WiFi.SSID(i).c_str();
      Serial.print(ssid.c_str());
      Serial.print(" (");
      Serial.print(WiFi.RSSI(i));
      Serial.print(")");
      bool noenc = (WiFi.encryptionType(i) == ENC_TYPE_NONE);
      Serial.println(noenc?" ":"*");
      // Search for suitable network
      if (noenc && startsWith(ssid, ssid_prefix))
      {
        Serial.println("Found!");
        AP = false;
        break;
      }
    }
  }
  Serial.println("");
  /////////////////
  
  if (AP)
  { // Create network
    sprintf(ssid, "%s%05d", ssid_prefix, serial);
    Serial.print("Starting AccessPoint: ");
    Serial.println(ssid);
    WiFi.softAP(ssid);
    WiFi.softAPConfig(host, gateway, submask);
    Serial.print("IP address: ");
    Serial.println(WiFi.softAPIP());
  }
  else
  { // Connect to network
    Serial.print("Connecting to ");
    Serial.println(ssid);
    
    WiFi.begin(ssid);
    while (WiFi.status() != WL_CONNECTED) {
      delay(500);
      Serial.print(".");
    }
    Serial.println("");
    Serial.println("WiFi connected");  
    Serial.print("IP address: ");
    Serial.println(WiFi.localIP());
  }

  // Start server
  Serial.print("Starting server on port ");
  Serial.println(port);
  server.begin();
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
  fbcp::serial = fbcp::ROBOT_PREFIX + serial;
  
  //ScheMo
  @INIT
  
  @SCHEDULE_ALL
  
  schemo::start_cycle();
}

void loop()
{
  
}
