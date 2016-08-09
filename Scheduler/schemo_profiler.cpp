#ifdef SCHEMO_PROFILER
#include "schemo.h"
#include <ctime>

FILE* schemo::profiler::profile;

void schemo::profiler::init()
{
  profile = fopen(SCHEMO_P_PROFILE_NAME, "a");
  fprintf(profile, "#EXECUTION START\n%lu\n", time(0));
}

void schemo::profiler::close()
{
  fclose(profile);
}

void schemo::profiler::start_task(int job, int task)
{
  fprintf(profile, "%d %d %f ", job, task, ((float)clock()/CLOCKS_PER_SEC));
}

void schemo::profiler::stop_task(int job, int task)
{
  fprintf(profile, "%f\n", ((float)clock()/CLOCKS_PER_SEC));
}
#endif
