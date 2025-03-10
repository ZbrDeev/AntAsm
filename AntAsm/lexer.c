#include "lexer.h"
#include "token.h"
#include <stdio.h>
#include <stdlib.h>

struct TokenArray lexer(const struct ContentInfo *content) {
  struct TokenArray token_array;
  unsigned int keyword_size = 0;

  unsigned int column = 0;
  unsigned int line = 0;

  bool is_string = false;
  char guillemet = '\0';

  for (size_t i = 0; i < content->content_size; ++i) {
    const char c = content->content[i];
    ++column;

    if (is_string && c != guillemet) {
      ++keyword_size;
      continue;
    } else if (is_string && c == guillemet) {
      char *string_value = (char *)malloc((keyword_size + 1) * sizeof(char));

      for (size_t j = 0; j < keyword_size; ++j) {
        unsigned int text_position = i - keyword_size + j;
        string_value[j] = content->content[text_position];
      }

      struct Token token;
      token.type = LiteralString;
      token.value = string_value;
      token.line = line;
      token.start = i - keyword_size - 1;
      token.end = i;
      token.file = content->filename;

      token_array.tokens = (struct Token *)realloc(
          token_array.tokens, (token_array.size + 1) * sizeof(char));
      token_array.tokens[token_array.size++] = token;

      is_string = false;
      guillemet = '\0';

      continue;
    }

    if (c == ' ' || c == '\t') {
      if (keyword_size > 0) {
        lexePart(i - 1, keyword_size, content->content, &token_array);
      }
      continue;
    } else if ((c >= 'A' && c <= 'Z') || (c >= 'a' && c <= 'z') ||
               (c >= '0' && c <= '9')) {
      ++keyword_size;
    } else if (c == '\n') {
      if (keyword_size > 0) {
        lexePart(i - 1, keyword_size, content->content, &token_array);
      }

      column = 0;
      ++line;
    } else if (c == ',') {
      if (keyword_size > 0) {
        lexePart(i - 1, keyword_size, content->content, &token_array);
      }

      struct Token token;
      token.type = Comma;

      token_array.tokens = (struct Token *)realloc(
          token_array.tokens, (token_array.size + 1) * sizeof(struct Token));
      token_array.tokens[token_array.size++] = token;
    } else if (c == ':') {
      if (keyword_size > 0) {
        lexePart(i - 1, keyword_size, content->content, &token_array);
      }

      struct Token token;
      token.type = SemiColon;

      token_array.tokens = (struct Token *)realloc(
          token_array.tokens, (token_array.size + 1) * sizeof(struct Token));
      token_array.tokens[token_array.size++] = token;
    } else if (c == '\'' || c == '"') {
      is_string = true;
      guillemet = c;
    } else {
      ++keyword_size;
    }
  }

  return token_array;
}

void lexePart(const unsigned int position, const unsigned int keyword_size,
              const char *content, struct TokenArray *array) {
  char *keyword_char = (char *)malloc((keyword_size + 1) * sizeof(char));

  for (size_t i = 0; i < keyword_size; ++i) {
    unsigned int text_position = position - keyword_size + i;
    keyword_char[i] = content[text_position];
  }

  keyword_char[keyword_size] = '\0';

  if (OPCODE_CMP(keyword_char)) {
    struct Token token;
    token.type = Opcode;

    array->tokens = (struct Token *)realloc(
        array->tokens, (array->size + 1) * sizeof(struct Token));
    array->tokens[array->size++] = token;
  } else if (REGISTER_CMP(keyword_char)) {
    struct Token token;
    token.type = Register;
    token.value = keyword_char;

    array->tokens = (struct Token *)realloc(
        array->tokens, (array->size + 1) * sizeof(struct Token));
    array->tokens[array->size++] = token;
  } else if (keyword_char[0] == '0' && keyword_char[1] == 'x') {
    struct Token token;
    token.type = LiteralHex;
    token.value = keyword_char;

    array->tokens = (struct Token *)realloc(
        array->tokens, (array->size + 1) * sizeof(struct Token));
    array->tokens[array->size++] = token;
  } else if (isNumber(keyword_char)) {
    struct Token token;
    token.type = LiteralNumber;
    token.value = keyword_char;

    array->tokens = (struct Token *)realloc(
        array->tokens, (array->size + 1) * sizeof(struct Token));
    array->tokens[array->size++] = token;
  }
}

bool isNumber(const char *number) {
  int len;
  float ignore;
  int ret = sscanf(number, "%f %n", &ignore, &len);

  return ret == 1 && !number[len];
}
