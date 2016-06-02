#include "FBNet.h"

FBNet::Network::Network () : SSID(), connected(false) {}

FBNet::Network::~Network () {}

bool FBNet::Network::start (const char* ssid)
{
  WiFi.softAP(ssid);
  WiFi.softAPConfig(GATEWAY,NETMASK,GATEWAY);
  return true;
}

bool FBNet::Network::tryConnect (const char* ssid)
{
  WiFi.begin(ssid);
  while (WiFi.status() != WL_CONNECTED)
  {
    delay(500);
  }
  WiFiClient client;
  if (!client.connect(GATEWAY, PORT)) return false;
  
  fbcp::COMMAND_LINE cmd;
  cmd.command = &fbcp::Q_SINGLE_PRESENTATION;
  cmd.params["serial"] = fbcp::SERIAL;
  client.print(fbcp::writeCommand(cmd).c_str());
  
}

const fbcp::string& FBNet::Network::getSSID () const
{
  return SSID;
}

bool FBNet::Network::isConnected () const
{
  return connected;
}

bool FBNet::Network::findBetter (fbcp::string&) const
{
}
      
      
void FBNet::manage()
{
  // Scan
  // Order by best
  // While better
  //  Try connect
  // 
}
