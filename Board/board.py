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
    'select' : 5,
    'hit' : 8
  },
  'DR' : {
    'select' : 15,
    'hit' : 16
  },
  'UR'  : {
    'select' : 21,
    'hit' : 22
  },
  'UL' : {
    'select' : 23,
    'hit' : 24
  },
  'C' : {
    'select' : 3,
    'hit' : 7
  }
}

for tot in totems.values():
  gpio.setup(tot['select'], gpio.OUT, initial=gpio.LOW)
  gpio.setup(tot['hit'], gpio.IN)

# OTHER GPIOs
everythingButton = 18
gpio.setup(everythingButton, gpio.IN, pull_up_down=gpio.PUD_DOWN)
speaker = 19
gpio.setup(speaker, gpio.OUT)

# GAME PARAMETERS
deltat = 13
mint   = 0.5
blinkt = 5
period = 0.25

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
    show(1, shownText[1])
    sleep(dispt)
    show(2, shownText[2])
    sleep(dispt)
    show(3, shownText[3])
    sleep(dispt)
    textLock.acquire()
    shownText = text
    textLock.release()
  show(0, " ")
  print("displayThread stopped")

points = 0

anim_deltat = 0.1
anim_duration = 2

totemList = [totems['DL'], totems['C'], totems['DR']]

stopped = False

def gameThread():
  global text
  global textLock
  global points
  global totemList
  global stopped
  global paused
  points = 0
  lose = False
  while not stopped:
    for tot in totemList:#totems.values():
      selectOn = False
      blink = False
      gpio.output(tot['select'], gpio.HIGH)
      last = time()
      lasthit = last
      missing = deltat
      while True:
        sleep(0.01)
        now = time()
        missing = deltat+last-now
        textLock.acquire()
        text = [points/10, points%10, int(missing)/10, int(missing)%10]
        textLock.release()
        while paused and not stopped:
          pass
        now = time()
        last = now+missing-deltat
        if stopped:
          break
        if not blink and missing < blinkt:
          blink = True
          lastblink = now
        if blink and now - lastblink > period:
          selectOn = not selectOn
          lastblink = now
          gpio.output(tot['select'], selectOn)
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
            sleep(anim_deltat)
            textLock.acquire()
            text = "_._-"
            textLock.release()
            sleep(anim_deltat)
            textLock.acquire()
            text = "-_._"
            textLock.release()
            sleep(anim_deltat)
            textLock.acquire()
            text = "^-_."
            textLock.release()
            sleep(anim_deltat)
            textLock.acquire()
            text = "-^-_"
            textLock.release()
            sleep(anim_deltat)
            textLock.acquire()
            text = "_-^-"
            textLock.release()
            sleep(anim_deltat)
          break
      gpio.output(tot['select'], gpio.LOW)
      if stopped:
        break
      if lose:
        points = 0
        lose = False
        break
  textLock.acquire()
  text = None
  textLock.release()
  print("gameThread stopped")

paused = True

def pauseThread():
  global paused
  global stopped
  pressed = False
  resuming = False
  while not stopped:
    if not pressed and gpio.input(everythingButton):
      if paused:
        resuming = True
      else:
        paused = True
        resuming = False
    if resuming and not gpio.input(everythingButton):
      paused = False
      resuming = False
    
    if gpio.input(everythingButton):
      pressed = True
    else:
      pressed = False
    sleep(0.05)
  print("pauseThread stopped")

t1 = None
t2 = None
t3 = None
def start():
  global t1
  global t2
  global t3
  t1 = Thread(target=displayThread)
  t2 = Thread(target=gameThread)
  t3 = Thread(target=pauseThread)
  t1.start()
  t2.start()
  t3.start()

def stop():
  global stopped
  stopped = True
  gpio.cleanup()

start()
