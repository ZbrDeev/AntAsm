#include "file_utils.h"
#include <stdio.h>
#include <stdlib.h>

// Open file in read mode
char *readFile(const char *path) {
  FILE *fp;

  if ((fp = fopen(path, "r")) == NULL) {
    printf("Invalid file path\n");
    return NULL;
  }

  // Go to the end of file and calc the size of the file content
  if (fseek(fp, 0, SEEK_END) != 0) {
    printf("Failed to read the file\n");
    fclose(fp);
    return NULL;
  }

  long filesize = ftell(fp);

  if (filesize < 3) {
    printf("Empty file detected\n");
    fclose(fp);
    return NULL;
  }

  rewind(fp);

  char *buffer = (char *)malloc(filesize + 2);
  buffer[filesize] = '\n';
  buffer[filesize + 1] = '\0';

  fread(buffer, filesize, 1, fp);
  fclose(fp);

  return buffer;
}
