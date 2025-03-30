#include "parser.h"
#include "ast.h"
#include "bst.h"
#include "throw.h"
#include "token.h"
#include <assert.h>
#include <math.h>
#include <stdlib.h>
#include <string.h>

enum OperationType stringToOperationType(const char *string) {
  enum OperationType operation_type;

  if (strcmp(string, "mov") == 0) {
    operation_type = Operation_Mov;
  } else if (strcmp(string, "add") == 0) {
    operation_type = Operation_Add;
  } else if (strcmp(string, "cmp") == 0) {
    operation_type = Operation_Cmp;
  } else if (strcmp(string, "or") == 0) {
    operation_type = Operation_Or;
  } else if (strcmp(string, "jmp") == 0) {
    operation_type = Operation_Jmp;
  } else if (strcmp(string, "sub") == 0) {
    operation_type = Operation_Sub;
  } else if (strcmp(string, "imul") == 0) {
    operation_type = Operation_Imul;
  } else if (strcmp(string, "push") == 0) {
    operation_type = Operation_Push;
  } else if (strcmp(string, "pop") == 0) {
    operation_type = Operation_Pop;
  } else if (strcmp(string, "inc") == 0) {
    operation_type = Operation_Inc;
  } else if (strcmp(string, "dec") == 0) {
    operation_type = Operation_Dec;
  } else if (strcmp(string, "and") == 0) {
    operation_type = Operation_And;
  } else if (strcmp(string, "xor") == 0) {
    operation_type = Operation_Xor;
  } else if (strcmp(string, "jo") == 0) {
    operation_type = Operation_Jo;
  } else if (strcmp(string, "jno") == 0) {
    operation_type = Operation_Jno;
  } else if (strcmp(string, "jb") == 0) {
    operation_type = Operation_Jb;
  } else if (strcmp(string, "jnb") == 0) {
    operation_type = Operation_Jnb;
  } else if (strcmp(string, "jz") == 0) {
    operation_type = Operation_Jz;
  } else if (strcmp(string, "jnz") == 0) {
    operation_type = Operation_Jnz;
  } else if (strcmp(string, "jbe") == 0) {
    operation_type = Operation_Jbe;
  } else if (strcmp(string, "ja") == 0) {
    operation_type = Operation_Ja;
  } else if (strcmp(string, "js") == 0) {
    operation_type = Operation_Js;
  } else if (strcmp(string, "jns") == 0) {
    operation_type = Operation_Jns;
  } else if (strcmp(string, "jp") == 0) {
    operation_type = Operation_Jp;
  } else if (strcmp(string, "jnp") == 0) {
    operation_type = Operation_Jnp;
  } else if (strcmp(string, "jl") == 0) {
    operation_type = Operation_Jl;
  } else if (strcmp(string, "jnl") == 0) {
    operation_type = Operation_Jnl;
  } else if (strcmp(string, "jle") == 0) {
    operation_type = Operation_Jle;
  } else if (strcmp(string, "jnle") == 0) {
    operation_type = Operation_Jnle;
  } else if (strcmp(string, "equ") == 0) {
    operation_type = Operation_Equ;
  } else if (strcmp(string, "syscall") == 0) {
    operation_type = Operation_Syscall;
  } else {
    assert(false);
  }

  return operation_type;
}

int intStringToInt(struct TokenArray *token_array, size_t index) {
  struct Token token = token_array->tokens[index];

  size_t size = strlen(token.value);
  int result = 0;

  size_t i = 0;
  bool is_positive = true;

  // Check if the number has unary operator
  if (token.value[0] == '+') {
    ++i;
  } else if (token.value[0] == '-') {
    is_positive = false;
    ++i;
  }

  for (; i < size; ++i) {
    result += pow(10, size - i - 1) * (token.value[i] & 0xf);
  }

  if (!is_positive && !result) {
    result = ~result + 1;
  }

  return result;
}

int hexStringToInt(struct TokenArray *token_array, size_t index) {
  struct Token token = token_array->tokens[index];

  // Ignore the 0x at the begining
  const char *hex_without_prefix = &token.value[2];
  size_t size = strlen(hex_without_prefix);
  int result = 0;

  for (size_t i = 0; i < size; ++i) {
    int utf8_code = 0;

    // For numerical value take the last 4 bits and for the alphabetical value A
    // to F
    if (hex_without_prefix[i] >= '0' && hex_without_prefix[i] <= '9') {
      utf8_code = hex_without_prefix[i] & 0xf;
    } else if (hex_without_prefix[i] >= 'a' && hex_without_prefix[i] <= 'f') {
      utf8_code = (hex_without_prefix[i] & 0x7) + 9;
    } else if (hex_without_prefix[i] >= 'A' && hex_without_prefix[i] <= 'F') {
      utf8_code = (hex_without_prefix[i] & 0x7) + 9;
    } else {
      return -1;
    }

    result += pow(16, size - i - 1) * utf8_code;
  }

  return result;
}

