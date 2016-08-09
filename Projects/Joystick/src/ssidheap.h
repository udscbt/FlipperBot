const byte SSID_min_NUM = 50;

byte ssidHeap[SSID_min_NUM];
byte ssidNum;

void clearSsidHeap()
{
  ssidNum = 0;
}

bool ssidHeapRule(byte i, byte j)
{
  fbcp::string s1 = WiFi.SSID(ssidHeap[i]).c_str();
  fbcp::string s2 = WiFi.SSID(ssidHeap[j]).c_str();
  return s1 < s2;
}

bool updateSsidHeap(byte i)
{
  if (i > ssidNum) return false;
  
  byte l = i<<1;
  byte r = l+1;
  byte min = i;
  if (l < ssidNum && ssidHeapRule(l, min))
  {
    min = l;
  }
  if (r < ssidNum && ssidHeapRule(r, min))
  {
    min = r;
  }
  if (min != i)
  {
    ssidHeap[i] = ssidHeap[i] ^ ssidHeap[min];
    ssidHeap[min] = ssidHeap[i] ^ ssidHeap[min];
    ssidHeap[i] = ssidHeap[i] ^ ssidHeap[min];
    updateSsidHeap(min);
  }
  return true;
}

bool insertSsid(byte ssid)
{
  if (ssidNum == SSID_min_NUM) return false;

  byte i = ssidNum++;
  byte j;
  ssidHeap[i] = ssid;
  while (i >  0 && ssidHeapRule(i, j=(i>>1)))
  {
    ssidHeap[i] = ssidHeap[i] ^ ssidHeap[j];
    ssidHeap[j] = ssidHeap[i] ^ ssidHeap[j];
    ssidHeap[i] = ssidHeap[i] ^ ssidHeap[j];
    i = j;
  }
  
  return true;
}

byte popSsid()
{
  if (ssidNum < 1) return -1;
  
  byte min = ssidHeap[0];
  
  ssidHeap[0] = ssidHeap[--ssidNum];
  updateSsidHeap(0);
  
  return min;
}
