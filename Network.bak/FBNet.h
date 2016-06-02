#ifndef FBNET_H
#define FBNET_H

#include "fbcp.h"

#ifdef ESP8266
  #include <ESP8266WiFi.h>
#else
  #include "wrapper/ESP8266WiFi.h"
#endif

namespace FBNet
{  
  const IPAddress GATEWAY (192,168,1,1);
  const IPAddress NETMASK (255,255,255,0);
  const int       PORT = 10000;

  class Network
  {
    public:
      Network ();
      ~Network ();
      bool start (const char*);
      bool tryConnect (const char*);
      const fbcp::string& getSSID () const;
      bool isConnected () const;
      bool findBetter (fbcp::string&) const;
    private:
      fbcp::string SSID;
      bool connected;
  };
  
  class Connection
  {
    public:
      
    private:
  };
  
  void manage ();
}

#endif // FBNET_H
