#include "types.h"
#include "stat.h"
#include "user.h"

#define BUFFER_SIZE 50

int main(int argc, char *argv[])
{
  /* Syscall invocation here */
  printf(1, "Available memory: %d \n", memtop());
  int pid = fork();
  if (pid != 0)
  {
    printf(1, "Parent PID: %d \n", getpid());
    printf(1, "Child PID: %d \n", pid);
  }

  char *buf = malloc(BUFFER_SIZE * sizeof(char));
  printf(1, "Available memory: %d \n", getmeminfo(1, buf, 50));
  free(buf);
  exit();
}