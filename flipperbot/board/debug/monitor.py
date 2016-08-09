from os.path import dirname, abspath, join
from ..game import Game
from ..totems import totems
from ...controller import Controller
from ...robot import Robot
from time import time

debug_root = dirname(abspath(__file__))
template_file = join(debug_root, "monitor_text_format.txt")
config_file = join(debug_root, "monitor_config.py")

class format_dict (dict):
  def __init__(self, *args, replace=None, **kwargs):
    self.replace = replace
    dict.__init__(self, *args, **kwargs)
  
  def __missing__(self, key):
    if self.replace is None:
      return "{%s}" % key
    else:
      return self.replace

class Monitor:
  def __init__(self, game=None, config=None, template=None):
    if game is None:
      game = Game(totems.values())
    self.game = game
    self.template = ""
    self.config = format_dict()
    if config is None:
      self.setConfig(config_file)
      self.setTemplate(template_file)
    else:
      self.setConfig(config)
      self.setTemplate(template)
    self.loadConfig()
    self.loadTemplate()
  
  def setConfig(self, filename):
    self.config_file = filename
  
  def setTemplate(self, filename):
    self.template_file = filename
  
  def loadTemplate(self):
    f = open(self.template_file, "r")
    self.template = f.read()
    f.close()
    self.formatted = self.template.format(**self.config)
  
  def loadConfig(self):
    f = open(self.config_file, "r")
    self.config.update(eval(f.read()))
    f.close()
    self.formatted = self.template.format(**self.config)
  
  def print(self, clear=False):
    d = format_dict(replace=" ")
    for t in self.game.totemList:
      if t._on:
        if t._hit:
          val = self.config['totem_ha']
        else:
          val = self.config['totem_ta']
      else:
        if t._hit:
          val = self.config['totem_hp']
        else:
          val = self.config['totem_tp']
      d['totem_'+t.pos.lower()] = val
    for i in range(2):
      if self.game.controllers[i].active:
        d['j{}_connected'.format(i)] = self.config['connected']
        d['j{}_direction'.format(i)] = (
          Controller.Direction.getName(
            self.game.controllers[i].direction
          ).replace("_", " "))
        d['j{}_eb'.format(i)] = 'N/A'
      else:
        d['j{}_connected'.format(i)] = self.config['disconnected']
        d['j{}_direction'.format(i)] = self.config['none']
        d['j{}_eb'.format(i)] = self.config['none']
      
      if self.game.robots[i].active:
        d['r{}_connected'.format(i)] = self.config['connected']
        d['r{}_direction'.format(i)] = (
          Robot.Direction.getName(
            self.game.robots[i].direction
          ).replace("_", " "))
        d['r{}_hit'.format(i)] = (self.config['pressed']
                                  if self.game.robots[i].hit
                                  else self.config['released'])
      else:
        d['r{}_connected'.format(i)] = self.config['disconnected']
        d['r{}_direction'.format(i)] = self.config['none']
        d['r{}_hit'.format(i)] = self.config['none']
    
    d['game_mode'] = self.game.getModeName(self.game.mode)
    
    #~ clear_command = "\033[2J\033[H"
    clear_command = "\033c"
    print((clear_command if clear else "") + self.formatted.format(**d))
  
  def display(self, updateTime=None):
    if updateTime == None:
      updateTime = 0.1
    while True:
      t = time()
      self.print(clear=True)
      while time()-t < updateTime:
        pass
  
  def log(self, msg, *tags):
    self.game.log.write(msg, "Monitor", *tags)
