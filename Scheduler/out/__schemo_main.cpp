/*************************************
 * Auto-generated ScheMo source file *
 *************************************
 * Original file name: main.cpp
 * New file name: out/__schemo_main.cpp
 * Identified jobs: 2
 *   Named: 1
 *   Unnamed: 1
 * Identified tasks: 22
 *   Named: 1
 *   Unnamed: 21
 */

#include "schemo.h"
#include <cstdio>

#include <vector>

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
SCHEMO_DECLARE_TASK(__schemo_task_17)
SCHEMO_DECLARE_TASK(__schemo_task_18)
SCHEMO_DECLARE_TASK(__schemo_task_19)
SCHEMO_DECLARE_TASK(__schemo_task_20)
SCHEMO_DECLARE_JOB(job_5)
SCHEMO_DECLARE_JOB(__schemo_job_0)
SCHEMO_DECLARE_JVAR(i, __schemo_job_0, int)
SCHEMO_DECLARE_PARAM_STACK(int,n,fibonacci)
SCHEMO_DECLARE_FVAR(ret2, fibonacci, unsigned int)
SCHEMO_DECLARE_FVAR(ret1, fibonacci, unsigned int)
SCHEMO_DECLARE_RETURN_STACK(unsigned int,fibonacci)
SCHEMO_DECLARE_GETBACK_STACK(fibonacci)
SCHEMO_DECLARE_MUTEX(fibonacci)
SCHEMO_DECLARE_MUTEX(count)


int global_count = 0;
std::vector<int> fibonacci_results;

SCHEMO_FUNCTION(fibonacci)



{
  SCHEMO_INIT_FVAR(ret2, fibonacci, unsigned int)
SCHEMO_INIT_FVAR(ret1, fibonacci, unsigned int)

  
  SCHEMO_IF(__schemo_task_1, __schemo_task_0, (SCHEMO_PARAM(n, fibonacci) < 0))
    SCHEMO_GETBACK(0, fibonacci)
SCHEMO_DELETE_PARAM(n,fibonacci)
SCHEMO_DELETE_FVAR(ret2,fibonacci)
SCHEMO_DELETE_FVAR(ret1,fibonacci)
SCHEMO_RETURN
  SCHEMO_ENDIF(__schemo_task_0)

  
  SCHEMO_MUTEX_START(fibonacci, __schemo_task_18)
    SCHEMO_IF(__schemo_task_3, __schemo_task_2, (fibonacci_results.size() <= SCHEMO_PARAM(n, fibonacci)))
      SCHEMO_IF(__schemo_task_5, __schemo_task_4, (SCHEMO_PARAM(n, fibonacci) == 0))
        fibonacci_results.push_back(1);
      SCHEMO_ENDIF(__schemo_task_4)

      
      SCHEMO_PUSH_PARAM(n,fibonacci, SCHEMO_PARAM(n, fibonacci)-1)
SCHEMO_CALL(unsigned int, fibonacci, __schemo_task_15, ret1)

      SCHEMO_FVAR(ret1, fibonacci) = ret1;
      SCHEMO_PUSH_PARAM(n,fibonacci, SCHEMO_PARAM(n, fibonacci)-2)
SCHEMO_CALL(unsigned int, fibonacci, __schemo_task_16, ret2)

      SCHEMO_FVAR(ret2, fibonacci) = ret2;
      fibonacci_results.push_back(SCHEMO_FVAR(ret1, fibonacci)+SCHEMO_FVAR(ret2, fibonacci));
    SCHEMO_ENDIF(__schemo_task_2)

  SCHEMO_MUTEX_END(fibonacci)
  
  SCHEMO_GETBACK(fibonacci_results[SCHEMO_PARAM(n, fibonacci)], fibonacci)
SCHEMO_DELETE_PARAM(n,fibonacci)
SCHEMO_DELETE_FVAR(ret2,fibonacci)
SCHEMO_DELETE_FVAR(ret1,fibonacci)
SCHEMO_RETURN
}

SCHEMO_JOB(__schemo_job_0,__schemo_task_12)
{
  
  
  printf("Fibonacci numbers:\n");
  
  SCHEMO_JVAR(i, __schemo_job_0) = 0;
  SCHEMO_WHILE(__schemo_task_13, __schemo_task_14, (SCHEMO_JVAR(i, __schemo_job_0) < 50))    SCHEMO_PUSH_PARAM(n,fibonacci, SCHEMO_JVAR(i, __schemo_job_0))
SCHEMO_CALL(unsigned int, fibonacci, __schemo_task_17, ret)

    printf("#%d: %u\n", SCHEMO_JVAR(i, __schemo_job_0)++, ret);
    SCHEMO_MUTEX_START(count, __schemo_task_19)
      global_count++;
    SCHEMO_MUTEX_END(count)
  SCHEMO_LOOPBACK(__schemo_task_13, __schemo_task_14)
  
  schemo::deschedule_job(job_5);
}

SCHEMO_JOB(job_5,__schemo_task_7)
{
  SCHEMO_WHILE(__schemo_task_8, __schemo_task_9, (true))    SCHEMO_MUTEX_START(count, __schemo_task_20)
      SCHEMO_IF(__schemo_task_11, __schemo_task_10, (global_count == 5))        printf("BANG!\n");
        SCHEMO_TBREAK(__schemo_task_6)
        global_count = 0;
      SCHEMO_ENDIF(__schemo_task_10)
    SCHEMO_MUTEX_END(count)
  SCHEMO_LOOPBACK(__schemo_task_8, __schemo_task_9)
}

int main()
{
  SCHEMO_INIT
SCHEMO_INIT_JOB(job_5,__schemo_task_7)
SCHEMO_INIT_JOB(__schemo_job_0,__schemo_task_12)

  
  SCHEMO_SCHEDULE_JOB(job_5)
SCHEMO_SCHEDULE_JOB(__schemo_job_0)

  
  global_count = 0;

  schemo::start_cycle();
  
  return 0;
}
