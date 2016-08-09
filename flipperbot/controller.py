from .shared import SharedVariable

class Controller:
  class Direction:
    STOP = 1
    FORWARD = 2
    BACKWARD = 3
    LEFT = 4
    RIGHT = 5
    FORWARD_LEFT = 6
    FORWARD_RIGHT = 7
    BACKWARD_LEFT = 8
    BACKWARD_RIGHT = 9
    
    def getName(d, default=None):
      for k,v in Controller.Direction.__dict__.items():
        if v == d:
          return k
      return default
  
  direction = SharedVariable(Direction.STOP)
  active    = SharedVariable(False)
  
  def __init__(self):
    pass
