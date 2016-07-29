from .log import Log
from time import strftime
from ..shared import termColors

class Debug:
  logging = True
  stdout = False
  def __init__(self, log=None, logging=None, stdout=None, name=None, tags=None, parent=None):
    if logging is not None:
      self.logging = logging
    if stdout is not None:
      self.stdout = stdout
    if self.logging:
      if log is None:
        log = Log()
      self.log = log
    else:
      self.log = None
    self.name = name if name is not None else ""
    self.tags = tags if tags is not None else []
    if parent is not None:
      self.parent = parent
    else:
      self.parent = self
  
  def addTags(self, *tags):
    self.tags = self.tags + tags
  
  def clearTags(self):
    self.tags = []
  
  def setName(self, name):
    self.name = name
  
  def __call__(self, *args, **kwargs):
    if self.stdout:
      print(
        "[{}] {}{}:{} ".format(
          strftime("%H:%M:%S"),
          termColors['BOLD'],
          self.name,
          termColors['END']
        ), *args, **kwargs
      )
    if self.logging:
      s = " ".join([str(x) for x in args])
      tags = self.tags
      if 'tags' in kwargs:
        tags = tags + self.kwargs
      if self.name != "":
        tags = ["Name={}".format(self.name)]+tags
      self.log.write(s, *tags, caller=self.parent)

fakedebug = Debug(logging=False, stdout=False)
