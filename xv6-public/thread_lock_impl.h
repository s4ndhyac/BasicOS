#include "thread_spinlock.h"
#include "thread_mutex.h"
#include "thread_util.h"
#include "thread_cond_var.h"

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

// conditional variable
struct q
{
  struct thread_cond cv;
  struct thread_mutex m;
  int num;
  void *ptr;
};

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

// Thread 1 (sender)
void *
send(struct q *q, void *p)
{
  thread_mutex_lock(&q->m);
  while (q->ptr != 0)
    ;
  q->ptr = p;
  thread_cond_signal(&q->cv);
  thread_mutex_unlock(&q->m);
}

// Thread 2 (receiver)

void *
recv(struct q *q)
{
  void *p;

  thread_mutex_lock(&q->m);

  while ((p = q->ptr) == 0)
    thread_cond_wait(&q->cv, &q->m);
  q->ptr = 0;

  thread_mutex_unlock(&q->m);
  return p;
}

// Semaphore
void sem_init(struct q *q, uint num)
{
  q->ptr = 0;
  q->num = num;
  thread_cond_init(q);
}

void sem_wait(struct q *semm)
{
  thread_mutex_lock(&(semm->m));
  while (semm->num == 0)
    thread_cond_wait(&(semm->cv), &(semm->m));
  semm->num--;
  thread_mutex_unlock(&(semm->m));
}

void sem_post(struct q *semm)
{
  thread_mutex_lock(&(semm->m));
  /*fuck! Here is a stupid mistake!Just add the num will be ok!!
	 Otherwise,we will be blocked here！
	 while ( semm->num == 0)
	 pthread_cond_wait(&(semm->cond), &(semm->lock));
	 */
  semm->num++;
  thread_mutex_unlock(&(semm->m));
  thread_cond_signal(&(semm->cv));
}
