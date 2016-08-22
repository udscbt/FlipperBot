#include "schemo.h"
#include <cstring>

#include <cassert>

namespace schemo
{
  /**
   * Global variables
   */
  int new_job_id = 0;
  CYCLE_NODE job_queue[MAX_JOBS];
  CYCLE_NODE NULL_NODE;
  CYCLE_NODE* first_job;
  CYCLE_NODE* current_job;
  
  char ERROR_STR[ERROR_COUNT][MAX_ERROR_NAME_LENGTH] = {};
  char ERROR_MSG[MAX_ERROR_MSG_LENGTH] = "";
  ERROR_CODE _last_error = ERROR_NONE;
}

/**
 * Functions
 */

bool schemo::cycle_setup()
{
  NULL_NODE.job = NULL;
  NULL_NODE.prev = &NULL_NODE;
  NULL_NODE.next = &NULL_NODE;
  first_job = &NULL_NODE;
  current_job = &NULL_NODE;
  
  new_job_id = 0;
  return true;
}

bool schemo::start_cycle()
{
  SCHEMO_P_INIT
  while (first_job != &NULL_NODE)
  {
    current_job = first_job;
    
    while (current_job != &NULL_NODE)
    {
      if (current_job->job->elapsed)
      {
        current_job->job->elapsed--;
      }
      else
      {
        current_job->job->elapsed = current_job->job->delay;
        
        TASK *task = current_job->job->current;
        if (task == NULL)
        {
          deschedule_job(current_job);
        }
        else
        {
          current_job->job->current = NULL;
          SCHEMO_P_START_TASK(current_job->job, task)
          task->handle();
          SCHEMO_P_STOP_TASK(current_job->job, task)
        }
      }
      
      current_job = current_job->next;
    }
  }
  SCHEMO_P_CLOSE
  
  return true;
}

bool schemo::stop_cycle()
{
  first_job = &NULL_NODE;
  return true;
}

bool schemo::init_job(JOB& job, const char name[], TASK& start_point, int delay, int priority)
{
  if (new_job_id > MAX_JOBS)
  {
    set_error(ERROR_TOO_MANY_JOBS, "");
    return false;
  }
  
  if (!start_point.checkpoint)
  {
    set_error(ERROR_NO_CHECKPOINT, "");
    return false;
  }
  
  int i;
  char* src;
  char* dst;
  for (i=0, src=(char*)name, dst=job.name; i<JOB_MAX_NAME_LENGTH; i++, src++, dst++)
  {
    if (*src) *dst = *src;
    else break;
  }
  *dst = '\0';
  
  job.id = new_job_id++;
  job.current = &start_point;
  job.checkpoint = job.current;
  job.delay = delay;
  job.elapsed = 0;
  job.priority = priority;
  
  return true;
}

schemo::JOB schemo::init_job(const char name[], TASK& start_point, int delay, int priority)
{
  JOB job;
  init_job(job, name, start_point, delay, priority);
  return job;
}

bool schemo::schedule_job(JOB& job)
{
  CYCLE_NODE* newnode = &job_queue[job.id];
  CYCLE_NODE* node = first_job;
  
  while (node != &NULL_NODE && node->job->priority >= job.priority) node = node->next;
  
  if (node == first_job)
  {
    first_job = newnode;
  }
  newnode->job = &job;
  newnode->next = node;
  newnode->prev = node->prev;
  node->prev->next = newnode;
  node->prev = newnode;

  return true;
}

bool schemo::deschedule_job(CYCLE_NODE* node)
{
  if (node == &NULL_NODE)
    return false;

  if (node->prev != &NULL_NODE)
    node->prev->next = node->next;
  if (node->next != &NULL_NODE)
    node->next->prev = node->prev;
  if (node == first_job)
    first_job = node->next;
  
  return true;
}

bool schemo::deschedule_job(JOB& job)
{
  CYCLE_NODE* node = first_job;
  while (node != &NULL_NODE && node->job != &job) node = node->next;
  
  return deschedule_job(node);
}

schemo::CYCLE_NODE* schemo::find_job_by_name(char name[])
{
  CYCLE_NODE* node = first_job;
  while (node != &NULL_NODE && strcmp(node->job->name, name)) node = node->next;
  
  return node;
}

schemo::CYCLE_NODE* schemo::find_job_by_id(int id)
{
  CYCLE_NODE* node = first_job;
  while (node != &NULL_NODE && node->job->id != id) node = node->next;
  
  return node;
}

bool schemo::run_task(TASK& t)
{
  current_job->job->current = &t;
  
  return true;
}

bool schemo::check_timeout()
{
  return false;
}


bool schemo::lock_mutex(MUTEX& mutex, TASK& task)
{
  mutex.push(&task);
}

bool schemo::unlock_mutex(MUTEX& mutex, TASK& task)
{
  if (check_mutex(mutex, task))
  {
    mutex.pop();
    return true;
  }
  else
  {
    return false;
  }
}

bool schemo::check_mutex(MUTEX& mutex, TASK& task)
{
  if (mutex.front() == &task)
    return true;
  else
    return false;
}

schemo::ERROR_CODE schemo::last_error()
{
  return _last_error;
}

bool schemo::set_error(ERROR_CODE ec, const char em[])
{
  _last_error = ec;
  strcpy((char*)em, ERROR_MSG);
  return true;
}
