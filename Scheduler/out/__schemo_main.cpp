/*************************************
 * Auto-generated ScheMo source file *
 *************************************
 * Original file name: main.cpp
 * New file name: out/__schemo_main.cpp
 * Identified jobs: 2
 *   Named: 1
 *   Unnamed: 1
 * Identified tasks: 18
 *   Named: 1
 *   Unnamed: 17
 */

#include "schemo.h"
#include <cstdio>

SCHEMO_DECLARE_TASK(fibonacci)
SCHEMO_DECLARE_TASK(__schemo_task_0)
SCHEMO_DECLARE_TASK(__schemo_task_1)
SCHEMO_DECLARE_TASK(__schemo_task_2)
SCHEMO_DECLARE_TASK(__schemo_task_3)
SCHEMO_DECLARE_TASK(__schemo_task_4)
SCHEMO_DECLARE_TASK(__schemo_task_5)
SCHEMO_DECLARE_TASK(__schemo_task_6)
SCHEMO_DECLARE_TASK(__schemo_task_7)
SCHEMO_DECLARE_TASK(__schemo_task_8)
SCHEMO_DECLARE_TASK(__schemo_task_9)
SCHEMO_DECLARE_TASK(__schemo_task_10)
SCHEMO_DECLARE_TASK(__schemo_task_11)
SCHEMO_DECLARE_TASK(__schemo_task_12)
SCHEMO_DECLARE_TASK(__schemo_task_13)
SCHEMO_DECLARE_TASK(__schemo_task_14)
SCHEMO_DECLARE_TASK(__schemo_task_15)
SCHEMO_DECLARE_TASK(__schemo_task_16)
SCHEMO_DECLARE_JOB(__schemo_job_0)
SCHEMO_DECLARE_JVAR(i, __schemo_job_0, int)
SCHEMO_DECLARE_JOB(job_5)
SCHEMO_DECLARE_PARAM_STACK(int,n,fibonacci)
SCHEMO_DECLARE_FVAR(ret1, fibonacci, int)
SCHEMO_DECLARE_FVAR(ret2, fibonacci, int)
SCHEMO_DECLARE_RETURN_STACK(int,fibonacci)
SCHEMO_DECLARE_GETBACK_STACK(fibonacci)


int global_i = 0;

SCHEMO_FUNCTION(fibonacci)



{
  SCHEMO_INIT_FVAR(ret1, fibonacci, int)
SCHEMO_INIT_FVAR(ret2, fibonacci, int)

  SCHEMO_IF(__schemo_task_1, __schemo_task_0, (SCHEMO_PARAM(n, fibonacci) < 0))
    SCHEMO_GETBACK(-1, fibonacci)
SCHEMO_DELETE_PARAM(n,fibonacci)
SCHEMO_DELETE_FVAR(ret1,fibonacci)
SCHEMO_DELETE_FVAR(ret2,fibonacci)
SCHEMO_RETURN
  SCHEMO_ENDIF(__schemo_task_0)

  SCHEMO_IF(__schemo_task_3, __schemo_task_2, (SCHEMO_PARAM(n, fibonacci) == 0))
    SCHEMO_GETBACK(1, fibonacci)
SCHEMO_DELETE_PARAM(n,fibonacci)
SCHEMO_DELETE_FVAR(ret1,fibonacci)
SCHEMO_DELETE_FVAR(ret2,fibonacci)
SCHEMO_RETURN
  SCHEMO_ENDIF(__schemo_task_2)

  SCHEMO_IF(__schemo_task_5, __schemo_task_4, (SCHEMO_PARAM(n, fibonacci) == 1))
    SCHEMO_GETBACK(1, fibonacci)
SCHEMO_DELETE_PARAM(n,fibonacci)
SCHEMO_DELETE_FVAR(ret1,fibonacci)
SCHEMO_DELETE_FVAR(ret2,fibonacci)
SCHEMO_RETURN
  SCHEMO_ENDIF(__schemo_task_4)

  
  SCHEMO_PUSH_PARAM(n,fibonacci, SCHEMO_PARAM(n, fibonacci)-1)
SCHEMO_CALL(int, fibonacci, __schemo_task_14, ret1)

  SCHEMO_FVAR(ret1, fibonacci) = ret1;
  SCHEMO_PUSH_PARAM(n,fibonacci, SCHEMO_PARAM(n, fibonacci)-2)
SCHEMO_CALL(int, fibonacci, __schemo_task_15, ret2)

  SCHEMO_FVAR(ret2, fibonacci) = ret2;
  SCHEMO_GETBACK(SCHEMO_FVAR(ret1, fibonacci)+SCHEMO_FVAR(ret2, fibonacci), fibonacci)
SCHEMO_DELETE_PARAM(n,fibonacci)
SCHEMO_DELETE_FVAR(ret1,fibonacci)
SCHEMO_DELETE_FVAR(ret2,fibonacci)
SCHEMO_RETURN
}

SCHEMO_JOB(__schemo_job_0,__schemo_task_11)
{
  
  
  printf("Fibonacci numbers:\n");
  
  SCHEMO_JVAR(i, __schemo_job_0) = 0;
  SCHEMO_WHILE(__schemo_task_12, __schemo_task_13, (SCHEMO_JVAR(i, __schemo_job_0) < 10))    SCHEMO_PUSH_PARAM(n,fibonacci, SCHEMO_JVAR(i, __schemo_job_0))
SCHEMO_CALL(int, fibonacci, __schemo_task_16, ret)

    global_i = SCHEMO_JVAR(i, __schemo_job_0);
    printf("#%d: %d\n", SCHEMO_JVAR(i, __schemo_job_0)++, ret);
  SCHEMO_LOOPBACK(__schemo_task_12, __schemo_task_13)
  
  schemo::deschedule_job(job_5);
}

SCHEMO_JOB(job_5,__schemo_task_6)
{
  SCHEMO_WHILE(__schemo_task_7, __schemo_task_8, (true))    SCHEMO_IF(__schemo_task_10, __schemo_task_9, (global_i == 5))      printf("5 reached!\n");
    SCHEMO_ENDIF(__schemo_task_9)
  SCHEMO_LOOPBACK(__schemo_task_7, __schemo_task_8)
}

int main()
{
  SCHEMO_INIT
SCHEMO_INIT_JOB(__schemo_job_0,__schemo_task_11)
SCHEMO_INIT_JOB(job_5,__schemo_task_6)

  
  SCHEMO_SCHEDULE_JOB(__schemo_job_0)
SCHEMO_SCHEDULE_JOB(job_5)


  schemo::start_cycle();
  
  return 0;
}
