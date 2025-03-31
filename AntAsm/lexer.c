#include "lexer.h"
#include "throw.h"
#include "token.h"
#include <assert.h>
#include <stdlib.h>
#include <string.h>

struct TokenArray *lexer(const struct ContentInfo *content) {
  // Initialize token array
  struct TokenArray *token_array =
      (struct TokenArray *)malloc(sizeof(struct TokenArray));
  size_t keyword_size = 0;
  token_array->size = 1;
  token_array->tokens = (struct Token *)malloc(sizeof(struct Token));
  token_array->line_content = (char **)malloc(sizeof(char *));
  token_array->line_content[0] = NULL;

  size_t column = 0;
  size_t line = 1;

  // For tokenize string type
  bool is_string = false;
  char guillemet = '\0';

  bool is_comment = false;

  size_t i = 0;

  for (; i < content->content_size; ++i) {
    const char c = content->content[i];
    ++column;

    // If c reach the enline we can now set is_comment to false
    if (is_comment && c == '\n') {
      is_comment = false;
    } else if (is_comment) {
      continue;
    }

    // Tokenize string
    if (is_string && c != guillemet) {
      if (c == '\n') {
        char **temp = (char **)realloc(token_array->line_content,
                                       (line + 1) * sizeof(char *));

        assert(temp != NULL);

        token_array->line_content = temp;

        token_array->line_content[line - 1] = (char *)malloc(column--);
        token_array->line_content[line - 1][column] = '\0';
        token_array->line_content[line] = NULL;

        for (size_t j = 0; j < column; ++j) {
          size_t text_position = i - column + j;
          token_array->line_content[line - 1][j] =
              content->content[text_position];
        }

        printError(INVALID_STRING, content->filename,
                   token_array->line_content[line - 1], line, i - keyword_size,
                   column);
        freeToken(token_array);
        return NULL;
      } else {
        ++keyword_size;
        continue;
      }
    } else if (is_string && c == guillemet) {
      char *string_value = (char *)malloc(keyword_size + 1);
      string_value[keyword_size] = '\0';

      for (size_t j = 0; j < keyword_size; ++j) {
        size_t text_position = i - keyword_size + j;
        string_value[j] = content->content[text_position];
      }

      struct Token token;
      token.type = Token_LiteralString;
      token.value = string_value;
      token.line = line;
      token.start = column - keyword_size - 1;
      token.end = column;
      token.filename = content->filename;

      INSERT_TOKEN()

      is_string = false;
      guillemet = '\0';
      keyword_size = 0;

      continue;
    }

    if (c == ' ' || c == '\t') {
      CHECK_KEYWORD_SIZE()

      keyword_size = 0;
    } else if ((c >= 'A' && c <= 'Z') || (c >= 'a' && c <= 'z') ||
               (c >= '0' && c <= '9') || (c == '-' && c == '+')) {
      ++keyword_size;
    } else if (c == '\n') {
      CHECK_KEYWORD_SIZE()

      char **temp = (char **)realloc(token_array->line_content,
                                     (line + 1) * sizeof(char *));

      assert(temp != NULL);

      token_array->line_content = temp;

      token_array->line_content[line - 1] = (char *)malloc(column--);
      token_array->line_content[line - 1][column] = '\0';
      token_array->line_content[line] = NULL;

      for (size_t j = 0; j < column; ++j) {
        size_t text_position = i - column + j;
        token_array->line_content[line - 1][j] =
            content->content[text_position];
      }

      column = 0;
      ++line;
      keyword_size = 0;
    } else if (c == ',') {
      CHECK_KEYWORD_SIZE()

      struct Token token = {.type = Token_Comma,
                            .value = NULL,
                            .start = column - 1,
                            .end = column,
                            .line = line,
                            .filename = content->filename};

      INSERT_TOKEN()
      keyword_size = 0;
    } else if (c == ':') {
      CHECK_KEYWORD_SIZE()

      struct Token token = {.type = Token_SemiColon,
                            .value = NULL,
                            .start = column - 1,
                            .end = column,
                            .line = line,
                            .filename = content->filename};

      INSERT_TOKEN()
    } else if (c == ';') {
      CHECK_KEYWORD_SIZE()

      is_comment = true;
    } else if (c == '\'' || c == '"') {
      CHECK_KEYWORD_SIZE()

      is_string = true;
      guillemet = c;
      keyword_size = 0;

    } else {
      ++keyword_size;
    }
  }

  --token_array->size;
  return token_array;
}

// Tokenize part with a value
void lexePart(const size_t position, const size_t keyword_size,
              const struct ContentInfo *content, struct TokenArray *token_array,
              size_t line, size_t column) {
  // Initialize the token
  struct Token token;
  token.value = (char *)malloc((keyword_size + 1));
  token.start = column - keyword_size;
  token.end = column;
  token.line = line;
  token.filename = content->filename;

  // Get the current string value tokenized
  size_t text_position = position - keyword_size;
  for (size_t i = 0; i < keyword_size; ++i) {
    token.value[i] = content->content[text_position + i];
  }

  token.value[keyword_size] = '\0';

  if (OPCODE_CMP(token.value)) {
    token.type = Token_Opcode;
  } else if (REGISTER_CMP(token.value)) {
    token.type = Token_Register;
  } else if (token.value[0] == '0' && token.value[1] == 'x') {
    token.type = Token_LiteralHex;
  } else if (isNumber(token.value)) {
    token.type = Token_LiteralNumber;
  } else {
    token.type = Token_Identifier;
  }

  struct Token *temp = (struct Token *)realloc(
      token_array->tokens, (token_array->size + 1) * sizeof(struct Token));

  assert(temp != NULL);

  token_array->tokens = temp;
  token_array->tokens[token_array->size - 1] = token;
  token_array->tokens[token_array->size].value = NULL;
  token_array->tokens[token_array->size].line = 0;
  ++token_array->size;
}

bool isNumber(const char *number) {
  size_t i = 0;

  // Ignore the unary operator
  if (number[0] == '+' || number[0] == '-') {
    ++i;
  }

  for (; i < strlen(number); ++i) {
    if (number[i] >= '0' && number[i] <= '9') {
      continue;
    } else {
      return false;
    }
  }

  return true;
}

// Free tokens, line content and the value in TokenArray types
void freeToken(struct TokenArray *token_array) {
  size_t line_size = token_array->tokens[token_array->size - 1].line;

  for (size_t i = 0; i < token_array->size; ++i) {
    if (token_array->tokens[i].value != NULL) {
      free(token_array->tokens[i].value);
    }
  }

  for (size_t i = 0; i < line_size + 1; ++i) {
    if (token_array->line_content[i] != NULL) {
      free(token_array->line_content[i]);
    }
  }

  free(token_array->line_content);
  free(token_array->tokens);
  free(token_array);
}
