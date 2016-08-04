from os.path import dirname, abspath, join, isfile
from os import mkdir, remove
from time import time, strftime, localtime, strptime
from threading import Lock, Thread

import re

class LogTag:
  def __init__(self, value, name=None):
    self.name = name
    self.value = value
  
  def __str__(self):
    return (
      "{}={}".format(str(self.name), str(self.value))
      if self.name is not None
      else str(self.value)
    )

class LogEntry:
  def __init__(self, text=None, timestamp=None, tags=None):
    if text is None:
      text = ""
    self.text = text
    if timestamp is None:
      timestamp = localtime()
    self.timestamp = timestamp
    if tags is None:
      tags = []
    self.tags = tags
    
  full_pattern = re.compile(
    "("
      "(\[.+?\]) ?"  #Timestamp
      "(\[.+?\])? ?" #Additional tags
      "([^[]+)\n?"   #Rest of the line
    ")"
  )
  full_format = (
      "{timestamp} "  #Timestamp
      "{tags}"   #Additional tags
      "{text}" #Rest of the line
      "\n"
  )
  
  time_format = (
    "[%Y-%m-%d %H:%M:%S]"
  )

  tag_pattern = re.compile(
    "([^]=,]+=)?([^],]+),? ?"
  )
  tags_pattern = re.compile(
    "\[({})+\]" \
    .format(tag_pattern)
  )

  def parse(self, line):
    m = self.full_pattern.match(line)
    if m.end() != len(line):
      return None
    
    timestamp = m.group(2)
    try:
      self.timestamp = strptime(timestamp, time_format)
    except:
      return None
    
    self.text = m.group(4)
    
    tags = m.group(3)
    self.tags = []
    
    if tags is not None:
      m = self.tags_pattern.match(tags)
      if m.end() != len(tags):
        return None
      tags = tag_pattern.split(tags[1:-1])
      for t in range(int(len(tags)/3)):
        name = tags[3*t+1]
        if name is not None:
          name = name[:-1]
        value = tags[3*t+2]
        tag = LogTag(value, name)
        self.tags.append(tag)
    
    return self
  
  def __str__(self):
    timestamp = strftime(time_format, self.timestamp)
    tags = ""
    if len(self.tags) > 0:
      tags = "[{}]".format(
        " ,".join([str(tag) for tag in self.tags])
      )
    return self.full_format.format(
      timestamp=timestamp,
      tags=tags,
      text=self.text
    )

NET_TAG = "Network"
INIT_TAG = "Initialization"
WARN_TAG = "Warning"
ERR_TAG = "Error"
INFO_TAG = "Information"

log_dir = join(dirname(dirname(abspath(__file__))), "logs")
log_location = join(log_dir, "log.location")

