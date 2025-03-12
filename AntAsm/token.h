#ifndef TOKEN_H
#define TOKEN_H

#include <stddef.h>

enum TokenType {
  Comma,
  SemiColon,
  Identifier,
  Opcode,
  Register,
  LiteralString,
  LiteralNumber,
  LiteralHex,
};

struct Token {
  enum TokenType type;
  unsigned int start;
  unsigned int end;
  unsigned int line;

  const char *value;
  const char *file;
};

struct TokenArray {
  struct Token *tokens;
  size_t size;
};

#endif // TOKEN_H
