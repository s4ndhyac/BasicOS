#include "types.h"
#include "stat.h"
#include "user.h"
#include "thread_lock_impl.h"

#define NUM 128
#define MAX_NUM 20

struct balance
{
  char name[32];
  int amount;
};

volatile int total_balance = 0;

int queue[NUM];
struct q producer;
struct q consumer;
struct thread_mutex printLock;
void prod_work(void *arg)
{
  int p = 0;
  int count = 0;
  while (1)
  {
    if (count > MAX_NUM)
      break;
    sem_wait(&consumer);
    thread_mutex_lock(&printLock);
    count++;
    queue[p] = count;
    printf(1, "Produce %d\n", queue[p]);
    thread_mutex_unlock(&printLock);
    sem_post(&producer);
    p = (p + 1) % NUM;
    sleep(1);
  }

  thread_exit();
  return;
}

void cons_work(void *arg)
{
  int c = 0;
  int count = 0;
  while (1)
  {
    if (count > MAX_NUM)
      break;
    sem_wait(&producer);
    thread_mutex_lock(&printLock);
    printf(1, "Consume %d\n", queue[c]);
    count++;
    queue[c] = 0;
    thread_mutex_unlock(&printLock);
    sem_post(&consumer);
    c = (c + 1) % NUM;
    sleep(3);
  }
  thread_exit();
  return;
}

int main(int argc, char *argv[])
{
  printf(1, "Semaphores test:\n");
  thread_mutex_init(&printLock);
  sem_init(&consumer, NUM);
  sem_init(&producer, 0);
  void *s1, *s2;
  int t1, t2, r1, r2;
  struct balance b1 = {"b1", 10};
  struct balance b2 = {"b2", 10};
  s1 = malloc(4096);
  s2 = malloc(4096);
  t1 = thread_create(prod_work, (void *)&b1, s1);
  t2 = thread_create(cons_work, (void *)&b2, s2);
  r1 = thread_join();
  r2 = thread_join();
  printf(1, "Threads finished: (%d):%d, (%d):%d, shared balance:%d\n", t1, r1,
         t2, r2, total_balance);
  free(s1);
  free(s2);
  exit();
}