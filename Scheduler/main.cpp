#include "schemo.h"
#include <cstdio>

@DECLARE

@JOB (prova)
{
  printf("Ciao");
  @TBREAK
  printf(" come ");
  @TBREAK
  printf("va?");
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
