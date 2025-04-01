#include "repl.h"
#include "ast.h"
#include "information.h"
#include "lexer.h"
#include "parser.h"
#include "token.h"
#include <assert.h>
#include <signal.h>
#include <stdbool.h>
#include <stdio.h>
#include <stdlib.h>
#include <string.h>

static void freeRegisterRepl(struct RegisterEmu *register_emu) {
  freeHashMap(&register_emu->hashmap);
  free(register_emu->stack.node);
  freeBst(register_emu->memory);
  freeBst(register_emu->symbol);
}

int parseReplLine(size_t code_size, char *code_line,
                  struct RegisterEmu *register_emu) {
  int status_code = 0;
  const struct ContentInfo content = {
      .content_size = code_size, .content = code_line, .filename = NULL};

  struct TokenArray *token_array = lexer(&content);

  if (token_array == NULL) {
    register_emu->exit = true;
    return -1;
  }

  struct Program *program = parse(token_array, register_emu->symbol);

  if (program->member_list[0].member_list_type == MemberList_OperationMember) {
    manageOperationType(program->member_list[0].member_list.operation_member,
                        register_emu, 0);
  } else if (program->member_list[0].member_list_type ==
             MemberList_LabelMember) {
    manageOperationType(
        program->member_list[0].member_list.label_member.operation_member,
        register_emu, 0);
  }

  if (register_emu->exit) {
    status_code = register_emu->reg_DI;
  }

  free(program->member_list);
  freeToken(token_array);

  return status_code;
}

static void changeExitState(int __attribute__((unused)) sig) {
  printf("Please press Ctrl-D to exit the REPL.\n");
}

static void printHelp(void) {
  printf("/exit \t Exit the REPL\n");
  printf("/help \t Get the help page\n");
}

int runRepl(void) {
  struct RegisterEmu register_emu;
  INIT_REGISTER_EMU(register_emu)
  INIT_HASHMAP_REGISTER_EMU(register_emu)
  SET_ZERO_FLAGS(register_emu.flags)
  register_emu.stack.node =
      (struct StackNode *)malloc(sizeof(struct StackNode));
  register_emu.stack.last = 1;
  register_emu.memory = NULL;
  register_emu.symbol = NULL;

  printf("Welcome to AntAsm %d.%d.%d.\n", MAJOR_VERSION, MINOR_VERSION,
         PATCH_VERSION);
  printf("Type /help for more information.\n");

  int status_code = 0;
  char buffer[MAX_INPUT_BUFFER];

  signal(SIGINT, changeExitState);

  while (true) {
    memset(buffer, '\0', MAX_INPUT_BUFFER);
    printf(">> ");

    fgets(buffer, MAX_INPUT_BUFFER, stdin);

    size_t buffer_len = strlen(buffer);
    char buffer_cmp[MAX_INPUT_BUFFER];
    strcpy(buffer_cmp, buffer);
    buffer_cmp[strcspn(buffer_cmp, "\n")] = 0;

    if (buffer_len > 0) {
      if (strcmp(buffer_cmp, "/exit") == 0) {
        break;
      } else if (strcmp(buffer_cmp, "/help") == 0) {
        printHelp();
      } else {
        status_code = parseReplLine(buffer_len, buffer, &register_emu);

        if (register_emu.exit) {
          break;
        }
      }
    } else {
      printf("\n");
      break;
    }
  }

  freeRegisterRepl(&register_emu);

  return status_code;
}
