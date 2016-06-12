if [ $# -ne 1 ]
then
  echo "Usage: ./dummynet.sh [start|stop]"
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
  netctl start wlan0-Milano
else
  echo "Usage: ./dummynet.sh [start|stop]"
  exit 1
fi
