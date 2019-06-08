#include "types.h"
#include "stat.h"
#include "user.h"
#include "thread_lock_impl.h"

#define N 5

struct balance
{
  char name[32];
  int amount;
};

int buffer[N];
int buffer_index;
struct q producer;
struct q consumer;
struct thread_mutex mLock;

void insertbuffer(int value)
{
  if (buffer_index < N)
  {
    buffer[buffer_index++] = value;
  }
  else
  {
    printf(1, "Buffer overflow\n");
  }
}

int dequeuebuffer()
{
  if (buffer_index > 0)
  {
    return buffer[--buffer_index];
  }
  else
  {
    printf(1, "Buffer underflow\n");
  }
  return 0;
}

void prod_work(void *arg)
{
  int i = 0;
  struct balance *b = (struct balance *)arg;
  while (i++ < N)
  {
    sem_wait(&consumer);
    thread_mutex_lock(&mLock);
    insertbuffer(i);
    thread_mutex_unlock(&mLock);
    sem_post(&producer);
    printf(1, "Producer %s added %d to buffer\n", b->name, i);
  }
  thread_exit();
  return;
}

void cons_work(void *arg)
{
  struct balance *b = (struct balance *)arg;
  int i = 0;
  int value;
  while (i++ < N)
  {
    sem_wait(&producer);
    thread_mutex_lock(&mLock);
    value = dequeuebuffer();
    thread_mutex_unlock(&mLock);
    sem_post(&consumer);
    printf(1, "Consumer %s dequeue %d from buffer\n", b->name, value);
  }
  thread_exit();
  return;
}

int main(int argc, char *argv[])
{
  printf(1, "Extracredit Semaphore:\n");
  buffer_index = 0;
  thread_mutex_init(&mLock);
  sem_init(&consumer, N);
  sem_init(&producer, 0);
  void *s1, *s2;
  int t1, t2, r1, r2;
  struct balance b1 = {"b1", 0};
  struct balance b2 = {"b2", 0};
  s1 = malloc(4096);
  s2 = malloc(4096);
  t1 = thread_create(prod_work, (void *)&b1, s1);
  t2 = thread_create(cons_work, (void *)&b2, s2);
  r1 = thread_join();
  r2 = thread_join();
  printf(1, "Threads finished: (%d):%d, (%d):%d\n", t1, r1,
         t2, r2);
  free(s1);
  free(s2);
  exit();
}