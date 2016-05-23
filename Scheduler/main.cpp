#include "schemo.h"
#include <cstdio>

@DECLARE

int global_i = 0;

@FUNCTION(fibonacci)
@PARAM(n:int)
@RETURN(int)
{
  @MEMORY
  {
    @VAR(ret1:int)
    @VAR(ret2:int)
  }
  @IF (@PARAM(n) < 0)
  {
    @RETURN(-1);
  }
  @IF (@PARAM(n) == 0)
  {
    @RETURN(1);
  }
  @IF (@PARAM(n) == 1)
  {
    @RETURN(1);
  }
  
  @CALL(fibonacci; @PARAM(n)-1):ret1;
  @VAR(ret1) = ret1;
  @CALL(fibonacci; @PARAM(n)-2):ret2;
  @VAR(ret2) = ret2;
  @RETURN(@VAR(ret1)+@VAR(ret2));
}

@JOB
{
  @MEMORY
  {
    @VAR(i:int)
  }
  
  printf("Fibonacci numbers:\n");
  
  @VAR(i) = 0;
  @WHILE (@VAR(i) < 10)
  {
    @CALL(fibonacci; @VAR(i)):ret;
    global_i = @VAR(i);
    printf("#%d: %d\n", @VAR(i)++, ret);
  }
  
  schemo::deschedule_job(job_5);
}

@JOB(job_5)
{
  @WHILE
  {
    @IF (global_i == 5)
    {
      printf("5 reached!\n");
    }
  }
}

int main()
{
  @INIT
  
  @SCHEDULE_ALL

  schemo::start_cycle();
  
  return 0;
}
