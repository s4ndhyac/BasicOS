#include "types.h"
#include "x86.h"
#include "defs.h"
#include "date.h"
#include "param.h"
#include "memlayout.h"
#include "mmu.h"
#include "proc.h"

int sys_fork(void)
{
  return fork();
}

int sys_exit(void)
{
  exit();
  return 0; // not reached
}

int sys_wait(void)
{
  return wait();
}

int sys_kill(void)
{
  int pid;

  if (argint(0, &pid) < 0)
    return -1;
  return kill(pid);
}

int sys_getpid(void)
{
  return myproc()->pid;
}

int sys_sbrk(void)
{
  int addr;
  int n;

  if (argint(0, &n) < 0)
    return -1;
  addr = myproc()->sz;
  if (growproc(n) < 0)
    return -1;
  return addr;
}

int sys_sleep(void)
{
  int n;
  uint ticks0;

  if (argint(0, &n) < 0)
    return -1;
  acquire(&tickslock);
  ticks0 = ticks;
  while (ticks - ticks0 < n)
  {
    if (myproc()->killed)
    {
      release(&tickslock);
      return -1;
    }
    sleep(&ticks, &tickslock);
  }
  release(&tickslock);
  return 0;
}

// return how many clock tick interrupts have occurred
// since start.
int sys_uptime(void)
{
  uint xticks;

  acquire(&tickslock);
  xticks = ticks;
  release(&tickslock);
  return xticks;
}

int sys_thread_create(void (*fcn)(void *), void *arg, void *stack)
{
  void (*fcn)(void *);
  void *arg;
  void *stack;
  if (argptr(0, (void *)&fcn, sizeof(void (*)(void *))) < 0 || argptr(1, (void *)&arg, sizeof(void *)) < 0 || argptr(2, (void *)&stack, sizeof(void *)) < 0)
    return -1;
  return thread_create(fcn, arg, stack);
}

int sys_thread_join(void)
{
  return thread_join();
}

int sys_thread_exit(void)
{
  return thread_exit();
}

void sys_thread_spin_init(struct thread_spinlock *lk)
{
}

void sys_thread_spin_lock(struct thread_spinlock *lk)
{
}

void sys_thread_spin_unlock(struct thread_spinlock *lk)
{
}

void sys_thread_mutex_init(struct thread_mutex *m)
{
}

void sys_thread_mutex_lock(struct thread_mutex *m)
{
}

void sys_thread_mutex_unlock(struct thread_mutex *m)
{
}
