#include "schemo.h"
#include <cstdio>
#include <ctime>

@DECLARE

int main()
{
  @INIT
  
  @SCHEDULE_ALL

  schemo::start_cycle();
  
  return 0;
}
