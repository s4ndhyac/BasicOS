#include "types.h"
#include "stat.h"
#include "user.h"
#include "thread_lock_impl.h"

#define THREAD_NUM 10

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

  char *mem = sbrk(b->amount);

  thread_mutex_lock(&ml);
  printf(1, "Thread %s sbrk: after sbrk(%d) process size = %d and address returned = %d\n", b->amount, b->name, (uint)sbrk(0), (uint)mem);
  thread_mutex_unlock(&ml);

  thread_exit();
  return;
}

void do_malloc(void *arg)
{

  balance_t *b = (balance_t *)arg;

  thread_mutex_lock(&ml);
  printf(1, "Thread %s malloc: process size is %d before doing malloc(%d)\n", b->name, (uint)sbrk(0), b->amount);
  thread_mutex_unlock(&ml);

  void *mem = malloc(b->amount);

  thread_mutex_lock(&ml);
  printf(1, "Thread %s malloc: after malloc(%d) process size = %d and address retured = %d\n", b->amount, b->name, (uint)sbrk(0), (uint)mem);
  thread_mutex_unlock(&ml);

  thread_exit();
  return;
}

int main(int argc, char *argv[])
{

  thread_mutex_init(&ml);

  struct balance b1 = {"b1", 50};
  struct balance b2 = {"b2", 100};
  struct balance b3 = {"b1", 150};
  struct balance b4 = {"b2", 200};

  void *s1, *s2, *s3, *s4;
  int t1, t2, t3, t4, r1, r2, r3, r4;

  s1 = sbrk(4096);
  s2 = sbrk(4096);
  s3 = sbrk(4096);
  s4 = sbrk(4096);

  t1 = thread_create(do_sbrk, (void *)&b1, s1);
  t2 = thread_create(do_malloc, (void *)&b2, s2);
  t3 = thread_create(do_sbrk, (void *)&b3, s3);
  t4 = thread_create(do_malloc, (void *)&b4, s4);

  r1 = thread_join();
  r2 = thread_join();
  r3 = thread_join();
  r4 = thread_join();

  thread_mutex_lock(&ml);
  printf(1, "Threads finished: (%d):%d, (%d):%d, (%d):%d, (%d):%d\n",
         t1, r1, t2, r2, t3, r3, t4, r4);
  printf(1, "process size is %d after all child threads have exited\n", (int)sbrk(0));
  thread_mutex_unlock(&ml);
  exit();
}