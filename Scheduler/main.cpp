#include "schemo.h"
#include <cstdio>

@DECLARE

@FUNCTION (ciao)
@PARAM(hi:int)
@RETURN(int)
{
  @PARAM(hi) = @PARAM(hi)+1;
  @TBREAK
  @RETURN(@PARAM(hi)*2);
}

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
    @CALL(ciao;@VAR(i)):res;
    printf("%d", res);
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
