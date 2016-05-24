#include "schemo.h"
#include <cstdio>
#include <vector>

@DECLARE

int global_count = 0;
std::vector<int> fibonacci_results;

@FUNCTION(fibonacci)
@PARAM(n:int)
@RETURN(unsigned int)
{
  @MEMORY
  {
    @VAR(ret1:unsigned int)
    @VAR(ret2:unsigned int)
  }
  
  @IF (@PARAM(n) < 0)
  {
    @RETURN(0);
  }
  
  @CRITSEC(fibonacci)
  {
    @IF (fibonacci_results.size() <= @PARAM(n))
    {
      @IF (@PARAM(n) == 0)
      {
        fibonacci_results.push_back(1);
      }
      
      @CALL(fibonacci; @PARAM(n)-1):ret1;
      @VAR(ret1) = ret1;
      @CALL(fibonacci; @PARAM(n)-2):ret2;
      @VAR(ret2) = ret2;
      fibonacci_results.push_back(@VAR(ret1)+@VAR(ret2));
    }
  }
  
  @RETURN(fibonacci_results[@PARAM(n)]);
}

@JOB
{
  @MEMORY
  {
    @VAR(i:int)
  }
  
  printf("Fibonacci numbers:\n");
  
  @VAR(i) = 0;
  @WHILE (@VAR(i) < 50)
  {
    @CALL(fibonacci; @VAR(i)):ret;
    printf("#%d: %u\n", @VAR(i)++, ret);
    @CRITSEC(count)
    {
      global_count++;
    }
  }
  
  schemo::deschedule_job(job_5);
}

@JOB(job_5)
{
  @WHILE
  {
    @CRITSEC(count)
    {
      @IF(global_count == 5)
      {
        printf("BANG!\n");
        @TBREAK
        global_count = 0;
      }
    }
  }
}

int main()
{
  @INIT
  
  @SCHEDULE_ALL
  
  global_count = 0;

  schemo::start_cycle();
  
  return 0;
}
