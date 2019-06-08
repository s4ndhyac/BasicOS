#include "thread_spinlock.h"
#include "thread_mutex.h"
#include "thread_util.h"
#include "thread_cond_var.h"

// spinlock
void thread_spin_init(struct thread_spinlock *lk);

void thread_spin_lock(struct thread_spinlock *lk);

void thread_spin_unlock(struct thread_spinlock *lk);

// mutex
void thread_mutex_init(struct thread_mutex *m);

void thread_mutex_lock(struct thread_mutex *m);

void thread_mutex_unlock(struct thread_mutex *m);

// conditional variable
struct q
{
  struct thread_cond cv;
  struct thread_mutex m;
  int count;
  void *ptr;
};

void thread_cond_init(struct q *q);

void thread_cond_wait(struct thread_cond *cv, struct thread_mutex *m);

void thread_cond_signal(struct thread_cond *cv);

// Semaphore
void sem_init(struct q *q, uint count);

void sem_wait(struct q *sem);

void sem_post(struct q *sem);
