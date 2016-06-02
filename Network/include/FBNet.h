#ifndef FBNET_H
#define FBNET_H

#include "fbcp.h"

namespace FBNet
{
  
  typedef unsigned char IPAddress[4];  
  
  extern const IPAddress GATEWAY;
  extern const IPAddress NETMASK;
  extern const int       PORT;
}

#endif // FBNET_H
