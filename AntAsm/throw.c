#include "throw.h"
#include <stdio.h>
#include <stdlib.h>

void throwError(int status, const char *reason) {
  printf("%s\n", reason);
  exit(status);
}
