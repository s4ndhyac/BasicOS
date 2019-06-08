#include "types.h"
#include "stat.h"
#include "user.h"
#include "thread_lock_impl.h"

struct q msgQ;

struct balance
{
  char name[32];
  int amount;
};

// Thread 1 (sender)
void *send(struct q *q, void *p)
{
  thread_mutex_lock(&q->m);
  while (q->ptr != 0)
    ;
  q->ptr = p;
  thread_cond_signal(&q->cv);
  thread_mutex_unlock(&q->m);
  return p;
}

// Thread 2 (receiver)

void *recv(struct q *q)
{
  void *p;

  thread_mutex_lock(&q->m);

  while ((p = q->ptr) == 0)
    thread_cond_wait(&q->cv, &q->m);
  q->ptr = 0;

  thread_mutex_unlock(&q->m);
  return p;
}

void send_msg(void *arg)
{
  char *msg = "Hello world!";
  struct balance *b = (struct balance *)arg;
  printf(1, "Send message: %s\n", msg);
  printf(1, "Send message by s:%x\n", b->name);
  send(&msgQ, msg);
  thread_exit();
  return;
}

void receive_msg(void *arg)
{
  char *r_msg = recv(&msgQ);
  struct balance *b = (struct balance *)arg;
  printf(1, "Receive message by s:%x\n", b->name);
  printf(1, "Receive message: %s\n", r_msg);
  thread_exit();
  return;
}

int main(int argc, char *argv[])
{
  printf(1, "Extracredit: Conditional variable:\n");
  thread_cond_init(&msgQ);
  struct balance b1 = {"b1", 0};
  struct balance b2 = {"b2", 0};
  void *s1, *s2;
  int t1, t2, r1, r2;
  s1 = malloc(4096);
  s2 = malloc(4096);
  t1 = thread_create(send_msg, (void *)&b1, s1);
  t2 = thread_create(receive_msg, (void *)&b2, s2);
  r1 = thread_join();
  r2 = thread_join();
  printf(1, "Threads finished: (%d):%d, (%d):%d\n", t1, r1,
         t2, r2);
  free(s1);
  free(s2);
  exit();
}