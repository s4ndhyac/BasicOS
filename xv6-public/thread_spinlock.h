// Mutual exclusion spinlock
struct thread_spinlock
{
  uint locked;
  struct cpu *cpu; // The cpu holding the lock.
};
