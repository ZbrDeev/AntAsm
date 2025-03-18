#include "throw.h"
#include <stdio.h>
#include <stdlib.h>
#include <string.h>

void printError(int status, const char *reason, const char *filename,
                const char *line_content, int line, int start, int end) {
  const char *error_title;

  if (status < 100) {
    error_title = "Syntax error";
  } else if (status < 200) {
    error_title = "Not implemented yet";
  } else {
    error_title = "Internal error";
  }

  printf("\033[1;38;5;196m Error:\033[0m \033[1m%s:%d:%d:\033[0m %s\n",
         filename, line, start, error_title);

  printf("  |\n");
  printf("\033[33m%d\033[0m | ", line);

  size_t line_size = strlen(line_content);
  char *error_arrow = (char *)malloc(line_size + 1);
  error_arrow[line_size] = '\0';

  for (size_t i = 0; i < line_size; ++i) {
    if (i >= start - 1 && i <= end - 1) {
      printf("\033[1;38;5;196m%c\033[0m", line_content[i]);
      error_arrow[i] = '^';
    } else {
      printf("%c", line_content[i]);
      error_arrow[i] = ' ';
    }
  }

  size_t reason_size = strlen(reason);
  char *error_info = (char *)malloc(4 + start + reason_size);
  error_info[0] = '\0';

  char *space_string = (char *)malloc(4 + start);
  memset(space_string, ' ', 3 + start);
  space_string[3 + start] = '\0';

  strcat(error_info, space_string);
  strcat(error_info, reason);

  free(space_string);

  printf("\n  | \033[1;38;5;196m%s\033[0m\n", error_arrow);
  printf("%s\n", error_info);

  free(error_arrow);
  free(error_info);
}

void throwError(int status, const char *reason, const char *filename,
                const char *line_content, int line, int start, int end) {
  printError(status, reason, filename, line_content, line, start, end);
  exit(status);
}

void throwAndFreeToken(int status, const char *reason,
                       void(free_token)(struct TokenArray *),
                       struct TokenArray *token_array, size_t i,
                       struct MemberList *member_list) {
  struct Token token = token_array->tokens[i];

  printError(status, reason, token.filename,
             token_array->line_content[token.line - 1], token.line, token.start,
             token.end);
  free(member_list);
  free_token(token_array);
  exit(status);
}
