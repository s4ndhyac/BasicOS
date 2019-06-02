struct thread_mutexlock
{
  uint locked;
  struct cpu *cpu; // The cpu holding the lock.
};