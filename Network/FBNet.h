#include <ESP8266WiFi.h>

namespace FlipperBot
{
  bool net_find(const char* pattern, char* ssid);
  bool net_connect(const char* ssid);
  WiFiClient host_connect(const char* host, int port);
}
