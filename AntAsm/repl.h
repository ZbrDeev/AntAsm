#ifndef REPL_H
#define REPL_H

#include "run.h"
#include "token.h"

#define MAX_INPUT_BUFFER 512

int runRepl();

int parseReplLine(size_t code_size, char *code_line,
                  struct RegisterEmu *register_emu);
#endif // REPL_H
