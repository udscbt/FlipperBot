#ifdef SCHEMO_PROFILER
#include "schemo.h"
#include <ctime>

FILE* schemo::profiler::profile;

void schemo::profiler::init()
{
  profile = fopen(SCHEMO_P_PROFILE_NAME, "a");
  fprintf(profile, "#EXECUTION START\n%lu\n", time(0));
  fprintf(profile, "#JOB\tTASK\tSTART\tSTOP\n");
}

void schemo::profiler::close()
{
  fclose(profile);
}

void schemo::profiler::start_task(int job, int task)
{
  fprintf(profile, "%d\t%d\t%.3f\t", job, task, ((float)clock()*1000/CLOCKS_PER_SEC));
}

void schemo::profiler::stop_task(int job, int task)
{
  fprintf(profile, "%.3f\n", ((float)clock()*1000/CLOCKS_PER_SEC));
}
#endif
