#include "schemo.h"
#include <cstdio>

@DECLARE

@JOB (job_1)
{
  @TBREAK
  @CRITSEC (hello)
  {
    printf("HELLO");
    @TBREAK
    @TBREAK
  }
  @TBREAK
}

@JOB (job_2)
{
  @CRITSEC (hello)
  {
    @TBREAK
    @TBREAK
  }
  @TBREAK
  @TBREAK

}

@JOB (job_3)
{
  @TBREAK
  @TBREAK
  @TBREAK
}

int main()
{
  @INIT
  @SCHEDULE_ALL

  schemo::start_cycle();
  
  return 0;
}
