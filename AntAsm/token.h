#ifndef TOKEN_H
#define TOKEN_H

#include <stddef.h>

enum TokenType {
  Token_Comma,
  Token_SemiColon,
  Token_Identifier,
  Token_Opcode,
  Token_Register,
  Token_LiteralString,
  Token_LiteralNumber,
  Token_LiteralHex,
};

struct Token {
  enum TokenType type;
  unsigned int start;
  unsigned int end;
  unsigned int line;

  char *value;
  const char *filename;
};

struct TokenArray {
  struct Token *tokens;
  size_t size;
  char **line_content;
};

#endif // TOKEN_H
