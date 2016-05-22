#include "schemo.h"
#include <cstdio>

@DECLARE

@JOB(job1)
{
  @JDELAY(3)

  @WHILE
  {
    printf("Ciao");
    
    @TBREAK
    
    printf(" come ");
    
    @TBREAK
    
    printf("va?\n");
  }
}

@JOB
{
  @MEMORY
  {
    int @VAR(i), @VAR(j);
  }
  
  @VAR(i) = 0;
  @WHILE (@VAR(i) < 100)
  {
    @VAR(j) = 0;
    @IF (@VAR(i) < 50)
    {
      @WHILE (@VAR(j) < @VAR(i))
      {
        printf("*");
        @VAR(j)++;
      }
    }
    printf(" %d\n", @VAR(i));
    @TBREAK
    @VAR(i)++;    
  }
  
  schemo::deschedule_job(job1);
}

/*
Notes:
Each function has stacks for return values and parameters. Each stack level is
a call of the function. They need a current depth level variable too.
@CALLing a function means running a related task. A function task extracts
parameters from the stack and puts the return value in the stack. @CALLing
increments the depth level, @RETURNing decreases it.
*/

int main()
{
  @INIT
  
  @SCHEDULE_ALL

  schemo::start_cycle();
  
  return 0;
}
