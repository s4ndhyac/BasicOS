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
  int fileDesc = open("y", O_CREAT | O_RDWR | O_TRUNC, 0777);
  if (fileDesc == -1)
  {
    fprintf(stderr, "Error while opening file y\n");
    exit(EXIT_FAILURE);
  }

  dup2(fileDesc, 1);

  char *args[] = {"ls", NULL};
  execv("/bin/ls", args);

  exit(EXIT_SUCCESS);
}