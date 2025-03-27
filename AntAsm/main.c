#include "repl.h"
#include "run.h"
#include <stdio.h>

int main(int argc, char **argv) {
  int status_code = 0;

  if (argc == 1) {
    status_code = runRepl();
  } else if (argc == 2) {
    status_code = doAllProcess(argv[1]);
  } else {
    printf("Usage: AntAsm\n");
    printf("Or\n");
    printf("AntAsm <file>.asm\n");
    return -1;
  }

  return status_code;
}
