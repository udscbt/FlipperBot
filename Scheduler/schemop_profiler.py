#!/usr/bin/env python

current_job = None
current_task = None

class JobNode:
  JOB_ID = 0
  JOB_NODES = []
  def __init__(self, name):
    self.name = name
    self.id = JobNode.newId()
    self.first = None
    JobNode.JOB_NODES.append(self)
  
  @staticmethod
  def newId():
    JobNode.JOB_ID = JobNode.JOB_ID+1
    return str(JobNode.JOB_ID)
  
  @staticmethod
  def get(job):
    for j in JobNode.JOB_NODES:
      if j.name == job:
        return j
    return None
  
  def __str__(self):
    s = self.id + "\t"
    if self.first is not None:
      s = s + str(self.first)
      n = self.first
      while n.next is not None:
        n = n.next
        s = s + " " + str(n)
    s = s + " S"
    return s

class FunctionNode:
  FUN_ID = 0
  FUN_NODES = []
  def __init__(self, name):
    self.name = name
    self.id = FunctionNode.newId()
    self.first = None
    FunctionNode.FUN_NODES.append(self)
  
  @staticmethod
  def newId():
    FunctionNode.FUN_ID = FunctionNode.FUN_ID+1
    return str(FunctionNode.FUN_ID)
  
  @staticmethod
  def get(fun):
    for f in FunctionNode.FUN_NODES:
      if f.name == fun:
        return f
    return None
  
  def __str__(self):
    s = self.id + "\t"
    if self.first is not None:
      s = s + str(self.first)
      n = self.first
      while n.next is not None:
        n = n.next
        s = s + " " + str(n)
    s = s + " S"
    return s

class TaskNode:
  TASK_ID = 0
  TASK_NODES = []
  def __init__(self, name):
    self.name = name
    self.id = TaskNode.newId()
    self.next = None
    self.cnt = None
    self.brk = None
    self.ext = False
    self.shd = False
    TaskNode.TASK_NODES.append(self)
  
  @staticmethod
  def newId():
    TaskNode.TASK_ID = TaskNode.TASK_ID+1
    return str(TaskNode.TASK_ID)
  
  @staticmethod
  def get(task):
    for t in TaskNode.TASK_NODES:
      if t.name == task:
        return t
    return None
  
  def __str__(self):
    s = self.id
    if self.cnt is not None:
      s = s + " C ( {} )".format(self.cnt.id)
    if self.brk:
      s = s + " B ( {} )".format(self.brk.id)
    if self.ext:
      s = s + " R"
    if self.shd:
      s = s + " S!"
    return s

class WhileNode:
  WHILE_NODES = []
  def __init__(self, loop):
    self.inside = None
    self.next = None
    self.loop = loop
    WhileNode.WHILE_NODES.append(self)
  
  @staticmethod
  def get(task):
    for w in WhileNode.WHILE_NODES:
      if w.inside == task:
        return w
    return None
  
  def __str__(self):
    s = "{} [".format('L' if self.loop else 'W')
    n = self.inside
    if n is not None:
      s = s + " " + str(n)
      while n.next is not None:
        n = n.next
        s = s + " " + str(n)
    s = s + " ]"
    return s

class IfNode:
  def __init__(self):
    self.inside = None
    self.next = None
  
  def __str__(self):
    s = "I ["
    n = self.inside
    if n is not None:
      s = s + " " + str(n)
      while n.next is not None:
        n = n.next
        s = s + " " + str(n)
    s = s + " ]"
    return s

class CallNode:
  def __init__(self, fun=None, task=None):
    self.fun = fun
    self.next = task

  def __str__(self):
    return "F ( {} )".format(self.fun.id)

def profileJob(job, task):
  global current_job
  global current_task
  current_job = JobNode(job)
  current_task = TaskNode(task)
  current_job.first = current_task

def profileFunction(fun):
  global current_job
  global current_task
  current_job = FunctionNode(fun)
  current_task = TaskNode(fun)
  current_job.first = current_task

def profileTbreak(task):
  global current_task
  current_task.next = TaskNode(task)
  current_task = current_task.next

def profileWhileInside(wnode, task):
  global current_task
  wnode.inside = TaskNode(task)
  current_task = wnode.inside

def profileWhileNext(task, loop=False):
  global current_task
  wnode = WhileNode(loop)
  current_task.next = wnode
  wnode.next = TaskNode(task)
  current_task = wnode.next
  return wnode

def profileIfInside(inode, task):
  global current_task
  inode.inside = TaskNode(task)
  current_task = inode.inside

def profileIfNext(task):
  global current_task
  inode = IfNode()
  current_task.next = inode
  inode.next = TaskNode(task)
  current_task = inode.next
  return inode

def profileCall(fun, task):
  global current_task
  task = TaskNode(task)
  cnode = CallNode(FunctionNode.get(fun), task)
  current_task.next = cnode
  current_task = task

def profileContinue(bw):
  global current_task
  current_task.cnt = TaskNode.get(bw)

def profileBreak(bw):
  global current_task
  wnode = WhileNode.get(TaskNode.get(bw))
  current_task.brk = wnode.next

def profileExit():
  global current_task
  current_task.ext = True

def profileShutdown():
  global current_task
  current_task.shd = True

def profileReturn():
  profileExit()
