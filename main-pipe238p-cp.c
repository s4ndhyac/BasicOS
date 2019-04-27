#include <stdlib.h>
#include <unistd.h>
#include <stdio.h>
#include <fcntl.h>
#include <string.h>
#include <assert.h>
#include <sys/types.h>
#include <sys/stat.h>
#include <sys/wait.h>

int main(void)
{
  pid_t child_1;
  pid_t child_2;
  int pipe_1_fds[2];
  int pipe_2_fds[2];

  if (pipe(pipe_2_fds) < 0)
    fprintf(stderr, "Error in pipe_1_fds system call\n");

  if (pipe(pipe_1_fds) < 0)
    fprintf(stderr, "Error in pipe_2_fds system call\n");

  if (fork() == 0)
  {
    if (fork() == 0)
    {
      dup2(pipe_1_fds[1], 1);
      close(pipe_1_fds[0]);
      execl("/bin/ls", "ls", NULL);
      exit(0);
    }

    dup2(pipe_1_fds[0], 0);
    close(pipe_1_fds[1]);
    dup2(pipe_2_fds[1], 1);
    execl("/bin/grep", "grep", "main", NULL);

    exit(0);
  }
  else
  {
    dup2(pipe_2_fds[0], 0);
    close(pipe_2_fds[1]);
    execlp("/usr/bin/wc", "wc", NULL);
    exit(0);
  }
  exit(0);
}