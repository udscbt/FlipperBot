import RPi.GPIO as gpio
from time import sleep, time
from threading import Thread, Lock

gpio.setmode(gpio.BOARD)

text = "0000"
textLock = Lock()
paused = True
stopped = False
