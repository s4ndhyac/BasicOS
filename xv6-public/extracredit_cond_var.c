#include "types.h"
#include "stat.h"
#include "user.h"
#include "thread_lock_impl.h"

char *buffer;
struct q messageQ;

struct balance
{
  char name[32];
  int amount;
};

volatile int total_balance = 0;

volatile unsigned int delay(unsigned int d)
{
  unsigned int i;
  for (i = 0; i < d; i++)
  {
    __asm volatile("nop" ::
                       :);
  }

  return i;
}

void write_work(void *arg)
{
  char *newMessage = "Hello, Multi-thread world!";
  struct balance *b = (struct balance *)arg;
  printf(1, "I send message: %s\n", newMessage);
  printf(1, "send message by s:%x\n", b->name);
  send(&messageQ, newMessage);
  thread_exit();
  return;
}

void read_work(void *arg)
{
  char *receiveMessage = recv(&messageQ);
  struct balance *b = (struct balance *)arg;
  printf(1, "receive message by s:%x\n", b->name);
  printf(1, "I receive message: %s\n", receiveMessage);
  thread_exit();
  return;
}

int main(int argc, char *argv[])
{
  printf(1, "Conditional variable test:\n");
  thread_cond_init(&messageQ);
  buffer = "hello world";
  struct balance b1 = {"b1", 10};
  struct balance b2 = {"b2", 10};
  void *s1, *s2;
  int t1, t2, r1, r2;
  s1 = malloc(4096);
  s2 = malloc(4096);
  t1 = thread_create(write_work, (void *)&b1, s1);
  t2 = thread_create(read_work, (void *)&b2, s2);
  r1 = thread_join();
  r2 = thread_join();
  printf(1, "Threads finished: (%d):%d, (%d):%d, shared balance:%d\n", t1, r1,
         t2, r2, total_balance);
  free(s1);
  free(s2);
  exit();
}