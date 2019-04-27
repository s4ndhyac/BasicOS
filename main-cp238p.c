#include <stdlib.h>
#include <unistd.h>
#include <stdio.h>
#include <fcntl.h>
#include <string.h>
#include <assert.h>
#include <sys/types.h>
#include <sys/stat.h>
#include <sys/wait.h>

#define FILE_SIZE 50

// Tested on Andromeda-66 GCC version: 4.8.5

int main(int argc, char *argv[])
{
  if (argc == 3)
  {
    char *src = argv[1];
    char *dest = argv[2];

    int srcFileDesc = open(src, O_RDONLY);
    if (srcFileDesc == -1)
    {
      fprintf(stderr, "Error while opening source file\n");
      exit(1);
    }

    int destFileDesc = open(dest, O_RDWR | O_CREAT | O_TRUNC, 0777);
    if (destFileDesc == -1)
    {
      fprintf(stderr, "Error while opening target file\n");
      exit(1);
    }

    char *fileBuf = malloc(FILE_SIZE * sizeof(char));
    int readFileSize;
    while (1)
    {
      readFileSize = read(srcFileDesc, fileBuf, FILE_SIZE);
      if (readFileSize < 1)
        break;

      if (write(destFileDesc, fileBuf, readFileSize) < 0)
      {
        fprintf(stderr, "Error in writing copied file\n");
        exit(1);
      }
    }

    close(srcFileDesc);
    close(destFileDesc);
  }
  else
  {
    return 1;
  }
  exit(0);
}
