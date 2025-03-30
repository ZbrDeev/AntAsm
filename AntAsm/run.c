#include "run.h"
#include "ast.h"
#include "bst.h"
#include "file_utils.h"
#include "hashmap.h"
#include "lexer.h"
#include "parser.h"
#include "syscall.h"
#include "throw.h"
#include "token.h"
#include <assert.h>
#include <stddef.h>
#include <stdint.h>
#include <stdlib.h>
#include <string.h>

int runScript(struct Program *program) {
  // Initialize the register emulator
  struct RegisterEmu register_emu;

  INIT_REGISTER_EMU(register_emu)
  register_emu.symbol = program->member_list->symbol;

  INIT_HASHMAP_REGISTER_EMU(register_emu)
  SET_ZERO_FLAGS(register_emu.flags)

  int status_code = 0;

  for (size_t i = 0; i < program->size - 1; ++i) {
    struct MemberList member_list = program->member_list[i];

    if (member_list.member_list_type == MemberList_OperationMember) {
      if (manageOperationType(member_list.member_list.operation_member,
                              &register_emu, &i) == -1) {
        return -1;
      }
    } else if (member_list.member_list_type == MemberList_LabelMember) {
      if (manageOperationType(
              member_list.member_list.label_member.operation_member,
              &register_emu, &i) == -1) {
        return -1;
      }
    }

    if (register_emu.exit) {
      status_code = register_emu.reg_DI;
    }
  }

  freeRegister(&register_emu);

  return status_code;
}

int manageOperationType(struct OperationMember operation_member,
                        struct RegisterEmu *register_emu, size_t *i) {
  if (operation_member.operation_type == Operation_Syscall) {
    int error_code = 0;

#if defined(__linux__)
    error_code = callSyscallLinux(register_emu);
#endif

    if (error_code != 0) {
      printError(error_code, "Syscall error",
                 operation_member.location.filename,
                 operation_member.location.line_content,
                 operation_member.location.start.line,
                 operation_member.location.start.column,
                 operation_member.location.end.column);
      return -1;
    }

    if (register_emu->exit) {
      return 0;
    }
  } else if (operation_member.operation_type == Operation_Push ||
             operation_member.operation_type == Operation_Pop ||
             operation_member.operation_type == Operation_Inc ||
             operation_member.operation_type == Operation_Dec) {
    if (manageOnlyRegisterDest(operation_member, register_emu) == -1) {
      return -1;
    }
  } else if (operation_member.operation_type == Operation_Jmp ||
             operation_member.operation_type == Operation_Jo ||
             operation_member.operation_type == Operation_Jno ||
             operation_member.operation_type == Operation_Jb ||
             operation_member.operation_type == Operation_Jnb ||
             operation_member.operation_type == Operation_Jz ||
             operation_member.operation_type == Operation_Jnz ||
             operation_member.operation_type == Operation_Jbe ||
             operation_member.operation_type == Operation_Ja ||
             operation_member.operation_type == Operation_Js ||
             operation_member.operation_type == Operation_Jns ||
             operation_member.operation_type == Operation_Jp ||
             operation_member.operation_type == Operation_Jnp ||
             operation_member.operation_type == Operation_Jl ||
             operation_member.operation_type == Operation_Jnl ||
             operation_member.operation_type == Operation_Jle ||
             operation_member.operation_type == Operation_Jnle) {
    if (manageIdentifierAsADest(operation_member, register_emu, i) == -1) {
      return -1;
    }
  } else if (operation_member.operation_type == Operation_Mov ||
             operation_member.operation_type == Operation_Add ||
             operation_member.operation_type == Operation_Cmp ||
             operation_member.operation_type == Operation_Or ||
             operation_member.operation_type == Operation_Sub ||
             operation_member.operation_type == Operation_Imul ||
             operation_member.operation_type == Operation_And ||
             operation_member.operation_type == Operation_Xor ||
             operation_member.operation_type == Operation_Equ) {
    if (manageDestSrc(operation_member, register_emu) == -1) {
      return -1;
    }
  } else {
    assert(false);
  }

  return 0;
}

