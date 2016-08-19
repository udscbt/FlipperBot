if [ $# -lt 1 ]
then
  echo "Usage: ./dummynet.sh [start|stop|connect]"
  exit 1
fi

if [ $1 = "start" ]
then
  systemctl stop netctl*
  ifconfig wlan0 192.168.1.1
  hostapd /etc/hostapd/hostapd.conf &
  dhcpd wlan0 &
elif [ $1 = "stop" ]
then
  kill `pidof hostapd`
  kill `pidof dhcpd`
  ifconfig wlan0 down
  netctl start wlan0-Milano
elif [ $1 == "connect" ]
then
  systemctl stop netctl*
  kill `pidof hostapd`
  kill `pidof dhcpd`
  ifconfig wlan0 192.168.1.162
  iw dev wlan0 disconnect
  iw dev wlan0 connect $2
else
  echo "Usage: ./dummynet.sh [start|stop|connect]"
  exit 1
fi
