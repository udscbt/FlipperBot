from .shared import ThreadEx, sleep, time, SharedVariable
from random import random
from math import ceil
from .audio import join, audio_root, Audio, SoundEffect
from .totems import totems

Audio.LOOK = join(audio_root, "sounds", 'simonlook.ogg')
Audio.SAY = join(audio_root, "sounds", 'simonsay.ogg')

class GameThread (ThreadEx):
  def __init__(self, game):
    self.game = game
    super(GameThread, self).__init__(name="game")
    
    self.difficulty = 0
    self.sequence = []
    self.center = self.game.totemList.index(totems['C'])
  
  def newSequence(self):
    l = 0
    for i in range(10):
      l = l + random()*(self.difficulty/5+1)
    l = ceil(l/10)
    self.sequence = []
    for i in range(l):
      t = self.center
      while t == self.center:
        t = int(random()*len(self.game.totemList))
      self.sequence.append(t)
  
  def showSequence(self):
    self.game.mode = self.game.NOSTOP
    self.game.display.show("Stop")
    sound = SoundEffect(self.game.audio.LOOK)
    sound.start()
    sound.wait()
    for index in range(len(self.sequence)):
      self.game.totem = self.game.totemList[self.sequence[index]]
      self.game.totem.on()
      sleep(1)
      self.game.totem.off()
      sleep(0.5)
    sleep(1)
    self.game.mode = self.game.GAME
  
  def setup(self):
    self.lasthit = -1
    
    print("Game started")
    self.game.mode = self.game.NOSTOP
    self.game.display.show("_-^-"*2)
    self.game.display.setScrollSpeed(10);
    sound = SoundEffect(self.game.audio.READY)
    self.game.audio.stop()
    sound.start()
    sound.wait()
    self.game.mode = self.game.GAME
    
    self.lifes = 10
    self.points = 0
    self.index = 0
    self.game.display.setScrollSpeed(self.game.scrollF)
    self.newSequence()
    self.showSequence()
    sound = SoundEffect(self.game.audio.SAY)
    sound.start()
    sound.wait()
  
  def turnOffTotems(self):
    for t in self.sequence:
      self.game.totemList[t].off()
  
  def loop(self):
    righttotem = self.game.totemList[self.sequence[self.index]]
    # Update display
    text = "{:02d}{:02d}".format(self.lifes, self.points%100)
    self.game.display.show(text, [False, True, False, False])
    
    # Check for hit
    for t in self.game.totemList:
      if t == self.lasthit:
        continue
      self.game.totem = t
      if self.game.totem.isHit() or self.game.isHit2():
        if t == self.game.totemList[self.center]:
          pass
        elif t == righttotem:
          sound = SoundEffect(self.game.audio.HIT)
          sound.start()
          self.game.totem.on()
          self.index = self.index + 1
          if self.index == len(self.sequence):
            self.points = self.points + self.index
            self.difficulty = self.difficulty+1
            self.turnOffTotems()
            self.index = 0
            self.newSequence()
            self.showSequence()
            sound = SoundEffect(self.game.audio.SAY)
            sound.start()
            sound.wait()
        else:
          self.lifes = self.lifes - 1
          sound = SoundEffect(self.game.audio.LOSE)
          sound.start()
          sound.wait()
          self.turnOffTotems()
          if self.lifes > 0:
            self.index = 0
            self.showSequence()
            sound = SoundEffect(self.game.audio.SAY)
            sound.start()
            sound.wait()
          else:
            elf.game.setMode(self.game.LOST)
        self.lasthit = t
        return
  
  def cleanup(self):
    if not self.game.totem.stopped():
      self.game.totem.off()
    self.game.audio.stop()
