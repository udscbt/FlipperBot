/*************************************
 * Auto-generated ScheMo source file *
 *************************************
 * Original file name: main.cpp
 * New file name: out/__schemo_main.cpp
 * Identified jobs: 1
 *   Named: 0
 *   Unnamed: 1
 * Identified tasks: 8
 *   Named: 1
 *   Unnamed: 7
 */

#include "schemo.h"
#include <cstdio>

SCHEMO_DECLARE_TASK(fatt)
SCHEMO_DECLARE_TASK(__schemo_task_0)
SCHEMO_DECLARE_TASK(__schemo_task_1)
SCHEMO_DECLARE_TASK(__schemo_task_2)
SCHEMO_DECLARE_TASK(__schemo_task_3)
SCHEMO_DECLARE_TASK(__schemo_task_4)
SCHEMO_DECLARE_TASK(__schemo_task_5)
SCHEMO_DECLARE_TASK(__schemo_task_6)
SCHEMO_DECLARE_JOB(__schemo_job_0)
SCHEMO_DECLARE_JVAR(i, __schemo_job_0, int)
SCHEMO_DECLARE_PARAM_STACK(int,n,fatt)
SCHEMO_DECLARE_FVAR(n, fatt, int)
SCHEMO_DECLARE_FVAR(ret, fatt, int)
SCHEMO_DECLARE_RETURN_STACK(int,fatt)
SCHEMO_DECLARE_GETBACK_STACK(fatt)



SCHEMO_FUNCTION(fatt)



{
  
  
  SCHEMO_IF(__schemo_task_1, __schemo_task_0, (SCHEMO_PARAM(n, fatt) == 0))
    SCHEMO_GETBACK(1, fatt)
SCHEMO_DELETE_PARAM(n,fatt)
SCHEMO_RETURN
  SCHEMO_ENDIF(__schemo_task_0)

  
  SCHEMO_PUSH_PARAM(n,fatt,SCHEMO_PARAM(n, fatt)-1)
SCHEMO_INIT_FVAR(n, fatt, int)
SCHEMO_INIT_FVAR(ret, fatt, int)
SCHEMO_CALL(int, fatt, __schemo_task_5, ret)

  
  SCHEMO_FVAR(ret, fatt) = ret;
  
  SCHEMO_FVAR(n, fatt) = SCHEMO_PARAM(n, fatt);
  SCHEMO_GETBACK(SCHEMO_FVAR(n, fatt)*SCHEMO_FVAR(ret, fatt), fatt)
SCHEMO_DELETE_PARAM(n,fatt)
SCHEMO_RETURN
}

SCHEMO_JOB(__schemo_job_0,__schemo_task_2)
{
  
  
  SCHEMO_JVAR(i, __schemo_job_0) = 0;
  SCHEMO_WHILE(__schemo_task_3, __schemo_task_4, (SCHEMO_JVAR(i, __schemo_job_0) < 10))    SCHEMO_PUSH_PARAM(n,fatt,SCHEMO_JVAR(i, __schemo_job_0))
SCHEMO_INIT_FVAR(n, fatt, int)
SCHEMO_INIT_FVAR(ret, fatt, int)
SCHEMO_CALL(int, fatt, __schemo_task_6, ret)

    printf("%d: %d\n", SCHEMO_JVAR(i, __schemo_job_0)++, ret);
  SCHEMO_LOOPBACK(__schemo_task_3, __schemo_task_4)
}

#include <stack>

int main()
{
  SCHEMO_INIT
SCHEMO_INIT_JOB(__schemo_job_0,__schemo_task_2)

  
  SCHEMO_SCHEDULE_JOB(__schemo_job_0)


  schemo::start_cycle();
  
  return 0;
}
