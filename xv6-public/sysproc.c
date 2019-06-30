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

int sys_memtop(void)
{
  int freemem = kcount() * PGSIZE;
  return freemem;
}

int sys_getmeminfo(void)
{
  int pid, size;
  char *name;
  if (argint(0, &pid) < 0 || argstr(1, &name) < 0 || argint(2, &size) < 0)
    return -1;
  return getmeminfo(pid, name, size);
}

int sys_thread_create(void)
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
  thread_exit();
  return 0;
}

int sys_thread_sleep(void)
{
  void *chan;
  void *lk;
  if (argptr(0, (void *)&chan, sizeof(void *)) < 0 || argptr(1, (void *)&lk, sizeof(void *)) < 0)
    return -1;
  thread_sleep(chan, lk);
  return 0;
}

int sys_thread_wakeup(void)
{
  void *chan;
  if (argptr(0, (void *)&chan, sizeof(void *)) < 0)
    return -1;
  wakeup(chan);
  return 0;
}
