#include "schemo.h"
#include <cstdio>

void __schemo_task_0_handle();
schemo::TASK __schemo_task_0 = {.handle = &__schemo_task_0_handle, .checkpoint = true};

void __schemo_task_1_handle();
schemo::TASK __schemo_task_1 = {.handle = &__schemo_task_1_handle, .checkpoint = true};

void __schemo_task_2_handle();
schemo::TASK __schemo_task_2 = {.handle = &__schemo_task_2_handle, .checkpoint = true};

void __schemo_task_3_handle();
schemo::TASK __schemo_task_3 = {.handle = &__schemo_task_3_handle, .checkpoint = true};

void __schemo_task_4_handle();
schemo::TASK __schemo_task_4 = {.handle = &__schemo_task_4_handle, .checkpoint = true};

void __schemo_task_5_handle();
schemo::TASK __schemo_task_5 = {.handle = &__schemo_task_5_handle, .checkpoint = true};

void __schemo_task_6_handle();
schemo::TASK __schemo_task_6 = {.handle = &__schemo_task_6_handle, .checkpoint = true};

void __schemo_task_7_handle();
schemo::TASK __schemo_task_7 = {.handle = &__schemo_task_7_handle, .checkpoint = true};

void __schemo_task_8_handle();
schemo::TASK __schemo_task_8 = {.handle = &__schemo_task_8_handle, .checkpoint = true};

void __schemo_task_9_handle();
schemo::TASK __schemo_task_9 = {.handle = &__schemo_task_9_handle, .checkpoint = true};

void __schemo_task_10_handle();
schemo::TASK __schemo_task_10 = {.handle = &__schemo_task_10_handle, .checkpoint = true};

void __schemo_task_11_handle();
schemo::TASK __schemo_task_11 = {.handle = &__schemo_task_11_handle, .checkpoint = true};

schemo::JOB __schemo_job_1;
int __schemo_job_1_var_i, __schemo_job_1_var_j;

schemo::JOB __schemo_job_0;
int __schemo_job_0_var_i;


void __schemo_task_3_handle()
{
  __schemo_job_0_var_i = 0;
  schemo::run_task(__schemo_task_5);
}

void __schemo_task_5_handle()
{
  if (__schemo_job_0_var_i < 100) {}
  else
  {
    schemo::run_task(__schemo_task_6);
    return;
  }
  printf("Ciao");
  schemo::run_task(__schemo_task_0);
}

void __schemo_task_0_handle()
{
  printf(" come ");

  schemo::run_task(__schemo_task_1);
}

void __schemo_task_1_handle()
{
  printf(" va?\n");

  __schemo_job_0_var_i++;
  schemo::run_task(__schemo_task_5);
}

void __schemo_task_6_handle()
{
}

void __schemo_task_4_handle()
{
  __schemo_job_1_var_i = 0;

  schemo::run_task(__schemo_task_7);
}

void __schemo_task_7_handle()
{
  if (__schemo_job_1_var_i < 100) {}
  else
  {
    schemo::run_task(__schemo_task_8);
    return;
  }
  __schemo_job_1_var_j = 0;
  
  if (__schemo_job_1_var_i < 50) {}
  else
  {
    schemo::run_task(__schemo_task_11);
    return;
  }
  schemo::run_task(__schemo_task_9);
}

void __schemo_task_9_handle()
{
  if (__schemo_job_1_var_j < __schemo_job_1_var_i) {}
  else
  {
    schemo::run_task(__schemo_task_10);
    return;
  }
  printf("*");
  __schemo_job_1_var_j++;
  schemo::run_task(__schemo_task_9);
}

void __schemo_task_10_handle()
{
  schemo::run_task(__schemo_task_11);
}

void __schemo_task_11_handle()
{
  printf(" %d\n", __schemo_job_1_var_i);
  schemo::run_task(__schemo_task_2);
}

void __schemo_task_2_handle()
{
  __schemo_job_1_var_i++;
  schemo::run_task(__schemo_task_7);
}

void __schemo_task_8_handle() {
}

int main()
{
  schemo::cycle_setup();

  schemo::init_job(__schemo_job_1, "__schemo_job_1", __schemo_task_4);
  schemo::init_job(__schemo_job_0, "__schemo_job_0", __schemo_task_3);

  schemo::schedule_job(__schemo_job_1);
  schemo::schedule_job(__schemo_job_0);

  schemo::start_cycle();

  return 0;
}