int manageOnlyRegisterDest(struct OperationMember operation_member,
                           struct RegisterEmu *register_emu) {
  struct NodeValue value =
      getValue(&register_emu->hashmap, operation_member.register_dest);

  int64_t register_value;

  // Convert the register value to int64 for better manipulation
  if (value.node_value_type == NodeValue_Int64) {
    register_value = *(int64_t *)value.node_value;
  } else if (value.node_value_type == NodeValue_Int32) {
    register_value = *(int32_t *)value.node_value;
  } else if (value.node_value_type == NodeValue_Int16) {
    register_value = *(int16_t *)value.node_value;
  } else if (value.node_value_type == NodeValue_Int8 ||
             value.node_value_type == NodeValue_Int8_Low) {
    register_value = *(int8_t *)value.node_value;
  } else if (value.node_value_type == NodeValue_Int8_High) {
    register_value = ((int8_t *)value.node_value)[1];
  } else {
    printError(INCORRECT_VALUE, operation_member.location.filename,
               operation_member.location.line_content,
               operation_member.location.start.line,
               operation_member.location.start.column,
               operation_member.location.end.column);
    return -1;
  }

  if (operation_member.operation_type == Operation_Inc) {
    ++register_value;
  } else if (operation_member.operation_type == Operation_Dec) {
    --register_value;
  } else if (operation_member.operation_type == Operation_Push) {
    pushStack(register_emu, register_value);
  } else if (operation_member.operation_type == Operation_Pop) {
    popStack(register_emu, &register_value);
  } else {
    printError(7, "Other operation are not implemented yet",
               operation_member.location.filename,
               operation_member.location.line_content,
               operation_member.location.start.line,
               operation_member.location.start.column,
               operation_member.location.end.column);
    return -1;
  }

  // Convert register_value to his respective register value with sized int
  if (value.node_value_type == NodeValue_Int64) {
    *(int64_t *)value.node_value = register_value;
  } else if (value.node_value_type == NodeValue_Int32) {
    *(int32_t *)value.node_value = register_value;
  } else if (value.node_value_type == NodeValue_Int16) {
    *(int16_t *)value.node_value = register_value;
  } else if (value.node_value_type == NodeValue_Int8 ||
             value.node_value_type == NodeValue_Int8_Low) {
    *(int8_t *)value.node_value = register_value;
  } else if (value.node_value_type == NodeValue_Int8_High) {
    ((int8_t *)value.node_value)[1] = (int8_t)register_value;
  }

  return 0;
}

// Only for jump condition
int manageIdentifierAsADest(struct OperationMember operation_member,
                            struct RegisterEmu *register_emu, size_t *i) {
  struct Bst *line =
      getValueBst(register_emu->symbol,
                  calcStringUtf((char *)operation_member.register_dest));

  if (line == NULL) {
    printError(INCORRECT_VALUE, operation_member.location.filename,
               operation_member.location.line_content,
               operation_member.location.start.line,
               operation_member.location.start.column,
               operation_member.location.end.column);
    return -1;
  }

  struct Flags flags = register_emu->flags;

  // Check if one of these condition are valid for jump
  if ((operation_member.operation_type == Operation_Jmp) ||
      (operation_member.operation_type == Operation_Jo && flags.of) ||
      (operation_member.operation_type == Operation_Jno && !flags.of) ||
      (operation_member.operation_type == Operation_Jb && flags.cf) ||
      (operation_member.operation_type == Operation_Jnb && !flags.cf) ||
      (operation_member.operation_type == Operation_Jz && flags.zf) ||
      (operation_member.operation_type == Operation_Jnz && !flags.zf) ||
      (operation_member.operation_type == Operation_Jbe &&
       (flags.cf || flags.zf)) ||
      (operation_member.operation_type == Operation_Ja &&
       !(flags.cf && flags.zf)) ||
      (operation_member.operation_type == Operation_Js && flags.sf) ||
      (operation_member.operation_type == Operation_Jns && !flags.sf) ||
      (operation_member.operation_type == Operation_Jp && flags.pf) ||
      (operation_member.operation_type == Operation_Jnp && !flags.pf) ||
      (operation_member.operation_type == Operation_Jl &&
       flags.sf != flags.of) ||
      (operation_member.operation_type == Operation_Jnl &&
       flags.sf == flags.of) ||
      (operation_member.operation_type == Operation_Jle && flags.zf &&
       flags.sf != flags.of) ||
      (operation_member.operation_type == Operation_Jnle && !flags.zf &&
       flags.sf == flags.of)) {
    *i = line->value - 1;
  }

  return 0;
}

