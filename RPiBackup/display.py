from RPi import GPIO
import time


dp = 40
g = 38
f = 37
e = 36
d = 35
c = 33
b = 32
a = 31

d1 = 10
d2 = 11
d3 = 12
d4 = 13

GPIO.setmode(GPIO.BOARD)
GPIO.setup(a, GPIO.OUT)
GPIO.output(a, False)
GPIO.setup(b, GPIO.OUT)
GPIO.output(b, False)
GPIO.setup(c, GPIO.OUT)
GPIO.output(c, False)
GPIO.setup(d, GPIO.OUT)
GPIO.output(d, False)
GPIO.setup(e, GPIO.OUT)
GPIO.output(e, False)
GPIO.setup(f, GPIO.OUT)
GPIO.output(f, False)
GPIO.setup(g, GPIO.OUT)
GPIO.output(g, False)
GPIO.setup(dp, GPIO.OUT)
GPIO.output(dp, False)

GPIO.setup(d1, GPIO.OUT)
GPIO.output(d1, True)
GPIO.setup(d2, GPIO.OUT)
GPIO.output(d2, True)
GPIO.setup(d3, GPIO.OUT)
GPIO.output(d3, True)
GPIO.setup(d4, GPIO.OUT)
GPIO.output(d4, True)

while True:
  GPIO.output(d, True)
  GPIO.output(c, True)
  GPIO.output(b, True)
#  GPIO.output(a, True)
  GPIO.output(dp, True)
  GPIO.output(g, True)
  GPIO.output(f, True)
#  GPIO.output(e, True)
  GPIO.output(d1, False)
  GPIO.output(d4, True)
  time.sleep(0.001)
  GPIO.output(d2, False)
  GPIO.output(d1, True)
  time.sleep(0.001)
  GPIO.output(d3, False)
  GPIO.output(d2, True)
  time.sleep(0.001)
  GPIO.output(d4, False)
  GPIO.output(d3, True)
  time.sleep(0.001)
