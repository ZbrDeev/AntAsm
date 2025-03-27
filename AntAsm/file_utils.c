#include "file_utils.h"
#include <stdlib.h>

// Open file in read mode
char *readFile(const char *path) {
  FILE *fp;

  if ((fp = fopen(path, "r")) == NULL) {
    printf("Invalid file path");
    exit(-1);
  }

  // Go to the end of file and calc the size of the file content
  fseek(fp, 0, SEEK_END);
  long filesize = ftell(fp);
  rewind(fp);

  char *buffer = (char *)malloc(filesize + 2);
  buffer[filesize] = '\n';
  buffer[filesize+1] = '\0';

  fread(buffer, filesize, 1, fp);
  fclose(fp);

  return buffer;
}