int manageDestSrc(struct OperationMember operation_member,
                  struct RegisterEmu *register_emu) {
  SET_ZERO_FLAGS(register_emu->flags)

  /*
   * Check if the operation_type is equal to "cmp" or "equ" and the src_type for
   * "equ" is egal to string If the src_value is equal to string and the
   * operation_type is not equal to equ throw an error
   */
  if (operation_member.src_type == Value_String &&
      operation_member.operation_type == Operation_Equ) {
    if (pushMemory(operation_member, register_emu) == -1) {
      return -1;
    }
    return 0;
  } else if (operation_member.operation_type == Operation_Cmp) {
    if (cmpValue(operation_member, register_emu) == -1) {
      return -1;
    }
    return 0;
  } else if (operation_member.src_type == Value_String &&
             operation_member.operation_type != Operation_Equ) {
    printError(INCORRECT_VALUE, operation_member.location.filename,
               operation_member.location.line_content,
               operation_member.location.start.line,
               operation_member.location.start.column,
               operation_member.location.end.column);
    return -1;
  }

  struct NodeValue value =
      getValue(&register_emu->hashmap, operation_member.register_dest);

  int64_t register_value = 0;
  int64_t src_value = 0;

  // Convert the register value to int64 for better manipulation
  if (value.node_value_type == NodeValue_Int64) {
    register_value = *(int64_t *)value.node_value;
  } else if (value.node_value_type == NodeValue_Int32) {
    register_value = *(int32_t *)value.node_value;
  } else if (value.node_value_type == NodeValue_Int16) {
    register_value = *(int16_t *)value.node_value;
  } else if (value.node_value_type == NodeValue_Int8 ||
             value.node_value_type == NodeValue_Int8_Low) {
    register_value = ((int8_t *)value.node_value)[0];
  } else if (value.node_value_type == NodeValue_Int8_High) {
    register_value = ((int8_t *)value.node_value)[1];
  } else {
    printError(INCORRECT_VALUE, operation_member.location.filename,
               operation_member.location.line_content,
               operation_member.location.start.line,
               operation_member.location.start.column,
               operation_member.location.end.column);
    return -1;
  }

  /*
   * If the src_type is equal to hex or number just take the value
   * If the src_type is equal to register take the value in the register hashmap
   * If the src_type is equal to Identifier calculate the sum of all utf8 string
   * Else is error
   */
  if (operation_member.src_type == Value_Hex ||
      operation_member.src_type == Value_Number) {
    src_value = operation_member.src_value.hex_number;
  } else if (operation_member.src_type == Value_Register) {
    struct NodeValue register_src_value =
        getValue(&register_emu->hashmap,
                 operation_member.src_value.string_register_identifier);

    src_value = *(int64_t *)register_src_value.node_value;
  } else if (operation_member.src_type == Value_Identifier) {
    src_value =
        calcStringUtf(operation_member.src_value.string_register_identifier);
  } else {
    printError(INCORRECT_VALUE, operation_member.location.filename,
               operation_member.location.line_content,
               operation_member.location.start.line,
               operation_member.location.start.column,
               operation_member.location.end.column);
    return -1;
  }

  if (operation_member.operation_type == Operation_Mov) {
    register_value = src_value;
  } else if (operation_member.operation_type == Operation_Add) {
    register_value += src_value;
    CHECK_IF_ADD_OVERFLOW(register_value, src_value, register_emu->flags,
                          value.node_value_type)
  } else if (operation_member.operation_type == Operation_Or) {
    register_value |= src_value;
  } else if (operation_member.operation_type == Operation_Sub) {
    register_value -= src_value;
    CHECK_IF_SUB_OVERFLOW(register_value, src_value, register_emu->flags,
                          value.node_value_type)
  } else if (operation_member.operation_type == Operation_Imul) {
    register_value *= src_value;
    CHECK_IF_MUL_OVERFLOW(register_value, src_value, register_emu->flags,
                          value.node_value_type)
  } else if (operation_member.operation_type == Operation_And) {
    register_value &= src_value;
  } else if (operation_member.operation_type == Operation_Xor) {
    register_value ^= src_value;
  }

  // Convert register_value to his respective register value with sized int
  if (value.node_value_type == NodeValue_Int64) {
    *(int64_t *)value.node_value = register_value;
  } else if (value.node_value_type == NodeValue_Int32) {
    *(int32_t *)value.node_value = register_value;
  } else if (value.node_value_type == NodeValue_Int16) {
    *(int16_t *)value.node_value = register_value;
  } else if (value.node_value_type == NodeValue_Int8 ||
             value.node_value_type == NodeValue_Int8_Low) {
    *(int8_t *)value.node_value = register_value;
  } else if (value.node_value_type == NodeValue_Int8_High) {
    ((int8_t *)value.node_value)[1] = (int8_t)register_value;
  }

  return 0;
}