class Log:
  def __init__(self, filename=None, abs=False, threshold=1000, enabled=True):
    self.enabled = enabled
    dirname = ""
    if filename is None or not abs:
      if isfile(log_location):
        f = open(log_location, "r")
        dirname = f.read()
        f.close()
      else:
        dirname = "{}".format(strftime("%Y%m%dT%H%M%S"))
        mkdir(join(log_dir, dirname))
        f = open(log_location, "w")
        f.write(dirname)
        f.close()
    if filename is None:
      filename = join(
        log_dir,
        dirname,
        "{}.log".format(strftime("%Y%m%dT%H%M%S"))
      )
    elif not abs:
      filename = join(log_dir, dirname, filename)
    self.log = filename
    log = open(self.log, "w")
    log.close()
    self.buffer = ""
    self.count = 0
    self.written = 0
    self.lastread = 0
    self.threshold = threshold
    self.queue = []
    self.qlock = Lock()
    self.wlock = Lock()
  
  class WriteThread (Thread):
    def __init__(self, parent, msg):
      self.parent = parent
      self.msg = msg
      Thread.__init__(self)
      
    def run(self):
      self.parent.qlock.acquire()
      self.parent.queue.append(self)
      self.parent.qlock.release()
      written = False
      while not written:
        self.parent.wlock.acquire()
        self.parent.qlock.acquire()
        if self.parent.queue[0] == self:
          self.parent.queue.pop(0)
          self.parent.qlock.release()
          
          self.parent._write(msg)
          written = True
          self.parent.wlock.release()
        else:
          self.parent.qlock.release()
          self.parent.wlock.release()
  
  def write(self, msg, *tags, caller=None):
    if self.enabled:
      tags = [str(tag) for tag in tags]
      if caller is not None:
        tags.append("Class={}".format(caller.__class__.__name__))
        tags.append("Module={}".format(caller.__module__))
      tagstr = "[{}] ".format(", ".join(tags)) if len(tags)>0 else ""
      msg = strftime("[%Y-%m-%d %H:%M:%S] ")+tagstr+msg+"\n"
      t = self.WriteThread(self, msg)
      t.start()
  
  def _write(self, msg):
    self.buffer = self.buffer + self.msg
    self.count = self.count + len(self.msg)
    if self.count-self.written > self.threshold:
      self.flush()
  
  def flush(self):
    log = open(self.log, "a")
    log.write(self.buffer)
    log.close()
    self.buffer = ""
    self.written = self.count
  
  def read(self, seek=None):
    self.wlock.acquire()
    if seek == None:
      seek = self.lastread
    msg = ""
    if seek < self.written:
      log = open(self.log, "r")
      log.seek(seek)
      msg = log.read()
      log.close()
      seek = self.written
    if seek < self.count:
      msg = msg + self.buffer[seek-self.written:]
    self.lastread = self.count
    ret = (self.lastread, msg)
    self.wlock.release()
    return ret
  
  def available(self, seek=None):
    if seek == None:
      seek = self.lastread
    return seek<self.count
  
  def close(self):
    while True:
      self.qlock.acquire()
      if len(self.queue) == 0:
        break
      self.qlock.release()
    self.qlock.release()
    self.flush()
  
  def closeall(self):
    self.close()
    if isfile(log_location):
      f = open(log_location, "r")
      dir = f.read()
      f.close()
      if join(log_dir, dir) == dirname(self.log):
        remove(log_location)
  
  @staticmethod
  def getFullName(filename):
    if isfile(log_location):
      f = open(log_location, "r")
      dirname = f.read()
      f.close()
    else:
      dirname = ""
    return join(log_dir, dirname, filename)


class ReadOnlyLog:
  def __init__(self, filename, abs=False):
    if not abs:
      filename = Log.getFullName(filename)
    self.log = filename
    self.lastread = 0
  
  def read(self, seek=None):
    if seek == None:
      seek = self.lastread
    msg = ""
    log = open(self.log, "r")
    log.seek(seek)
    msg = log.read()
    self.lastread = log.tell()
    log.close()
    return (self.lastread, msg)
  
  def available(self, seek=None):
    if seek == None:
      seek = self.lastread
    log = open(self.log, "r")
    log.seek(0, 2)
    length = log.tell()
    log.close()
    return seek<length

class LogViewer:
  def __new__(typ, *args, gui=False, **kwargs):
    if gui:
      cls = LogViewer_GUI
    else:
      cls = LogViewer_Text
    return cls.__new__(typ, *args, **kwargs)


class LogViewer_Superclass:
  def __init__(self, log, follow=True, abs=False):
    self.follow = follow
    if isinstance(log, Log):
      self.log = log
    elif isinstance(log, ReadOnlyLog):
      self.log = log
    elif isinstance(log, str):
      self.log = ReadOnlyLog(log, abs=abs)
    else:
      raise TypeError("arg 1 must be a Log, a ReadOnlyLog or a string")
    self.lastread = 0
    self.entries = []
    self.lastentry = -1
    self.order = self.noOrder
  
  def update(self):
    if self.log.available():
      (self.lastread, buf) = self.log.read(seek=self.lastread)
      if self.lastentry >= 0:
        buf = str(self.entries[self.lastentry]) + buf
      newentries = self.split(buf)
      self.order()
      return True
    else:
      return False    
  
  def split(self, text):
    pass
    
  def noOrder(self):
    pass
  
  def orderByDate(self):
    pass
  
  def orderByTag(self, tagname):
    def ordering(self):
      pass
    return ordering
  
  def filter(self, f):
    pass


import tkinter as tk

class LogViewer_GUI (LogViewer_Superclass, tk.Frame):
  def __init__(self, parent):
    self.parent = parent
  
class LogViewer_Text (LogViewer_Superclass):
  def __init__(self, parent):
    self.parent = parent
