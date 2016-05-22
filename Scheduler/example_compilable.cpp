/*************************************
 * Auto-generated ScheMo source file *
 *************************************
 * Original file name: main.cpp
 * New file name: out/__schemo_main.cpp
 * Identified jobs: 2
 *   Named: 0
 *   Unnamed: 2
 * Identified tasks: 12
 *   Named: 0
 *   Unnamed: 12
 */

#include "schemo.h"
#include <cstdio>

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

SCHEMO_DECLARE_JOB(__schemo_job_1)
int SCHEMO_JVAR(i, __schemo_job_1), SCHEMO_JVAR(j, __schemo_job_1);

SCHEMO_DECLARE_JOB(__schemo_job_0)
int SCHEMO_JVAR(i, __schemo_job_0);

SCHEMO_JOB(__schemo_job_0,__schemo_task_3)
{
  SCHEMO_JVAR(i, __schemo_job_0) = 0;
  SCHEMO_WHILE(__schemo_task_5, __schemo_task_6, (SCHEMO_JVAR(i, __schemo_job_0) < 100))
    printf("Ciao");
    
    SCHEMO_TBREAK(__schemo_task_0)
    
    printf(" come ");
    
    SCHEMO_TBREAK(__schemo_task_1)
    
    printf(" va?\n");
    
    SCHEMO_JVAR(i, __schemo_job_0)++;
  SCHEMO_LOOPBACK(__schemo_task_5, __schemo_task_6)
}

SCHEMO_JOB(__schemo_job_1,__schemo_task_4)
{
  SCHEMO_JVAR(i, __schemo_job_1) = 0;
  
  SCHEMO_WHILE(__schemo_task_7, __schemo_task_8, (SCHEMO_JVAR(i, __schemo_job_1) < 100))
    SCHEMO_JVAR(j, __schemo_job_1) = 0;
    SCHEMO_IF(__schemo_task_11, (SCHEMO_JVAR(i, __schemo_job_1) < 50))
      SCHEMO_WHILE(__schemo_task_9, __schemo_task_10, (SCHEMO_JVAR(j, __schemo_job_1) < SCHEMO_JVAR(i, __schemo_job_1)))
        printf("*");
        SCHEMO_JVAR(j, __schemo_job_1)++;
      SCHEMO_LOOPBACK(__schemo_task_9, __schemo_task_10)
    SCHEMO_ENDIF(__schemo_task_11)
    printf(" %d\n", SCHEMO_JVAR(i, __schemo_job_1));
    SCHEMO_TBREAK(__schemo_task_2)
    SCHEMO_JVAR(i, __schemo_job_1)++;    
  SCHEMO_LOOPBACK(__schemo_task_7, __schemo_task_8)
}

int main()
{
  schemo::cycle_setup();
  
  SCHEMO_INIT_JOB(__schemo_job_1,__schemo_task_4)
  SCHEMO_INIT_JOB(__schemo_job_0,__schemo_task_3)

  SCHEMO_SCHEDULE_JOB(__schemo_job_1)
  SCHEMO_SCHEDULE_JOB(__schemo_job_0)
  
  schemo::start_cycle();
  
  return 0;
}