void pushStack(struct RegisterEmu *register_emu, int64_t value) {
  struct StackNode node;
  struct Stack *stack = &register_emu->stack;

  // Push the value with his respective size to the stack
  if (value < INT32_MIN || value > INT32_MAX) {
    node.byte_size = 8;
    node.value.value_64bit = value;
  } else {
    node.byte_size = 4;
    node.value.value_32bit = value;
  }

  struct StackNode *temp = (struct StackNode *)realloc(
      stack->node, (stack->last + 1) * sizeof(struct StackNode));

  assert(temp != NULL);

  stack->node = temp;
  stack->node[stack->last - 1] = node;
  ++stack->last;
}

void popStack(struct RegisterEmu *register_emu, int64_t *register_value) {
  struct Stack *stack = &register_emu->stack;
  struct StackNode node = stack->node[stack->last - 2];

  // Pop the value with his respective size to the stack
  if (node.byte_size == 4) {
    *register_value = node.value.value_32bit;
  } else if (node.byte_size == 8) {
    *register_value = node.value.value_64bit;
  } else {
    assert(false);
  }
}

int pushMemory(struct OperationMember operation_member,
               struct RegisterEmu *register_emu) {
  if (operation_member.src_type == Value_Number ||
      operation_member.src_type == Value_Hex) {
    addKeyValueBst(&register_emu->memory,
                   calcStringUtf(operation_member.register_dest), 0, NULL);

  } else if (operation_member.src_type == Value_String) {
    addKeyValueBst(&register_emu->memory,
                   calcStringUtf(operation_member.register_dest), 0,
                   operation_member.src_value.string_register_identifier);
  } else {
    printError(INCORRECT_VALUE, operation_member.location.filename,
               operation_member.location.line_content,
               operation_member.location.start.line,
               operation_member.location.start.column,
               operation_member.location.end.column);
    return -1;
  }
  return 0;
}

