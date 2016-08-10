#!/usr/bin/env python

current_block = None

class JobNode:
  JOB_ID = 0
  JOB_NODES = []
  def __init__(self, name):
    self.name = name
    self.id = JobNode.newId()
    self.firstBlock = TaskBlock()
  
  @staticmethod
  def newId():
    JobNode.TASK_ID = JobNode.JOB_ID+1
    return JobNode.JOB_ID
  
  @staticmethod
  def add(job):
    JobNode.JOB_NODES.append(job)
  
  @staticmethod
  def get(task):
    for t in JobNode.JOB_NODES:
      if t.name == task:
        return t
    return None

class TaskNode:
  TASK_ID = 0
  TASK_NODES = []
  def __init__(self, name):
    self.name = name
    self.id = TaskNode.newId()
  
  @staticmethod
  def newId():
    TaskNode.TASK_ID = TaskNode.TASK_ID+1
    return TaskNode.TASK_ID
  
  @staticmethod
  def add(task):
    TaskNode.TASK_NODES.append(task)
  
  @staticmethod
  def get(task):
    for t in TaskNode.TASK_NODES:
      if t.name == task:
        return t
    return None

class TaskBlock:
  def __init__(self, parent=None, next=None):
    self.taskList = []
    self.parent = parent
    self.next = next

class ControlNode:
  next = None
