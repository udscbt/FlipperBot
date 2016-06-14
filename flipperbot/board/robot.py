from .shared import SharedVariable

class Robot:
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
  
  hit       = SharedVariable(False)
  direction = SharedVariable(Direction.STOP)
  active    = SharedVariable(False)
  
  def __init__(self):
    pass
  
