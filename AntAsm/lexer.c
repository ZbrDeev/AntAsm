#include "lexer.h"
#include "token.h"
#include <stdlib.h>
#include <string.h>

struct TokenArray lexer(const struct ContentInfo *content) {
  struct TokenArray token_array;
  unsigned int keyword_size = 0;
  token_array.size = 1;
  token_array.tokens = (struct Token *)malloc(sizeof(struct Token));

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

      struct Token *temp = (struct Token *)realloc(
          token_array.tokens, (token_array.size + 1) * sizeof(struct Token));

      if (temp == NULL) {
        // TODO: HANDLE ERROR
      }
      token_array.tokens = temp;
      token_array.tokens[token_array.size - 1] = token;
      ++token_array.size;

      is_string = false;
      guillemet = '\0';

      continue;
    }

    if (c == ' ' || c == '\t') {
      if (keyword_size > 0) {
        lexePart(i, keyword_size, content->content, &token_array);
      }
      keyword_size = 0;
      continue;
    } else if ((c >= 'A' && c <= 'Z') || (c >= 'a' && c <= 'z') ||
               (c >= '0' && c <= '9')) {
      ++keyword_size;
    } else if (c == '\n') {
      if (keyword_size > 0) {
        lexePart(i, keyword_size, content->content, &token_array);
      }

      column = 0;
      ++line;
      keyword_size = 0;
    } else if (c == ',') {
      if (keyword_size > 0) {
        lexePart(i, keyword_size, content->content, &token_array);
      }

      struct Token token;
      token.type = Comma;
      token.value = NULL;

      struct Token *temp = (struct Token *)realloc(
          token_array.tokens, (token_array.size + 1) * sizeof(struct Token));

      if (temp == NULL) {
        // TODO: HANDLE ERROR
      }
      token_array.tokens = temp;
      token_array.tokens[token_array.size - 1] = token;
      ++token_array.size;
      keyword_size = 0;
    } else if (c == ':') {
      if (keyword_size > 0) {
        lexePart(i, keyword_size, content->content, &token_array);
      }

      struct Token token;
      token.type = SemiColon;
      token.value = NULL;

      struct Token *temp = (struct Token *)realloc(
          token_array.tokens, (token_array.size + 1) * sizeof(struct Token));

      if (temp == NULL) {
        // TODO: HANDLE ERROR
      }
      token_array.tokens = temp;
      token_array.tokens[token_array.size - 1] = token;
      ++token_array.size;
      keyword_size = 0;
    } else if (c == '\'' || c == '"') {
      is_string = true;
      guillemet = c;
    } else {
      ++keyword_size;
    }
  }

  --token_array.size;
  return token_array;
}

void lexePart(const unsigned int position, const unsigned int keyword_size,
              const char *content, struct TokenArray *array) {
  struct Token token;
  token.value = (char *)malloc((keyword_size + 1) * sizeof(char));

  for (size_t i = 0; i < keyword_size; ++i) {
    unsigned int text_position = position - keyword_size + i;
    token.value[i] = content[text_position];
  }

  token.value[keyword_size] = '\0';

  if (OPCODE_CMP(token.value)) {
    token.type = Opcode;
  } else if (REGISTER_CMP(token.value)) {
    token.type = Register;
  } else if (token.value[0] == '0' && token.value[1] == 'x') {
    token.type = LiteralHex;
  } else if (isNumber(token.value)) {
    token.type = LiteralNumber;
  } else {
    token.type = Identifier;
  }

  struct Token *temp = (struct Token *)realloc(
      array->tokens, (array->size + 1) * sizeof(struct Token));

  if (temp == NULL) {
    // TODO: HANDLE ERROR
  }
  array->tokens = temp;
  array->tokens[array->size - 1] = token;
  ++array->size;
}

bool isNumber(const char *number) {
  size_t size = strlen(number);

  for (size_t i = 0; i < size; ++i) {
    if (number[i] >= '0' && number[i] <= '9') {
      continue;
    } else {
      return false;
    }
  }

  return true;
}
