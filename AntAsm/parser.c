#include "parser.h"
#include "ast.h"
#include "token.h"
#include <stdlib.h>
#include <string.h>

enum OperationType stringToOperationType(const char *string) {
  enum OperationType operation_type;

  if (strcmp(string, "mov") == 0) {
    operation_type = Mov;
  } else if (strcmp(string, "add") == 0) {
    operation_type = Add;
  } else if (strcmp(string, "cmp") == 0) {
    operation_type = Cmp;
  } else if (strcmp(string, "or") == 0) {
    operation_type = Or;
  } else if (strcmp(string, "jmp") == 0) {
    operation_type = Jmp;
  } else if (strcmp(string, "sub") == 0) {
    operation_type = Sub;
  } else if (strcmp(string, "imul") == 0) {
    operation_type = Imul;
  } else if (strcmp(string, "push") == 0) {
    operation_type = Push;
  } else if (strcmp(string, "pop") == 0) {
    operation_type = Pop;
  } else if (strcmp(string, "inc") == 0) {
    operation_type = Inc;
  } else if (strcmp(string, "dec") == 0) {
    operation_type = Dec;
  } else if (strcmp(string, "and") == 0) {
    operation_type = And;
  } else if (strcmp(string, "xor") == 0) {
    operation_type = Xor;
  } else if (strcmp(string, "jo") == 0) {
    operation_type = Jo;
  } else if (strcmp(string, "jno") == 0) {
    operation_type = Jno;
  } else if (strcmp(string, "jb") == 0) {
    operation_type = Jb;
  } else if (strcmp(string, "jnb") == 0) {
    operation_type = Jnb;
  } else if (strcmp(string, "jz") == 0) {
    operation_type = Jz;
  } else if (strcmp(string, "jnz") == 0) {
    operation_type = Jnz;
  } else if (strcmp(string, "jbe") == 0) {
    operation_type = Jbe;
  } else if (strcmp(string, "ja") == 0) {
    operation_type = Ja;
  } else if (strcmp(string, "js") == 0) {
    operation_type = Js;
  } else if (strcmp(string, "jns") == 0) {
    operation_type = Jns;
  } else if (strcmp(string, "jp") == 0) {
    operation_type = Jp;
  } else if (strcmp(string, "jnp") == 0) {
    operation_type = Jnp;
  } else if (strcmp(string, "jl") == 0) {
    operation_type = Jl;
  } else if (strcmp(string, "jnl") == 0) {
    operation_type = Jnl;
  } else if (strcmp(string, "jle") == 0) {
    operation_type = Jle;
  } else if (strcmp(string, "jnle") == 0) {
    operation_type = Jnle;
  } else if (strcmp(string, "equ") == 0) {
    operation_type = Equ;
  }

  return operation_type;
}

enum ValueType literalToValue(enum TokenType token_type) {
  enum ValueType value_type;

  if (token_type == LiteralString) {
    value_type = StringType;
  } else if (token_type == LiteralNumber) {
    value_type = NumberType;
  } else if (token_type == LiteralHex) {
    value_type = HexType;
  } else if (token_type == Register) {
    value_type = RegisterType;
  }

  return value_type;
}

struct Program parse(const struct TokenArray *token_array) {
  struct Program ast;
  ast.member_list =
      (struct OperationMember *)malloc(sizeof(struct OperationMember));

  for (size_t i = 0; i < token_array->size; ++i) {
    struct OperationMember operation_member;
    struct Token token = token_array->tokens[i];

    if (token.type != Opcode) {
      // TODO: HANDLE ERROR
    }

    ast.member_list = realloc(ast.member_list,
                              (ast.size + 1) * sizeof(struct OperationMember));

    operation_member.operation_type = stringToOperationType(token.value);

    ++i;

    if (operation_member.operation_type == Jmp ||
        operation_member.operation_type == Push ||
        operation_member.operation_type == Pop ||
        operation_member.operation_type == Inc ||
        operation_member.operation_type == Dec ||
        operation_member.operation_type == Jo ||
        operation_member.operation_type == Jno ||
        operation_member.operation_type == Jb ||
        operation_member.operation_type == Jnb ||
        operation_member.operation_type == Jz ||
        operation_member.operation_type == Jnz ||
        operation_member.operation_type == Jbe ||
        operation_member.operation_type == Ja ||
        operation_member.operation_type == Js ||
        operation_member.operation_type == Jns ||
        operation_member.operation_type == Jp ||
        operation_member.operation_type == Jnp ||
        operation_member.operation_type == Jl ||
        operation_member.operation_type == Jnl ||
        operation_member.operation_type == Jle ||
        operation_member.operation_type == Jnle) {
      parseOnlyDestOperation(token_array, i, &operation_member);
    } else {
      parseSrcDestOperation(token_array, &i, &operation_member);
    }

    ast.member_list[ast.size] = operation_member;
    ++ast.size;
  }

  return ast;
}

void parseOnlyDestOperation(const struct TokenArray *token_array, size_t i,
                            struct OperationMember *operation_member) {
  struct Token token = token_array->tokens[i];

  if (token.type != Register) {
    // TODO: HANDLE ERROR
  }

  operation_member->register_dest = token.value;
}

void parseSrcDestOperation(const struct TokenArray *token_array, size_t *i,
                           struct OperationMember *operation_member) {
  if (token_array->tokens[*i].type != Register) {
    // TODO: HANDLE ERROR
  }

  operation_member->register_dest = token_array->tokens[*i++].value;

  if (token_array->tokens[*i++].type != Comma) {
    // TODO: HANDLE ERROR
  }

  enum TokenType src_type = token_array->tokens[*i].type;

  if (src_type != LiteralString || src_type != LiteralHex ||
      src_type != LiteralNumber || src_type != Register) {
    // TODO: HANDLE ERROR
  }

  operation_member->src_type = literalToValue(src_type);
  operation_member->src_value = token_array->tokens[*i].value;
}
