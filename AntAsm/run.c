#include "run.h"
#include "ast.h"
#include "file_utils.h"
#include "hashmap.h"
#include "lexer.h"
#include "parser.h"
#include "token.h"
#include <stddef.h>
#include <stdint.h>
#include <stdlib.h>
#include <string.h>

void runScript(struct Program *program) {
  struct RegisterEmu register_emu;
  INIT_REGISTER_EMU(register_emu)
  INIT_HASHMAP_REGISTER_EMU(register_emu)

  for (size_t i = 0; i < program->size - 1; ++i) {
    struct MemberList member_list = program->member_list[i];

    if (member_list.member_list_type == OperationMemberType) {
      manageOperationType(member_list.member_list.operation_member,
                          &register_emu);
    }
  }

  free(register_emu.hashmap.nodeList);
}

void manageOperationType(struct OperationMember operation_member,
                         struct RegisterEmu *register_emu) {
  if (operation_member.src_type == StringType) {
    // TODO: HANDLE ERROR
  }

  int64_t value;

  if (operation_member.operation_type == Push ||
      operation_member.operation_type == Pop ||
      operation_member.operation_type == Inc ||
      operation_member.operation_type == Dec) {
    manageOnlyRegisterDest(operation_member, register_emu);
  } else if (operation_member.operation_type == Jmp ||
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
    manageIdentifierAsADest(operation_member, register_emu);
  } else if (operation_member.operation_type == Mov ||
             operation_member.operation_type == Add ||
             operation_member.operation_type == Cmp ||
             operation_member.operation_type == Or ||
             operation_member.operation_type == Sub ||
             operation_member.operation_type == Imul ||
             operation_member.operation_type == And ||
             operation_member.operation_type == Xor ||
             operation_member.operation_type == Equ) {
    manageDestSrc(operation_member, register_emu);
  } else {
    // TODO: HANDLE ERROR
  }
}

void manageOnlyRegisterDest(struct OperationMember operation_member,
                            struct RegisterEmu *register_emu) {
  struct NodeValue value =
      getValue(&register_emu->hashmap, operation_member.register_dest);

  int64_t register_value;

  if (value.node_value_type == Int64) {
    register_value = *(int64_t *)value.node_value;
  } else if (value.node_value_type == Int32) {
    register_value = *(int32_t *)value.node_value;
  } else if (value.node_value_type == Int16) {
    register_value = *(int16_t *)value.node_value;
  } else if (value.node_value_type == Int8) {
    register_value = *(int8_t *)value.node_value;
  } else {
    // TODO: HANDLE ERROR
  }

  if (operation_member.operation_type == Inc) {
    ++register_value;
  } else if (operation_member.operation_type == Dec) {
    --register_value;
  } else {
    // TODO: HANDLE ERROR
  }

  if (value.node_value_type == Int64) {
    *(int64_t *)value.node_value = register_value;
  } else if (value.node_value_type == Int32) {
    *(int32_t *)value.node_value = register_value;
  } else if (value.node_value_type == Int16) {
    *(int16_t *)value.node_value = register_value;
  } else if (value.node_value_type == Int8) {
    *(int8_t *)value.node_value = register_value;
  }
}

void manageIdentifierAsADest(struct OperationMember operation_member,
                             struct RegisterEmu *register_emu) {
  // TODO: SOON WITH THE LINKED LIST
}

void manageDestSrc(struct OperationMember operation_member,
                   struct RegisterEmu *register_emu) {
  struct NodeValue value =
      getValue(&register_emu->hashmap, operation_member.register_dest);

  int64_t register_value = 0;
  int64_t src_value = 0;

  if (value.node_value_type == Int64) {
    register_value = *(int64_t *)value.node_value;
  } else if (value.node_value_type == Int32) {
    register_value = *(int32_t *)value.node_value;
  } else if (value.node_value_type == Int16) {
    register_value = *(int16_t *)value.node_value;
  } else if (value.node_value_type == Int8) {
    register_value = *(int8_t *)value.node_value;
  } else {
    // TODO: HANDLE ERROR
  }

  if (operation_member.src_type == HexType ||
      operation_member.src_type == NumberType) {
    src_value = operation_member.src_value.hex_number;
  } else {
    // TODO: HANDLE ERROR (NOT YET IMPLEMENTED)
  }

  if (operation_member.operation_type == Mov) {
    register_value = src_value;
  } else if (operation_member.operation_type == Add) {
    register_value += src_value;
  } else if (operation_member.operation_type == Cmp) {
    // SOON
  } else if (operation_member.operation_type == Or) {
    register_value |= src_value;
  } else if (operation_member.operation_type == Sub) {
    register_value -= src_value;
  } else if (operation_member.operation_type == Imul) {
    register_value *= src_value;
  } else if (operation_member.operation_type == And) {
    register_value &= src_value;
  } else if (operation_member.operation_type == Xor) {
    register_value ^= src_value;
  } else if (operation_member.operation_type == Equ) {
    // SOON
  }

  if (value.node_value_type == Int64) {
    *(int64_t *)value.node_value = register_value;
  } else if (value.node_value_type == Int32) {
    *(int32_t *)value.node_value = register_value;
  } else if (value.node_value_type == Int16) {
    *(int16_t *)value.node_value = register_value;
  } else if (value.node_value_type == Int8) {
    *(int8_t *)value.node_value = register_value;
  }
}

void doAllProcess(const char *file) {
  char *file_content = readFile(file);
  size_t file_size = strlen(file_content);

  const struct ContentInfo content_info = {
      .content = file_content, .filename = file, .content_size = file_size};

  struct TokenArray token = lexer(&content_info);

  struct Program program = parse(&token);

  runScript(&program);

  free(program.member_list);

  for (size_t i = 0; i < token.size; ++i) {
    if (token.tokens[i].value != NULL) {
      free(token.tokens[i].value);
    }
  }

  free(token.tokens);

  free(file_content);
}
