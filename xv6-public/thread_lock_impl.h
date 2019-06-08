#include "thread_spinlock.h"
#include "thread_mutex.h"
#include "thread_util.h"
#include "thread_cond_var.h"
#include "thread_lock.h"

// spinlock
void thread_spin_init(struct thread_spinlock *lk)
{
  lk->locked = 0;
}

void thread_spin_lock(struct thread_spinlock *lk)
{
  // The xchg is atomic.
  while (xchg(&lk->locked, 1) != 0)
    ;

  // Tell the C compiler and the processor to not move loads or stores
  // past this point, to ensure that the critical section's memory
  // references happen after the lock is acquired.
  __sync_synchronize();
}

void thread_spin_unlock(struct thread_spinlock *lk)
{
  // Tell the C compiler and the processor to not move loads or stores
  // past this point, to ensure that all the stores in the critical
  // section are visible to other cores before the lock is released.
  // Both the C compiler and the hardware may re-order loads and
  // stores; __sync_synchronize() tells them both not to.
  __sync_synchronize();

  // Release the lock, equivalent to lk->locked = 0.
  // This code can't use a C assignment, since it might
  // not be atomic. A real OS would use C atomics here.
  asm volatile("movl $0, %0"
               : "+m"(lk->locked)
               :);
}

// mutex
void thread_mutex_init(struct thread_mutex *m)
{
  m->locked = 0;
}

void thread_mutex_lock(struct thread_mutex *m)
{
  // The xchg is atomic.
  while (xchg(&m->locked, 1) != 0)
    sleep(1);

  // Tell the C compiler and the processor to not move loads or stores
  // past this point, to ensure that the critical section's memory
  // references happen after the lock is acquired.
  __sync_synchronize();
}

void thread_mutex_unlock(struct thread_mutex *m)
{
  // Tell the C compiler and the processor to not move loads or stores
  // past this point, to ensure that all the stores in the critical
  // section are visible to other cores before the lock is released.
  // Both the C compiler and the hardware may re-order loads and
  // stores; __sync_synchronize() tells them both not to.
  __sync_synchronize();

  // Release the lock, equivalent to lk->locked = 0.
  // This code can't use a C assignment, since it might
  // not be atomic. A real OS would use C atomics here.
  asm volatile("movl $0, %0"
               : "+m"(m->locked)
               :);
}

void thread_cond_init(struct q *q)
{
  q->cv.cond = 0;
  thread_mutex_init(&q->m);
}

void thread_cond_wait(struct thread_cond *cv, struct thread_mutex *m)
{
  thread_sleep((void *)&cv->cond, (void *)m);
}

void thread_cond_signal(struct thread_cond *cv)
{
  thread_wakeup((void *)&cv->cond);
}

// Semaphore
void sem_init(struct q *q, uint count)
{
  q->ptr = 0;
  q->count = count;
  thread_cond_init(q);
}

void sem_wait(struct q *sem)
{
  thread_mutex_lock(&(sem->m));
  while (sem->count == 0)
    thread_cond_wait(&(sem->cv), &(sem->m));
  sem->count--;
  thread_mutex_unlock(&(sem->m));
}

void sem_post(struct q *sem)
{
  thread_mutex_lock(&(sem->m));
  sem->count++;
  thread_mutex_unlock(&(sem->m));
  thread_cond_signal(&(sem->cv));
}
