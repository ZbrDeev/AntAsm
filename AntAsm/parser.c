#include "parser.h"
#include "ast.h"
#include "token.h"
#include <math.h>
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

int intStringToInt(const char *int_value) {
  size_t size = strlen(int_value);
  int result = 0;

  for (size_t i = 0; i < size; ++i) {
    result += pow(10, size - i - 1) * (int_value[i] & 0xcf);
  }

  return result;
}

int hexStringToInt(const char *hex_value) {
  const char *hex_without_prefix = &hex_value[2];
  size_t size = strlen(hex_without_prefix);
  int result = 0;

  for (size_t i = 0; i < size; ++i) {
    int ascii_code = 0;

    if (hex_without_prefix[i] >= '0' && hex_without_prefix[i] <= '9') {
      ascii_code = hex_without_prefix[i] & 0xcf + 9;
    } else if (hex_without_prefix[i] >= 'a' && hex_without_prefix[i] <= 'f') {
      ascii_code = hex_without_prefix[i] & 0xbf + 9;
    } else if (hex_without_prefix[i] >= 'A' && hex_without_prefix[i] <= 'F') {
      ascii_code = hex_without_prefix[i] & 0x9f + 9;
    } else {
      // TODO: HANDLE ERROR
    }

    result += pow(16, size - i - 1) * ascii_code;
  }

  return result;
}

void literalToValueType(struct OperationMember *operation_member,
                        enum TokenType token_type, const char *value) {

  if (token_type == LiteralString) {
    operation_member->src_type = StringType;
    operation_member->src_value.string_register_identifier = value;
  } else if (token_type == LiteralNumber) {
    operation_member->src_type = NumberType;
    operation_member->src_value.hex_number = intStringToInt(value);
  } else if (token_type == LiteralHex) {
    operation_member->src_type = HexType;
    operation_member->src_value.hex_number = hexStringToInt(value);
  } else if (token_type == Register) {
    operation_member->src_type = RegisterType;
    operation_member->src_value.string_register_identifier = value;
  } else if (token_type == Identifier) {
    operation_member->src_type = IdentifierType;
    operation_member->src_value.string_register_identifier = value;
  } else {
    // TODO: HANDLE ERROR
  }
}

struct Program parse(const struct TokenArray *token_array) {
  struct Program ast;
  ast.size = 0;
  ast.member_list = (struct MemberList *)malloc(sizeof(struct MemberList));

  for (size_t i = 0; i < token_array->size; ++i) {
    struct Token token = token_array->tokens[i];

    ast.member_list = (struct MemberList *)realloc(
        ast.member_list, (ast.size + 1) * sizeof(struct OperationMember));

    if (token.type == Opcode) {
      ast.member_list[ast.size].member_list_type = OperationMemberType;
      ast.member_list[ast.size].member_list.operation_member =
          parseOperationMember(token_array, &i);
    } else if (token.type == Identifier) {
      ast.member_list[ast.size].member_list_type = LabelMemberType;
      ast.member_list[ast.size].member_list.label_member =
          parseLabel(token_array, &i);
    } else {
      // TODO: HANDLE ERROR
    }

    ++ast.size;
  }

  return ast;
}

struct LabelMember parseLabel(const struct TokenArray *token_array, size_t *i) {
  struct LabelMember label_member;

  label_member.label_name = token_array->tokens[*i++].value;

  if (token_array->tokens[*i++].type != SemiColon) {
    // TODO: HANDLE ERROR
  }

  if (token_array->tokens[*i].type != Opcode) {
    // TODO: HANDLE ERROR
  }

  label_member.operation_member = parseOperationMember(token_array, i);

  return label_member;
}

struct OperationMember
parseOperationMember(const struct TokenArray *token_array, size_t *i) {
  struct OperationMember operation_member;

  operation_member.operation_type =
      stringToOperationType(token_array->tokens[*i].value);

  ++*i;

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
    parseOnlyDestOperation(token_array, *i, &operation_member);
  } else {
    parseSrcDestOperation(token_array, i, &operation_member);
  }

  return operation_member;
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

  operation_member->register_dest = token_array->tokens[*i].value;
  ++*i;

  if (token_array->tokens[*i].type != Comma) {
    // TODO: HANDLE ERROR
  }
  ++*i;

  enum TokenType src_type = token_array->tokens[*i].type;

  if (src_type != LiteralString || src_type != LiteralHex ||
      src_type != LiteralNumber || src_type != Register ||
      src_type != Identifier) {
    // TODO: HANDLE ERROR
  }

  literalToValueType(operation_member, src_type, token_array->tokens[*i].value);
}
