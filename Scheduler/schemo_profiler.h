#ifndef SCHEMO_PROFILER_H
#define SCHEMO_PROFILER_H

#ifdef SCHEMO_PROFILER
  #include <cstdio>
  #undef NULL

  #define SCHEMO_P_TASK_DATA \
    int profiler_id;
  #define SCHEMO_P_TASK_SETID(task, id) \
    task.profiler_id = id;
  #define SCHEMO_P_JOB_DATA \
    int profiler_id;
  #define SCHEMO_P_JOB_SETID(job, id) \
      job.profiler_id = id;
  #define SCHEMO_P_PROFILE_NAME "schemo.profile"
  #define SCHEMO_P_INIT \
    schemo::profiler::init();
  #define SCHEMO_P_CLOSE \
    schemo::profiler::close();
  #define SCHEMO_P_GET_ID(x) \
    ((x).profiler_id)
  #define SCHEMO_P_START_TASK(job, task) \
    schemo::profiler::start_task(SCHEMO_P_GET_ID(*job), SCHEMO_P_GET_ID(*task));
  #define SCHEMO_P_STOP_TASK(job, task) \
    schemo::profiler::stop_task(SCHEMO_P_GET_ID(*job), SCHEMO_P_GET_ID(*task));

  namespace schemo::profiler
  {
    void init();
    void close();
    void start_task(int job, int task);
    void stop_task(int job, int task);
    extern FILE* profile;
  }
#else
  #define SCHEMO_P_TASK_DATA
  #define SCHEMO_P_TASK_SETID
  #define SCHEMO_P_JOB_DATA
  #define SCHEMO_P_JOB_SETID
  #define SCHEMO_P_INIT
  #define SCHEMO_P_CLOSE
  #define SCHEMO_P_START_TASK(job, task)
  #define SCHEMO_P_STOP_TASK(job, task)
  #define SCHEMO_P_GET_ID(x)
#endif

#endif //SCHEMO_PROFILER_H
