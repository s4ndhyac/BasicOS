#include <stdlib.h>
#include <unistd.h>
#include <stdio.h>
#include <fcntl.h>
#include <string.h>
#include <assert.h>
#include <sys/types.h>
#include <sys/stat.h>
#include <sys/wait.h>

// Tested on Andromeda-66 GCC version: 4.8.5

int main(void)
{
  int pipe_1_fds[2];
  int pipe_2_fds[2];

  char *ls_args[] = {"ls", NULL};
  char *grep_args[] = {"grep", "main", NULL};
  char *wc_args[] = {"wc", NULL};

  if (pipe(pipe_2_fds) < 0)
    fprintf(stderr, "Error in pipe 2 system call\n");

  if (fork() == 0)
  {
    if (pipe(pipe_1_fds) < 0)
      fprintf(stderr, "Error in pipe 1 system call\n");
    if (fork() == 0)
    {
      dup2(pipe_1_fds[1], 1);
      close(pipe_1_fds[0]);
      execv("/bin/ls", ls_args);
      exit(EXIT_SUCCESS);
    }

    dup2(pipe_1_fds[0], 0);
    close(pipe_1_fds[1]);
    dup2(pipe_2_fds[1], 1);
    execv("/bin/grep", grep_args);

    exit(EXIT_SUCCESS);
  }
  else
  {
    dup2(pipe_2_fds[0], 0);
    close(pipe_2_fds[1]);
    execv("/usr/bin/wc", wc_args);
    exit(EXIT_SUCCESS);
  }
  exit(EXIT_SUCCESS);
}