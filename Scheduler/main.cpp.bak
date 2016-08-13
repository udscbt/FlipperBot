#include "schemo.h"
#include <cstdio>

@DECLARE

@JOB
{
  @MEMORY
  {
    @VAR(i:int)
  }
  @VAR(i) = 0;
  @WHILE
  {
    printf("%d\n", @VAR(i)++);
  }
}

@JOB (schemo_test)
{
  @MEMORY
  {
    @VAR(i:int)
  }
  @VAR(i) = 0;
  @WHILE (@VAR(i) < 10)
  {
    ++@VAR(i);
  }
}

@JOB
{
  @MEMORY
  {
    @VAR(i:int)
  }
  @VAR(i) = -1;
  @WHILE
  {
    ++@VAR(i);
    printf("%c\n", 'A'+@VAR(i));
    @IF (@VAR(i) % 5)
    {
      @CONTINUE
    }
    @IF (@VAR(i) == 10)
    {
      @SHUTDOWN
    }
    printf("!!!\n");
  }
}

int main()
{
  @INIT
  @SCHEDULE_ALL

  schemo::start_cycle();
  
  return 0;
}