int cmpValue(struct OperationMember operation_member,
             struct RegisterEmu *register_emu) {
  struct NodeValue register_node =
      getValue(&register_emu->hashmap, operation_member.register_dest);

  if (register_node.node_value_type == NodeValue_Null) {
    printError(EXPECT_REGISTER, operation_member.location.filename,
               operation_member.location.line_content,
               operation_member.location.start.line,
               operation_member.location.start.column,
               operation_member.location.end.column);
    return -1;
  }

  int64_t value = 0;
  int64_t register_value = 0;
  struct NodeValue *src_value = NULL;

  // Compare with only identifier or register as a src
  if (operation_member.src_type == Value_Identifier) {
    struct Bst *temp_bst = getValueBst(
        register_emu->symbol,
        calcStringUtf(operation_member.src_value.string_register_identifier));

    if (temp_bst == NULL) {
      printError(EXPECT_VALUE, operation_member.location.filename,
                 operation_member.location.line_content,
                 operation_member.location.start.line,
                 operation_member.location.start.column,
                 operation_member.location.end.column);
      return -1;
    }

    *(int32_t *)src_value = temp_bst->value;

  } else if (operation_member.src_type == Value_Register) {
    *src_value =
        getValue(&register_emu->hashmap,
                 operation_member.src_value.string_register_identifier);
  } else {
    printError(INCORRECT_VALUE, operation_member.location.filename,
               operation_member.location.line_content,
               operation_member.location.start.line,
               operation_member.location.start.column,
               operation_member.location.end.column);
    return -1;
  }

  if (src_value->node_value_type == NodeValue_Int64) {
    value = *(int64_t *)src_value->node_value;
  } else if (src_value->node_value_type == NodeValue_Int32) {
    value = *(int32_t *)src_value->node_value;
  } else if (src_value->node_value_type == NodeValue_Int16) {
    value = *(int16_t *)src_value->node_value;
  } else if (src_value->node_value_type == NodeValue_Int8 ||
             src_value->node_value_type == NodeValue_Int8_Low ||
             src_value->node_value_type == NodeValue_Int8_High) {
    value = *(int8_t *)src_value->node_value;
  } else {
    printError(INCORRECT_VALUE, operation_member.location.filename,
               operation_member.location.line_content,
               operation_member.location.start.line,
               operation_member.location.start.column,
               operation_member.location.end.column);
    return -1;
  }

  if (register_node.node_value_type == NodeValue_Int64) {
    register_value = *(int64_t *)register_node.node_value;
  } else if (register_node.node_value_type == NodeValue_Int32) {
    register_value = *(int32_t *)register_node.node_value;
  } else if (register_node.node_value_type == NodeValue_Int16) {
    register_value = *(int16_t *)register_node.node_value;
  } else if (register_node.node_value_type == NodeValue_Int8 ||
             register_node.node_value_type == NodeValue_Int8_Low) {
    register_value = *(int8_t *)register_node.node_value;
  } else if (register_node.node_value_type == NodeValue_Int8_High) {
    register_value = ((int8_t *)register_node.node_value)[1];
  } else {
    printError(INCORRECT_VALUE, operation_member.location.filename,
               operation_member.location.line_content,
               operation_member.location.start.line,
               operation_member.location.start.column,
               operation_member.location.end.column);
    return -1;
  }

  if (register_value == 0 || register_value == value) {
    register_emu->flags.zf = 1;
  } else if (register_value > value) {
    register_emu->flags.zf = 0;
    register_emu->flags.sf = register_emu->flags.of;
  } else if (register_value >= value) {
    register_emu->flags.of = register_emu->flags.sf;
  } else if (register_value < value) {
    register_emu->flags.sf = 0;
    register_emu->flags.of = 1;
  } else if (register_value <= value) {
    register_emu->flags.zf = 1;
    register_emu->flags.sf = 0;
    register_emu->flags.of = 1;
  }

  return 0;
}

void freeRegister(struct RegisterEmu *register_emu) {
  freeHashMap(&register_emu->hashmap);
  free(register_emu->stack.node);
  freeBst(register_emu->memory);
  freeBst(register_emu->symbol);
}

int doAllProcess(const char *file) {
  char *file_content = readFile(file);
  size_t file_size = strlen(file_content);

  const struct ContentInfo content_info = {
      .content = file_content, .filename = file, .content_size = file_size};

  struct TokenArray token = lexer(&content_info);
  free(file_content);

  struct Program *program = parse(&token, NULL);

  if (program == NULL) {
    freeToken(&token);
    return -1;
  }

  int status_code = runScript(program);

  freeProgram(program);
  freeToken(&token);

  return status_code;
}
