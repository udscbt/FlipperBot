#ifndef ESP8266WIFI_H
#define ESP8266WIFI_H

#include <string>

class IPAddress
{
  public:
    IPAddress (int,int,int,int);
  private:
      
};


class WiFiClient
{
  public:
    bool operator! ();
    IPAddress remoteIP ();
    int remotePort ();
    bool available ();
    void stop ();
    int read (char*,int);
    bool connect (IPAddress,int);
    void print (std::string);
  private:
};

class WiFiServer
{
  public:
    WiFiServer (int);
    void begin ();
    WiFiClient available ();
  private:
    
};

typedef enum
{
  
  ENC_TYPE_TKIP,
  ENC_TYPE_WEP,
  ENC_TYPE_CCMP,
  ENC_TYPE_NONE,
  ENC_TYPE_AUTO
} ENC_TYPE;

typedef enum
{
  WL_CONNECTED,
  WL_NO_SHIELD,
  WL_IDLE_STATUS,
  WL_NO_SSID_AVAIL,
  WL_SCAN_COMPLETED,
  WL_CONNECT_FAILED,
  WL_CONNECTION_LOST,
  WL_DISCONNECTED
} WL_STATUS;

class WiFi_class
{
  public:
    WiFi_class();
    static int scanNetworks ();
    static std::string SSID (int);
    static std::string RSSI (int);
    static ENC_TYPE encryptionType (int);
    static void softAP (std::string);
    static void softAPConfig (IPAddress,IPAddress,IPAddress);
    static WL_STATUS begin (std::string);
    static WL_STATUS status ();
  private:
    
};

extern WiFi_class WiFi;

void delay(int);
long int millis();

#endif
