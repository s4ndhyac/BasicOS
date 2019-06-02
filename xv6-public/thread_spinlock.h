// Mutual exclusion spinlock
struct thread_spinlock
{
  uint locked;
  struct cpu *cpu; // The cpu holding the lock.
};

void thread_spin_init(struct thread_spinlock *);
void thread_spin_lock(struct thread_spinlock *);
void thread_spin_unlock(struct thread_spinlock *);
int holding_spinlock(struct thread_spinlock *);
