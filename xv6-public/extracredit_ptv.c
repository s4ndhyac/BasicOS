#include "types.h"
#include "stat.h"
#include "user.h"
#include "thread_lock_impl.h"
#define MAX_THREADS 5

typedef struct balance
{
  char name[32];
  char amount;
} balance_t;

balance_t per_thread_balance[MAX_THREADS];

volatile int total_balance = 0;

struct thread_mutex lock;
struct thread_mutex printLock;

void do_work(void *arg)
{

  int i;
  int old;

  int id = gettid();

  char *name = per_thread_balance[id].name;
  int amount = per_thread_balance[id].amount;

  thread_mutex_lock(&printLock);
  printf(1, "[thread %d: %s] Start to do work. Workload: %d\n", id, name, amount);
  thread_mutex_unlock(&printLock);

  for (i = 0; i < amount; i++)
  {
    thread_mutex_lock(&lock);
    old = total_balance;
    delay(100000);
    total_balance = old + 1;
    thread_mutex_unlock(&lock);
  }

  thread_mutex_lock(&printLock);
  printf(1, "[thread %d: %s] Work done. Finished work: %d\n", id, name, amount);
  thread_mutex_unlock(&printLock);

  thread_exit();

  return;
}

int main(int argc, char *argv[])
{
  char *s[MAX_THREADS];
  int t[MAX_THREADS], r[MAX_THREADS];
  int balance_sum = 0;

  thread_mutex_init(&lock);
  thread_mutex_init(&printLock);

  int i;
  for (i = 0; i < MAX_THREADS; i++)
  {
    char *name = {'b', i, '\0'};
    strcpy(per_thread_balance[i].name, name);
    per_thread_balance[i].amount = (i + 1) * 100;
    balance_sum += per_thread_balance[i].amount;

    // using sbrk instead of malloc to ensure page aligned
    s[i] = sbrk(4096);
    t[i] = thread_create(do_work, (void *)&per_thread_balance[i], (void *)s[i]);
  }

  for (i = 0; i < MAX_THREADS; i++)
    r[i] = thread_join();

  printf(1, "Threads finished: ");
  for (i = 0; i < MAX_THREADS; i++)
  {
    if (i == MAX_THREADS - 1)
      printf(1, "(%d):%d\n", t[i], r[i]);
    else
      printf(1, "(%d):%d, ", t[i], r[i]);
  }

  printf(1, "Shared balance: %d, Balance sum: %d\n", total_balance, balance_sum);

  exit();
}