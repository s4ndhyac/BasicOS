#include "thread_spinlock.h"
#include "thread_util.h"

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