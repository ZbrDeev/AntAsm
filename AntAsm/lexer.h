#ifndef LEXER_H
#define LEXER_H

#include "token.h"
#include <stdbool.h>
#include <string.h>

// Compare String with opcodes
#define OPCODE_CMP(opcode)                                                     \
  (strcmp(opcode, "mov") == 0 || strcmp(opcode, "add") == 0 ||                 \
   strcmp(opcode, "cmp") == 0 || strcmp(opcode, "or") == 0 ||                  \
   strcmp(opcode, "jmp") == 0 || strcmp(opcode, "sub") == 0 ||                 \
   strcmp(opcode, "imul") == 0 || strcmp(opcode, "push") == 0 ||               \
   strcmp(opcode, "pop") == 0 || strcmp(opcode, "inc") == 0 ||                 \
   strcmp(opcode, "dec") == 0 || strcmp(opcode, "and") == 0 ||                 \
   strcmp(opcode, "xor") == 0 || strcmp(opcode, "jo") == 0 ||                  \
   strcmp(opcode, "jno") == 0 || strcmp(opcode, "jb") == 0 ||                  \
   strcmp(opcode, "jnb") == 0 || strcmp(opcode, "jz") == 0 ||                  \
   strcmp(opcode, "jnz") == 0 || strcmp(opcode, "jbe") == 0 ||                 \
   strcmp(opcode, "ja") == 0 || strcmp(opcode, "js") == 0 ||                   \
   strcmp(opcode, "jns") == 0 || strcmp(opcode, "jp") == 0 ||                  \
   strcmp(opcode, "jnp") == 0 || strcmp(opcode, "jl") == 0 ||                  \
   strcmp(opcode, "jnl") == 0 || strcmp(opcode, "jle") == 0 ||                 \
   strcmp(opcode, "jnle") == 0 || strcmp(opcode, "equ") == 0 ||                \
   strcmp(opcode, "syscall") == 0)

// Compare String with registers
#define REGISTER_CMP(register)                                                 \
  (strcmp(register, "rax") == 0 || strcmp(register, "eax") == 0 ||             \
   strcmp(register, "ax") == 0 || strcmp(register, "ah") == 0 ||               \
   strcmp(register, "al") == 0 || strcmp(register, "rbx") == 0 ||              \
   strcmp(register, "ebx") == 0 || strcmp(register, "bx") == 0 ||              \
   strcmp(register, "bh") == 0 || strcmp(register, "bl") == 0 ||               \
   strcmp(register, "rcx") == 0 || strcmp(register, "ecx") == 0 ||             \
   strcmp(register, "cx") == 0 || strcmp(register, "ch") == 0 ||               \
   strcmp(register, "cl") == 0 || strcmp(register, "rdx") == 0 ||              \
   strcmp(register, "edx") == 0 || strcmp(register, "dx") == 0 ||              \
   strcmp(register, "dh") == 0 || strcmp(register, "dl") == 0 ||               \
   strcmp(register, "rsi") == 0 || strcmp(register, "esi") == 0 ||             \
   strcmp(register, "si") == 0 || strcmp(register, "sil") == 0 ||              \
   strcmp(register, "rdi") == 0 || strcmp(register, "edi") == 0 ||             \
   strcmp(register, "di") == 0 || strcmp(register, "dil") == 0 ||              \
   strcmp(register, "rsp") == 0 || strcmp(register, "esp") == 0 ||             \
   strcmp(register, "sp") == 0 || strcmp(register, "spl") == 0 ||              \
   strcmp(register, "rbp") == 0 || strcmp(register, "ebp") == 0 ||             \
   strcmp(register, "bp") == 0 || strcmp(register, "bpl") == 0 ||              \
   strcmp(register, "r8") == 0 || strcmp(register, "r8d") == 0 ||              \
   strcmp(register, "r8w") == 0 || strcmp(register, "r8b") == 0 ||             \
   strcmp(register, "r9") == 0 || strcmp(register, "r9d") == 0 ||              \
   strcmp(register, "r9w") == 0 || strcmp(register, "r9b") == 0 ||             \
   strcmp(register, "r10") == 0 || strcmp(register, "r10d") == 0 ||            \
   strcmp(register, "r10w") == 0 || strcmp(register, "r10b") == 0 ||           \
   strcmp(register, "r11") == 0 || strcmp(register, "r11d") == 0 ||            \
   strcmp(register, "r11w") == 0 || strcmp(register, "r11b") == 0 ||           \
   strcmp(register, "r12") == 0 || strcmp(register, "r12d") == 0 ||            \
   strcmp(register, "r12w") == 0 || strcmp(register, "r12b") == 0 ||           \
   strcmp(register, "r13") == 0 || strcmp(register, "r13d") == 0 ||            \
   strcmp(register, "r13w") == 0 || strcmp(register, "r13b") == 0 ||           \
   strcmp(register, "r14") == 0 || strcmp(register, "r14d") == 0 ||            \
   strcmp(register, "r14w") == 0 || strcmp(register, "r14b") == 0 ||           \
   strcmp(register, "r15") == 0 || strcmp(register, "r15d") == 0 ||            \
   strcmp(register, "r15w") == 0 || strcmp(register, "r15b") == 0)

// Insert token in token_array
#define INSERT_TOKEN()                                                         \
  struct Token *temp = (struct Token *)realloc(                                \
      token_array->tokens, (token_array->size + 1) * sizeof(struct Token));    \
  assert(temp != NULL);                                                        \
  token_array->tokens = temp;                                                  \
  token_array->tokens[token_array->size - 1] = token;                          \
  token_array->tokens[token_array->size].line = 0;                             \
  token_array->tokens[token_array->size].value = NULL;                         \
  ++token_array->size;                                                         \
  keyword_size = 0;

// Check if keyword_size is greater than 0 if yes parse the token before the
// current
#define CHECK_KEYWORD_SIZE()                                                   \
  if (keyword_size > 0) {                                                      \
    lexePart(i, keyword_size, content, token_array, line, column);             \
  }

struct ContentInfo {
  const char *content;
  size_t content_size;
  const char *filename;
};

struct TokenArray *lexer(const struct ContentInfo *content);

void lexePart(const size_t position, const size_t keyword_size,
              const struct ContentInfo *content, struct TokenArray *token_array,
              size_t line, size_t column);

bool isNumber(const char *number);

void freeToken(struct TokenArray *token_array);

#endif // LEXER_H
