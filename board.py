import RPi.GPIO as gpio
from time import sleep, time
from threading import Thread, Lock

# GPIO SETUP
gpio.setmode(gpio.BOARD)

# DISPLAY SETUP
display = {
  'segment' : {
    'a' : 31,
    'b' : 32,
    'c' : 33,
    'd' : 35,
    'e' : 36,
    'f' : 37,
    'g' : 38,
    'dp': 40
  },
  'cathode' : {
    0 : 10,
    1 : 11,
    2 : 12,
    3 : 13
  }
}

for seg in display['segment'].values():
  gpio.setup(seg, gpio.OUT, initial=gpio.LOW)

for cat in display['cathode'].values():
  gpio.setup(cat, gpio.OUT, initial=gpio.HIGH)

def show(digit, value, dot=False):
  if value == 0 or value == '0':
    on = "abcdef"
  elif value == 1 or value == '1':
    on = "bc"
  elif value == 2 or value == '2':
    on = "abedg"
  elif value == 3 or value == '3':
    on = "abcdg"
  elif value == 4 or value == '4':
    on = "bcfg"
  elif value == 5 or value == '5':
    on = "acdfg"
  elif value == 6 or value == '6':
    on = "acdefg"
  elif value == 7 or value == '7':
    on = "abc"
  elif value == 8 or value == '8':
    on = "abcdefg"
  elif value == 9 or value == '9':
    on = "abcdfg"
  elif value == 'A':
    on = "abcefg"
  elif value == "B":
    on = "abcdefg"
  elif value == 'b':
    on = "cdefg"
  elif value == 'C':
    on = "adef"
  elif value == 'c':
    on = "deg"
  elif value == 'd':
    on = "bcdeg"
  elif value == 'E':
    on = "adefg"
  elif value == 'e':
    on = "abgdef"
  elif value == 'F':
    on = "adef"
  elif value == 'H':
    on = "bcefg"
  elif value == 'h':
    on = "cefg"
  elif value == 'I':
    on = "bc"
  elif value == 'i':
    on = "c"
  elif value == 'J':
    on = "bcd"
  elif value == 'j':
    on = "cd"
  elif value == 'L':
    on = "def"
  elif value == 'l':
    on = "ef"
  elif value == 'O':
    on = "abcdef"
  elif value == 'o':
    on = "cdeg"
  elif value == 'P':
    on = "abefg"
  elif value == 'r':
    on = "eg"
  elif value == 'S':
    on = "afgcd"
  elif value == 't':
    on = "defg"
  elif value == 'U':
    on = "bcdef"
  elif value == 'u':
    on = "cde"
  elif value == 'y':
    on = "bcdfg"
  elif value == '-':
    on = "g"
  elif value == '_':
    on = "d"
  elif value == '^':
    on = "a"
  elif value == ' ':
    on = ""
  elif value == '.':
    on = ["dp"]
  else:
    return False
    
  if dot:
    on = list(on)+["dp"]
  
  for seg in display['segment'].values():
    gpio.output(seg, gpio.LOW)
  
  for cat in display['cathode'].values():
    gpio.output(cat, gpio.HIGH)
  
  for seg in on:
    gpio.output(display['segment'][seg], gpio.HIGH)
  gpio.output(display['cathode'][digit], gpio.LOW)
  
  return True


# TOTEM SETUP

totems = {
  'DL' : {
    'led' : 7,
    'hit' : 8
  },
  'DR' : {
    'led' : 15,
    'hit' : 16
  },
  'C'  : {
    'led' : 21,
    'hit' : 22
  },
  'UL' : {
    'led' : 23,
    'hit' : 24
  },
  'UR' : {
    'led' : 3,
    'hit' : 5
  }
}

for tot in totems.values():
  gpio.setup(tot['led'], gpio.OUT, initial=gpio.LOW)
  gpio.setup(tot['hit'], gpio.IN)

# GAME PARAMETERS
deltat = 13
mint   = 0.5
blinkt = 5
period = 0.25
ledon  = False

def test(text, i):
  for j in range(int(10/(4*i))):
    for c in range(len(text)):
      show(c, text[c])
      sleep(i)

text = "0000"
textLock = Lock()

dispt = 0.001

def displayThread():
  global text
  global textLock
  global dispt
  shownText = "0000"
  while shownText is not None:
    show(0, shownText[0])
    sleep(dispt)
    show(1, shownText[1], True)
    sleep(dispt)
    show(2, shownText[2])
    sleep(dispt)
    show(3, shownText[3])
    sleep(dispt)
    textLock.acquire()
    shownText = text
    textLock.release()
  show(0, " ")
  print("displayThread stop")

points = 0

def gameThread():
  global text
  global textLock
  global ledon
  global points
  points = 0
  lose = False
  while points is not None:
    for tot in [totems['DL']]:#totems.values():
      blink = False
      gpio.output(tot['led'], gpio.HIGH)
      last = time()
      lasthit = last
      while True:
        now = time()
        missing = deltat+last-now
        textLock.acquire()
        text = [points/10, points%10, int(missing)/10, int(missing)%10]
        textLock.release()
        if not blink and missing < blinkt:
          blink = True
          lastblink = now
        if blink and now - lastblink > period:
          ledon = not ledon
          lastblink = now
          gpio.output(tot['led'], ledon)
        if gpio.input(tot['hit']):
          if now-lasthit >= mint:
            points = points + 1
            break
          else:
            lasthit = now
        if missing < 0: 
          lose = True
          for i in range(2):
            textLock.acquire()
            text = "._-^"
            textLock.release()
            sleep(0.2)
            textLock.acquire()
            text = "_._-"
            textLock.release()
            sleep(0.2)
            textLock.acquire()
            text = "-_._"
            textLock.release()
            sleep(0.2)
            textLock.acquire()
            text = "^-_."
            textLock.release()
            sleep(0.2)
            textLock.acquire()
            text = "-^-_"
            textLock.release()
            sleep(0.2)
            textLock.acquire()
            text = "_-^-"
            textLock.release()
            sleep(0.2)
          break
      gpio.output(tot['led'], gpio.LOW)
      if lose:
        points = 0
        lose = False
        break
  textLock.acquire()
  text = None
  textLock.release()
  print("gameThread stop")

t1 = None
t2 = None
def start():
  global t1
  global t2
  t1 = Thread(target=displayThread)
  t2 = Thread(target=gameThread)
  t1.start()
  t2.start()
