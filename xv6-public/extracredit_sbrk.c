#include "types.h"
#include "stat.h"
#include "user.h"
#include "thread_lock_impl.h"

typedef struct balance
{
  char name[32];
  char amount;
} balance_t;

struct thread_mutex ml;

void do_sbrk(void *arg)
{
  balance_t *b = (balance_t *)arg;

  thread_mutex_lock(&ml);
  printf(1, "Thread %s sbrk: process size is %d before doing sbrk(%d)\n", b->name, (uint)sbrk(0), b->amount);
  thread_mutex_unlock(&ml);

  sbrk(b->amount);

  thread_mutex_lock(&ml);
  printf(1, "Thread %s sbrk: after sbrk(%d) process size = %d\n", b->name, b->amount, (uint)sbrk(0));
  thread_mutex_unlock(&ml);

  thread_exit();
  return;
}

// void do_malloc(void *arg)
// {

//   balance_t *b = (balance_t *)arg;

//   thread_mutex_lock(&ml);
//   printf(1, "Thread %s malloc: process size is %d before doing malloc(%d)\n", b->name, (uint)sbrk(0), b->amount);
//   thread_mutex_unlock(&ml);

//   malloc(b->amount);

//   thread_mutex_lock(&ml);
//   printf(1, "Thread %s malloc: after malloc(%d) process size = %d\n", b->name, b->amount, (uint)sbrk(0));
//   thread_mutex_unlock(&ml);

//   thread_exit();
//   return;
// }

int main(int argc, char *argv[])
{

  thread_mutex_init(&ml);

  struct balance b1 = {"b1", 50};
  struct balance b2 = {"b2", 100};

  void *s1, *s2;
  int t1, t2, r1, r2;

  s1 = sbrk(4096);
  s2 = sbrk(4096);

  t1 = thread_create(do_sbrk, (void *)&b1, s1);
  t2 = thread_create(do_sbrk, (void *)&b2, s2);

  r1 = thread_join();
  r2 = thread_join();

  thread_mutex_lock(&ml);
  printf(1, "Threads finished: (%d):%d, (%d):%d\n",
         t1, r1, t2, r2);
  printf(1, "process size is %d after all child threads have exited\n", (int)sbrk(0));
  thread_mutex_unlock(&ml);
  exit();
}