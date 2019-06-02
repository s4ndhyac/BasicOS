struct thread_mutexlock
{
  uint locked;
  struct cpu *cpu; // The cpu holding the lock.
};

void thread_mutex_init(struct thread_mutexlock *);
void thread_mutex_lock(struct thread_mutexlock *);
void thread_mutex_unlock(struct thread_mutexlock *);
int holding_mutex(struct thread_mutexlock *);