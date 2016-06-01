#include "FBNet.h"

FBNet::Network::Network () : SSID(), connected(false)
{}

FBNet::Network::~Network ()
{}

bool FBNet::Network::start (const char* ssid)
{}

bool FBNet::Network::tryConnect (const char* ssid)
{}

const fbcp::string& FBNet::Network::getSSID () const
{
  return SSID;
}

bool FBNet::Network::isConnected () const
{
  return connected;
}
      
      
void FBNet::manage()
{
  // Scan
  // Order by best
  // While better
  //  Try connect
  // 
}
