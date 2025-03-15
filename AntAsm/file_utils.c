#include "file_utils.h"
#include <stdlib.h>

char *readFile(const char *path) {
  FILE *fp;

  if ((fp = fopen(path, "r")) == NULL) {
    // TODO: HANDLE ERROR
  }

  fseek(fp, 0, SEEK_END);
  long filesize = ftell(fp);
  rewind(fp);

  char *buffer = (char *)malloc(filesize + 1);
  buffer[filesize] = '\0';

  fread(buffer, filesize, 1, fp);
  fclose(fp);

  return buffer;
}
