#include "schemo.h"
#include <cstdio>

@DECLARE


@FUNCTION(fatt)
@PARAM(n:int)
@RETURN(int)
{
  @MEMORY
  {
    @VAR(n:int);
    @VAR(ret:int);
  }
  
  @IF (@PARAM(n) == 0)
  {
    @RETURN(1);
  }
  
  @CALL(fatt;@PARAM(n)-1):ret;
  
  @VAR(ret) = ret;
  
  @VAR(n) = @PARAM(n);
  @RETURN(@VAR(n)*@VAR(ret));
}

@JOB
{
  @MEMORY
  {
    @VAR(i:int)
  }
  
  @VAR(i) = 0;
  @WHILE (@VAR(i) < 10)
  {
    @CALL(fatt;@VAR(i)):ret;
    printf("%d: %d\n", @VAR(i)++, ret);
  }
}

#include <stack>

int main()
{
  @INIT
  
  @SCHEDULE_ALL

  schemo::start_cycle();
  
  return 0;
}