int literalToValueType(struct OperationMember *operation_member,
                       struct TokenArray *token_array, size_t i) {
  struct Token token = token_array->tokens[i];
  enum TokenType token_type = token.type;

  if (token_type == Token_LiteralString) {
    operation_member->src_type = Value_String;
    operation_member->src_value.string_register_identifier = token.value;
  } else if (token_type == Token_LiteralNumber) {
    operation_member->src_type = Value_Number;
    operation_member->src_value.hex_number = intStringToInt(token_array, i);
  } else if (token_type == Token_LiteralHex) {
    operation_member->src_type = Value_Hex;
    if ((operation_member->src_value.hex_number =
             hexStringToInt(token_array, i)) == -1) {
      return -1;
    }
  } else if (token_type == Token_Register) {
    operation_member->src_type = Value_Register;
    operation_member->src_value.string_register_identifier = token.value;
  } else if (token_type == Token_Identifier) {
    operation_member->src_type = Value_Identifier;
    operation_member->src_value.string_register_identifier = token.value;
  } else {
    return -1;
  }

  return 0;
}

struct Program *parse(struct TokenArray *token_array,
                      struct Bst *symb_repl_bst) {
  struct Program *ast = (struct Program *)malloc(sizeof(struct Program));
  ast->size = 1;
  ast->member_list = (struct MemberList *)malloc(sizeof(struct MemberList));

  if (symb_repl_bst != NULL) {
    ast->member_list->symbol = symb_repl_bst;
  } else {
    ast->member_list->symbol = NULL;
  }

  for (size_t i = 0; i < token_array->size; ++i) {
    struct Token token = token_array->tokens[i];

    if (token.type == Token_Opcode) {
      // If opcode is equal syscall do not check the 3 last token
      if (strcmp(token.value, "syscall") != 0) {
        for (size_t j = 1; j <= 3; ++j) {
          if (token_array->tokens[i + j].line <= 0) {
            printError(EXPECT_OPERAND, token.filename,
                       token_array->line_content[token.line - 1], token.line,
                       token.start, token.end);
            freeProgram(ast);
            return NULL;
          }
        }
      }

      struct MemberList *temp = (struct MemberList *)realloc(
          ast->member_list, (ast->size + 1) * sizeof(struct MemberList));

      assert(temp != NULL);

      ast->member_list = temp;

      ast->member_list[ast->size - 1].member_list_type =
          MemberList_OperationMember;

      struct OperationMember *om_temp;

      if ((om_temp = parseOperationMember(token_array, &i)) == NULL) {
        freeProgram(ast);
        return NULL;
      }

      ast->member_list[ast->size - 1].member_list.operation_member = *om_temp;
      free(om_temp);

    } else if (token.type == Token_Identifier) {
      for (size_t j = 1; j <= 4; ++j) {
        if (token_array->tokens[i + j].line <= 0) {
          printError(EXPECT_OPERAND, token.filename,
                     token_array->line_content[token.line - 1], token.line,
                     token.start, token.end);
          freeProgram(ast);
          return NULL;
        }
      }
      ast->member_list[ast->size - 1].member_list_type = MemberList_LabelMember;

      struct LabelMember *temp;

      if ((temp = parseLabel(token_array, &i, ast->member_list)) == NULL) {
        freeProgram(ast);
        return NULL;
      }

      ast->member_list[ast->size - 1].member_list.label_member = *temp;
      free(temp);

    } else {
      printError(EXPECT_KEYWORD, token.filename,
                 token_array->line_content[token.line - 1], token.line,
                 token.start, token.end);
      freeProgram(ast);
      return NULL;
    }

    ++ast->size;
  }

  return ast;
}

struct LabelMember *parseLabel(struct TokenArray *token_array, size_t *i,
                               struct MemberList *member_list) {
  struct Token token = token_array->tokens[*i];

  struct LabelMember *label_member =
      (struct LabelMember *)malloc(sizeof(struct LabelMember));
  label_member->location.start.column = token.start;
  label_member->location.start.line = token.line;
  label_member->location.filename = token.filename;

  label_member->label_name = token.value;

  token = token_array->tokens[++(*i)];

  if (token.type != Token_SemiColon) {
    printError(EXPECT_SEMICOLON, token.filename,
               token_array->line_content[token.line - 1], token.line,
               token.start, token.end);
    return NULL;
  }

  token = token_array->tokens[++(*i)];

  if (token.type != Token_Opcode) {
    printError(EXPECT_KEYWORD, token.filename,
               token_array->line_content[token.line - 1], token.line,
               token.start, token.end);
    return NULL;
  }

  struct OperationMember *temp;
  if ((temp = parseOperationMember(token_array, i)) == NULL) {
    printError(EXPECT_KEYWORD, token.filename,
               token_array->line_content[token.line - 1], token.line,
               token.start, token.end);
    return NULL;
  }

  label_member->operation_member = *temp;

  label_member->location.end = label_member->operation_member.location.end;

  addKeyValueBst(&member_list->symbol, calcStringUtf(label_member->label_name),
                 label_member->location.start.line - 1, NULL);

  return label_member;
}

