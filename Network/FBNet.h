#ifndef FBNET_H
#define FBNET_H

#include "fbcp.h"

namespace FBNet
{
  const char BOARD_PREFIX[] = "FlipperBot-Board-";
  const char ROBOT_PREFIX[] = "FlipperBot-Robot-";

  class Network
  {
    public:
      Network ();
      ~Network ();
      bool start (const char*);
      bool tryConnect (const char*);
      const fbcp::string& getSSID () const;
      bool isConnected () const;
    private:
      fbcp::string SSID;
      bool connected;
  };
  
  void manage ();
}

#endif // FBNET_H
