#include "types.h"
#include "stat.h"
#include "user.h"

#define BUFFER_SIZE 16

int main(int argc, char *argv[])
{
  /* Syscall invocation here */
  printf(1, "available memory: %d \n", memtop());
  int pid = fork();
  if (pid == 0)
    exit();
  else
    wait();

  int i;
  char *buf = malloc(BUFFER_SIZE * sizeof(char));
  for (i = 1; i <= pid; i++)
  {
    memset(buf, 0, BUFFER_SIZE);
    int mem = getmeminfo(i, buf, BUFFER_SIZE);
    if (mem > 0)
      printf(1, "pid: %d, name: %s, mem: %d \n", i, buf, mem);
  }

  free(buf);
  exit();
}