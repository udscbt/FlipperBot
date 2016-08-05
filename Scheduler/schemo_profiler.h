#ifndef SCHEMO_PROFILER_H
#define SCHEMO_PROFILER_H

#ifdef SCHEMO_PROFILER
  #define SCHEMO_P_TASK_DATA \
    int profiler_id;
  #define SCHEMO_P_TASK_SETID(task, id) \
    task.profiler_id = id;
  #define SCHEMO_P_JOB_DATA \
    int profiler_id;
  #define SCHEMO_P_JOB_SETID(job, id) \
      job.profiler_id = id;
  #define SCHEMO_P_INIT \
    schemo::profiler::init();
  #define SCHEMO_P_CLOSE \
    schemo::profiler::close();
  namespace schemo::profiler
  {
    void init();
    void close();
  }
#else
  #define SCHEMO_P_TASK_DATA
  #define SCHEMO_P_TASK_SETID
  #define SCHEMO_P_JOB_DATA
  #define SCHEMO_P_JOB_SETID
  #define SCHEMO_P_INIT
  #define SCHEMO_P_CLOSE
#endif

#endif //SCHEMO_PROFILER_H
