#ifndef LEXER_H
#define LEXER_H

#include "token.h"
#include <stdbool.h>
#include <string.h>

#define OPCODE_CMP(opcode)                                                     \
  (strcmp(opcode, "mov") || strcmp(opcode, "add") || strcmp(opcode, "cmp") ||  \
   strcmp(opcode, "or") || strcmp(opcode, "jmp") || strcmp(opcode, "sub") ||   \
   strcmp(opcode, "imul") || strcmp(opcode, "push") ||                         \
   strcmp(opcode, "pop") || strcmp(opcode, "inc") || strcmp(opcode, "dec") ||  \
   strcmp(opcode, "and") || strcmp(opcode, "xor") || strcmp(opcode, "jo") ||   \
   strcmp(opcode, "jno") || strcmp(opcode, "jb") || strcmp(opcode, "jnb") ||   \
   strcmp(opcode, "jz") || strcmp(opcode, "jnz") || strcmp(opcode, "jbe") ||   \
   strcmp(opcode, "ja") || strcmp(opcode, "js") || strcmp(opcode, "jns") ||    \
   strcmp(opcode, "jp") || strcmp(opcode, "jnp") || strcmp(opcode, "jl") ||    \
   strcmp(opcode, "jnl") || strcmp(opcode, "jle") || strcmp(opcode, "jnle") || \
   strcmp(opcode, "equ"))

#define REGISTER_CMP(register)                                                 \
  (strcmp(register, "rax") || strcmp(register, "eax") ||                       \
   strcmp(register, "ax") || strcmp(register, "ah") ||                         \
   strcmp(register, "al") || strcmp(register, "rbx") ||                        \
   strcmp(register, "ebx") || strcmp(register, "bx") ||                        \
   strcmp(register, "bh") || strcmp(register, "bl") ||                         \
   strcmp(register, "rcx") || strcmp(register, "ecx") ||                       \
   strcmp(register, "cx") || strcmp(register, "ch") ||                         \
   strcmp(register, "cl") || strcmp(register, "rdx") ||                        \
   strcmp(register, "edx") || strcmp(register, "dx") ||                        \
   strcmp(register, "dh") || strcmp(register, "dl") ||                         \
   strcmp(register, "rsi") || strcmp(register, "esi") ||                       \
   strcmp(register, "si") || strcmp(register, "sil") ||                        \
   strcmp(register, "rdi") || strcmp(register, "edi") ||                       \
   strcmp(register, "di") || strcmp(register, "dil") ||                        \
   strcmp(register, "rsp") || strcmp(register, "esp") ||                       \
   strcmp(register, "sp") || strcmp(register, "spl") ||                        \
   strcmp(register, "rbp") || strcmp(register, "ebp") ||                       \
   strcmp(register, "bp") || strcmp(register, "bpl") ||                        \
   strcmp(register, "r8") || strcmp(register, "r8d") ||                        \
   strcmp(register, "r8w") || strcmp(register, "r8b") ||                       \
   strcmp(register, "r9") || strcmp(register, "r9d") ||                        \
   strcmp(register, "r9w") || strcmp(register, "r9b") ||                       \
   strcmp(register, "r10") || strcmp(register, "r10d") ||                      \
   strcmp(register, "r10w") || strcmp(register, "r10b") ||                     \
   strcmp(register, "r11") || strcmp(register, "r11d") ||                      \
   strcmp(register, "r11w") || strcmp(register, "r11b") ||                     \
   strcmp(register, "r12") || strcmp(register, "r12d") ||                      \
   strcmp(register, "r12w") || strcmp(register, "r12b") ||                     \
   strcmp(register, "r13") || strcmp(register, "r13d") ||                      \
   strcmp(register, "r13w") || strcmp(register, "r13b") ||                     \
   strcmp(register, "r14") || strcmp(register, "r14d") ||                      \
   strcmp(register, "r14w") || strcmp(register, "r14b") ||                     \
   strcmp(register, "r15") || strcmp(register, "r15d") ||                      \
   strcmp(register, "r15w") || strcmp(register, "r15b"))

struct ContentInfo {
  const char *content;
  size_t content_size;
  const char *filename;
};

struct TokenArray lexer(const struct ContentInfo *content);

void lexePart(const unsigned int position, const unsigned int keyword_size,
              const char *content, struct TokenArray *array);

bool isNumber(const char *number);

#endif // LEXER_H
