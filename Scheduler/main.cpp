#include "schemo.h"
#include <cstdio>

@DECLARE

@JOB (prova)
{
  @MEMORY
  {
    @VAR(i:int)
    @VAR(j:int)
  }
  @VAR(i) = 0;
  @WHILE(@VAR(i)<10)
  {
    @VAR(j) = 0;
    @WHILE(@VAR(j)<10)
    {
      printf("*");
      ++@VAR(j);
    }
    printf("\n");
    ++@VAR(i);
  }
}

@JOB (test)
{
  @MEMORY
  {
    @VAR(i:int)
  }
  @VAR(i) = 0;
  @WHILE(@VAR(i) < 10)
  {
    @IF(@VAR(i)%5)
    {
      printf("AAA\n");
    }
    printf("%d", @VAR(i));
    ++@VAR(i);
  }
}

int main()
{
  @INIT
  @SCHEDULE_ALL

  schemo::start_cycle();
  
  return 0;
}
