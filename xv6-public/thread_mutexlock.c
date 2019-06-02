#include "types.h"
#include "defs.h"
#include "param.h"
#include "x86.h"
#include "memlayout.h"
#include "mmu.h"
#include "proc.h"
#include "thread_mutexlock.h"

void thread_mutex_init(struct thread_mutexlock *m)
{
  m->locked = 0;
  m->cpu = 0;
}

void thread_mutex_lock(struct thread_mutexlock *m)
{
  pushcli(); // disable interrupts to avoid deadlock.
  if (holding_mutex(m))
    panic("acquire");

  // The xchg is atomic.
  while (xchg(&m->locked, 1) != 0)
    yield();

  // Tell the C compiler and the processor to not move loads or stores
  // past this point, to ensure that the critical section's memory
  // references happen after the lock is acquired.
  __sync_synchronize();

  // Record info about lock acquisition for debugging.
  m->cpu = mycpu();
}

void thread_mutex_unlock(struct thread_mutexlock *m)
{
  if (!holding_mutex(m))
    panic("release");
  m->cpu = 0;
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

  popcli();
}

// Check whether this cpu is holding the lock.
int holding_mutex(struct thread_mutexlock *lock)
{
  int r;
  pushcli();
  r = lock->locked && lock->cpu == mycpu();
  popcli();
  return r;
}