#include "schemo.h"
#include <cstdio>

@DECLARE

@JOB (job_1)
{
  @TBREAK
  @TBREAK
  @CRITSEC (hello)
  {
    @TBREAK
    @TBREAK
  }
}

@JOB (job_2)
{
  @CRITSEC (ciao)
  {
    @CRITSEC (hello)
    {
      @TBREAK
      @TBREAK
    }
    @TBREAK
  }
  @TBREAK
  @TBREAK

}

@JOB (job_3)
{
  @CRITSEC (ciao)
  {
    @TBREAK
  }
  @TBREAK
}

int main()
{
  @INIT
  @SCHEDULE_ALL

  schemo::start_cycle();
  
  return 0;
}
