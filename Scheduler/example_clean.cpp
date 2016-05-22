#include "schemo.h"
#include <cstdio>

@DECLARE

@JOB
{
  @JOB_MEMORY
  {
    int @JVAR(i);
  }
  
  @JVAR(i) = 0;
  @WHILE (@JVAR(i) < 100)
  {
    printf("Ciao");
    
    @TBREAK
    
    printf(" come ");
    
    @TBREAK
    
    printf(" va?\n");
    
    @JVAR(i)++;
  }
}

@JOB
{
  @JOB_MEMORY
  {
    int @JVAR(i), @JVAR(j);
  }
  
  @JVAR(i) = 0;
  @WHILE (@JVAR(i) < 100)
  {
    @JVAR(j) = 0;
    @IF (@JVAR(i) < 50)
    {
      @WHILE (@JVAR(j) < @JVAR(i))
      {
        printf("*");
        @JVAR(j)++;
      }
    }
    printf(" %d\n", @JVAR(i));
    @TBREAK
    @JVAR(i)++;    
  }
}

int main()
{
  schemo::cycle_setup();
  
  @INIT
  
  @SCHEDULE_ALL

  schemo::start_cycle();
  
  return 0;
}
