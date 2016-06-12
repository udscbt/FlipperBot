/*************************************
 * Auto-generated ScheMo source file *
 *************************************
 * Original file name: main/schemo.h
 * New file name: ./main/schemo.h
 * Identified jobs: 0
 *   Named: 0
 *   Unnamed: 0
 * Identified tasks: 0
 *   Named: 0
 *   Unnamed: 0
 */



#ifndef SCHEMO_H
#define SCHEMO_H

#include <stack>
#include <queue>



#define SCHEMO_TASK_HANDLE_SUFFIX _handle
#define SCHEMO_JOIN_STRINGS_(prefix, suffix) prefix ## suffix
#define SCHEMO_JOIN_STRINGS(prefix, suffix) SCHEMO_JOIN_STRINGS_(prefix, suffix)
#define SCHEMO_TASK_HANDLE(task) SCHEMO_JOIN_STRINGS(task, SCHEMO_TASK_HANDLE_SUFFIX)

#define SCHEMO_JOB(name, task) \
  void SCHEMO_TASK_HANDLE(task)()

#define SCHEMO_TBREAK(task) \
    schemo::run_task(task); \
  } \
  void SCHEMO_TASK_HANDLE(task)() \
  {

#define SCHEMO_DECLARE
#define SCHEMO_SCHEDULE_ALL

#define SCHEMO_DECLARE_TASK(task) \
  void SCHEMO_TASK_HANDLE(task)(); \
  schemo::TASK task = {.handle = &SCHEMO_TASK_HANDLE(task), .checkpoint = true};

#define SCHEMO_DECLARE_JOB(job) \
  schemo::JOB job;

#define SCHEMO_INIT_JOB(job, task) \
  schemo::init_job(job, #job, task);
  
#define SCHEMO_INIT \
  schemo::cycle_setup();
  
#define SCHEMO_SCHEDULE_JOB(job) \
  schemo::schedule_job(job);

#define SCHEMO_JDELAY(job, d) \
  job.delay = d;

#define SCHEMO_JVAR(var, job) job##_var_##var

#define SCHEMO_DECLARE_JVAR(var, job, type) \
  type SCHEMO_JVAR(var, job);

#define SCHEMO_IF(task_if, task_after, condition) \
    if condition \
    { \
      schemo::run_task(task_if); \
    } \
    else \
    { \
      schemo::run_task(task_after); \
    } \
    return; \
  } \
  void SCHEMO_TASK_HANDLE(task_if)() \
  {

#define SCHEMO_ENDIF(task_after) SCHEMO_TBREAK(task_after)

#define SCHEMO_WHILE(task_bk, task_fw, condition) \
    schemo::run_task(task_bk); \
  } \
  void SCHEMO_TASK_HANDLE(task_bk)() \
  { \
    if (! condition) \
    { \
      schemo::run_task(task_fw); \
      return; \
    }

#define SCHEMO_LOOPBACK(task_bk, task_fw) \
    schemo::run_task(task_bk); \
  } \
  void SCHEMO_TASK_HANDLE(task_fw)() \
  {

#define SUBTASK if (schemo::check_timeout()) return;

#define SCHEMO_RETURN_STACK(function) \
  function ## _return_stack

#define SCHEMO_DECLARE_RETURN_STACK(type, function) \
  std::stack<type> SCHEMO_RETURN_STACK(function)[schemo::MAX_JOBS];

#define SCHEMO_PARAM_STACK(param, function) \
  function ## _param_ ## param ## _stack

#define SCHEMO_PARAM(param, function) \
  SCHEMO_PARAM_STACK(param, function)[schemo::current_job->job->id].top()


#define SCHEMO_DECLARE_PARAM_STACK(type, param, function) \
  std::stack<type> SCHEMO_PARAM_STACK(param, function)[schemo::MAX_JOBS];

#define SCHEMO_GETBACK_STACK(function) \
  function ## _getback_stack

#define SCHEMO_DECLARE_GETBACK_STACK(function) \
  std::stack<schemo::TASK*> SCHEMO_GETBACK_STACK(function)[schemo::MAX_JOBS];

#define SCHEMO_GETBACK(ret, function) \
  SCHEMO_RETURN_STACK(function)[schemo::current_job->job->id].push(ret); \
  schemo::run_task(*SCHEMO_GETBACK_STACK(function)[schemo::current_job->job->id].top()); \
  SCHEMO_GETBACK_STACK(function)[schemo::current_job->job->id].pop();

#define SCHEMO_RETURN \
  return;

#define SCHEMO_PUSH_PARAM(param, function, set) \
  SCHEMO_PARAM_STACK(param, function)[schemo::current_job->job->id].push(set);

#define SCHEMO_DELETE_PARAM(param, function) \
  SCHEMO_PARAM_STACK(param, function)[schemo::current_job->job->id].pop();

#define SCHEMO_FUNCTION(function) \
  void SCHEMO_TASK_HANDLE(function)()

#define SCHEMO_CALL(type, function, newtask, ret) \
    SCHEMO_GETBACK_STACK(function)[schemo::current_job->job->id].push(&newtask); \
    schemo::run_task(function); \
  } \
  void SCHEMO_TASK_HANDLE(newtask)() \
  { \
    type ret = SCHEMO_RETURN_STACK(function)[schemo::current_job->job->id].top(); \
    SCHEMO_RETURN_STACK(function)[schemo::current_job->job->id].pop();

#define SCHEMO_DECLARE_FVAR(var, fun, type) \
  std::stack<type> SCHEMO_JVAR(var, fun)[schemo::MAX_JOBS];

#define SCHEMO_FVAR(var, fun) \
  SCHEMO_JVAR(var, fun)[schemo::current_job->job->id].top()

#define SCHEMO_INIT_FVAR(var, fun, type) \
  type var; \
  SCHEMO_JVAR(var, fun)[schemo::current_job->job->id].push(var);

#define SCHEMO_DELETE_FVAR(var, fun) \
  SCHEMO_JVAR(var, fun)[schemo::current_job->job->id].pop();

#define SCHEMO_MUTEX(resource) \
  resource ## _mutex

#define SCHEMO_DECLARE_MUTEX(resource) \
  schemo::MUTEX SCHEMO_MUTEX(resource);

#define SCHEMO_MUTEX_START(resource, task) \
    schemo::lock_mutex(SCHEMO_MUTEX(resource), task); \
    schemo::run_task(task); \
  } \
  void SCHEMO_TASK_HANDLE(task)() \
  { \
    if (!schemo::check_mutex(SCHEMO_MUTEX(resource), task)) \
    { \
      schemo::run_task(task); \
      return; \
    }

#define SCHEMO_MUTEX_END(resource) \
  schemo::unlock_mutex(SCHEMO_MUTEX(resource));
  

namespace schemo {

  
  typedef long int timeout_t;

  typedef struct
  {
    void (*handle)();
    
    bool checkpoint;
    void (*revert)();
    
    bool dynamic;
  } TASK;
  
  const unsigned int JOB_MAX_NAME_LENGTH = 100;
  typedef struct
  {
    char name[JOB_MAX_NAME_LENGTH];
    int id;
    TASK *current;
    TASK *checkpoint;
    int delay;
    int elapsed;
    int priority;
  } JOB;
  
  typedef struct CYCLE_NODE
  {
    JOB* job;
    CYCLE_NODE* prev;
    CYCLE_NODE* next;
  } CYCLE_NODE;
  
  typedef enum
  {
    ERROR_NONE,
    ERROR_TOO_MANY_JOBS,
    ERROR_NO_CHECKPOINT,
    ERROR_COUNT
  } ERROR_CODE;
  
  typedef std::queue<TASK*> MUTEX;

  
  const int NULL = 0;
  const int MAX_JOBS = 10;
  extern int new_job_id;
  extern CYCLE_NODE job_queue[MAX_JOBS];
  extern CYCLE_NODE NULL_NODE;
  extern CYCLE_NODE* first_job;
  extern CYCLE_NODE* current_job;
  
  const int MAX_ERROR_NAME_LENGTH = 20;
  const int MAX_ERROR_MSG_LENGTH = 100;
  extern char ERROR_STR[ERROR_COUNT][MAX_ERROR_NAME_LENGTH];
  extern char ERROR_MSG[MAX_ERROR_MSG_LENGTH];
  extern ERROR_CODE _last_error;
   
  
  bool cycle_setup();
  bool start_cycle();
   
  bool init_job(JOB& job, const char name[], TASK& start_point, int delay = 0, int priority = 0);
  JOB init_job(const char name[], TASK& start_point, int delay = 0, int priority = 0);
  bool schedule_job(JOB& job);
  bool deschedule_job(JOB& job);
  bool deschedule_job(CYCLE_NODE* job);
  CYCLE_NODE* find_job_by_name(char name[]);
  CYCLE_NODE* find_job_by_id(int id);
  
  bool run_task(TASK&);
  bool check_timeout();
  bool lock_mutex(MUTEX&, TASK&);
  bool unlock_mutex(MUTEX&);
  bool check_mutex(MUTEX&, TASK&);
  
  ERROR_CODE last_error();
  bool set_error(ERROR_CODE ec, const char em[]);
};

#endif 