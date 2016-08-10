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
      s = s + str(self.first) + " "
      n = self.first
      while n.next is not None:
        n = n.next
        s = s + str(n) + " "
    s = s + "S"
    return s

class TaskNode:
  TASK_ID = 0
  TASK_NODES = []
  def __init__(self, name):
    self.name = name
    self.id = TaskNode.newId()
    self.next = None
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
    return self.id

class WhileNode:
  def __init__(self):
    self.inside = None
    self.next = None
  
  def __str__(self):
    s = "W ["
    n = self.inside
    if n is not None:
      s = s + " " + str(n) + " "
      while n.next is not None:
        n = n.next
        s = s + " " + str(n)
    s = s + "]"
    return s

class IfNode:
  def __init__(self):
    self.inside = None
    self.next = None
  
  def __str__(self):
    s = "I ["
    n = self.inside
    if n is not None:
      s = s + " " + str(n) + " "
      while n.next is not None:
        n = n.next
        s = s + " " + str(n)
    s = s + "] "
    return s

def profileJob(job, task):
  global current_job
  global current_task
  current_job = JobNode(job)
  current_task = TaskNode(task)
  current_job.first = current_task

def profileWhileInside(wnode, task):
  global current_task
  wnode.inside = TaskNode(task)
  current_task = wnode.inside

def profileWhileNext(task):
  global current_task
  wnode = WhileNode()
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
