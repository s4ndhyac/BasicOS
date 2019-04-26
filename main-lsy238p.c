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
  int ydesc = open("y", O_RDWR | O_CREAT | O_TRUNC, 0777);
  if (ydesc == -1)
  {
    perror("Error in opening y file");
    exit(1);
  }
  dup2(ydesc, 1);
  char *loc[2];
  loc[0] = "ls";
  loc[1] = NULL;
  execv("/bin/ls", loc);
  exit(0);
}