struct OperationMember *parseOperationMember(struct TokenArray *token_array,
                                             size_t *i) {
  struct Token token = token_array->tokens[*i];

  struct OperationMember *operation_member =
      (struct OperationMember *)malloc(sizeof(struct OperationMember));
  operation_member->location.filename = token.filename;
  operation_member->location.start.line = token.line;
  operation_member->location.start.column = token.start;
  operation_member->location.line_content =
      token_array->line_content[token.line - 1];

  operation_member->operation_type = stringToOperationType(token.value);

  if (operation_member->operation_type == Operation_Syscall) {
    return operation_member;
  }

  token = token_array->tokens[++(*i)];

  if (operation_member->operation_type == Operation_Jmp ||
      operation_member->operation_type == Operation_Push ||
      operation_member->operation_type == Operation_Pop ||
      operation_member->operation_type == Operation_Inc ||
      operation_member->operation_type == Operation_Dec ||
      operation_member->operation_type == Operation_Jo ||
      operation_member->operation_type == Operation_Jno ||
      operation_member->operation_type == Operation_Jb ||
      operation_member->operation_type == Operation_Jnb ||
      operation_member->operation_type == Operation_Jz ||
      operation_member->operation_type == Operation_Jnz ||
      operation_member->operation_type == Operation_Jbe ||
      operation_member->operation_type == Operation_Ja ||
      operation_member->operation_type == Operation_Js ||
      operation_member->operation_type == Operation_Jns ||
      operation_member->operation_type == Operation_Jp ||
      operation_member->operation_type == Operation_Jnp ||
      operation_member->operation_type == Operation_Jl ||
      operation_member->operation_type == Operation_Jnl ||
      operation_member->operation_type == Operation_Jle ||
      operation_member->operation_type == Operation_Jnle) {
    parseOnlyDestOperation(token_array, *i, operation_member);
  } else {
    if (parseSrcDestOperation(token_array, i, operation_member) == -1) {
      return NULL;
    }
  }

  return operation_member;
}

// For operation like "jmp test"
void parseOnlyDestOperation(struct TokenArray *token_array, size_t i,
                            struct OperationMember *operation_member) {
  struct Token token = token_array->tokens[i];

  operation_member->register_dest = token.value;
  operation_member->src_type = Value_Null;

  operation_member->location.end.line = token.line;
  operation_member->location.end.column = token.end;
}

// For operation like "mov eax, 4"
int parseSrcDestOperation(struct TokenArray *token_array, size_t *i,
                          struct OperationMember *operation_member) {
  struct Token token = token_array->tokens[*i];

  // Check if the operation is "equ" for operation with an Identifier as a
  // destination
  if (token.type != Token_Register &&
      !(strcmp(token_array->tokens[(*i) - 1].value, "equ") == 0 &&
        token.type == Token_Identifier)) {
    printError(EXPECT_REGISTER, token.filename,
               token_array->line_content[token.line - 1], token.line,
               token.start, token.end);
    return -1;
  }

  operation_member->register_dest = token.value;
  token = token_array->tokens[++(*i)];

  if (token.type != Token_Comma) {
    printError(EXPECT_COMMA, token.filename,
               token_array->line_content[token.line - 1], token.line,
               token.start, token.end);
    return -1;
  }

  token = token_array->tokens[++(*i)];

  enum TokenType src_type = token.type;

  if (src_type != Token_LiteralString && src_type != Token_LiteralHex &&
      src_type != Token_LiteralNumber && src_type != Token_Register &&
      src_type != Token_Identifier) {
    printError(EXPECT_VALUE, token.filename,
               token_array->line_content[token.line - 1], token.line,
               token.start, token.end);
    return -1;
  }

  if (literalToValueType(operation_member, token_array, *i) == -1) {
    printError(EXPECT_VALUE, token.filename,
               token_array->line_content[token.line - 1], token.line,
               token.start, token.end);
    return -1;
  }

  operation_member->location.end.line = token.line;
  operation_member->location.end.line = token.end;

  return 0;
}

void freeProgram(struct Program *program) {
  free(program->member_list);
  free(program);
}