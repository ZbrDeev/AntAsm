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
  struct RegisterEmu register_emu;
  INIT_REGISTER_EMU(register_emu)
  INIT_HASHMAP_REGISTER_EMU(register_emu)
  SET_ZERO_FLAGS(register_emu.flags)
  register_emu.stack.node =
      (struct StackNode *)malloc(sizeof(struct StackNode));
  register_emu.stack.last = 1;
  register_emu.memory = NULL;
  register_emu.symbol = program->member_list->symbol;
  int status_code = 0;

  for (size_t i = 0; i < program->size - 1; ++i) {
    struct MemberList member_list = program->member_list[i];

    if (member_list.member_list_type == OperationMemberType) {
      manageOperationType(member_list.member_list.operation_member,
                          &register_emu, &i);
    } else if (member_list.member_list_type == LabelMemberType) {
      manageOperationType(member_list.member_list.label_member.operation_member,
                          &register_emu, &i);
    }

    if (register_emu.exit) {
      status_code = register_emu.reg_DI;
    }
  }

  freeRegister(&register_emu);

  return status_code;
}

void manageOperationType(struct OperationMember operation_member,
                         struct RegisterEmu *register_emu, size_t *i) {
  if (operation_member.operation_type == Syscall) {
    int error_code = 0;

#if defined(__linux__)
    error_code = callSyscallLinux(register_emu);
#endif

    if (error_code != 0) {
      freeRegister(register_emu);
      throwError(error_code, "Syscall error",
                 operation_member.location.filename,
                 operation_member.location.line_content,
                 operation_member.location.start.line,
                 operation_member.location.start.column,
                 operation_member.location.end.column);
    }

    if (register_emu->exit) {
      return;
    }
  } else if (operation_member.operation_type == Push ||
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
    manageIdentifierAsADest(operation_member, register_emu, i);
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
    freeRegister(register_emu);
    throwError(INTERNAL_ERROR("Operation Type parsing"),
               operation_member.location.filename,
               operation_member.location.line_content,
               operation_member.location.start.line,
               operation_member.location.start.column,
               operation_member.location.end.column);
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
  } else if (value.node_value_type == Int8 ||
             value.node_value_type == Int8_Low) {
    register_value = *(int8_t *)value.node_value;
  } else if (value.node_value_type == Int8_High) {
    register_value = ((int8_t *)value.node_value)[1];
  } else {
    freeRegister(register_emu);
    throwError(INCORRECT_VALUE, operation_member.location.filename,
               operation_member.location.line_content,
               operation_member.location.start.line,
               operation_member.location.start.column,
               operation_member.location.end.column);
  }

  if (operation_member.operation_type == Inc) {
    ++register_value;
  } else if (operation_member.operation_type == Dec) {
    --register_value;
  } else if (operation_member.operation_type == Push) {
    pushStack(register_emu, register_value);
  } else if (operation_member.operation_type == Pop) {
    popStack(register_emu, &register_value, operation_member);
  } else {
    freeRegister(register_emu);
    throwError(7, "Other operation are not implemented yet",
               operation_member.location.filename,
               operation_member.location.line_content,
               operation_member.location.start.line,
               operation_member.location.start.column,
               operation_member.location.end.column);
  }

  if (value.node_value_type == Int64) {
    *(int64_t *)value.node_value = register_value;
  } else if (value.node_value_type == Int32) {
    *(int32_t *)value.node_value = register_value;
  } else if (value.node_value_type == Int16) {
    *(int16_t *)value.node_value = register_value;
  } else if (value.node_value_type == Int8 ||
             value.node_value_type == Int8_Low) {
    *(int8_t *)value.node_value = register_value;
  } else if (value.node_value_type == Int8_High) {
    ((int8_t *)value.node_value)[1] = (int8_t)register_value;
  }
}

void manageIdentifierAsADest(struct OperationMember operation_member,
                             struct RegisterEmu *register_emu, size_t *i) {
  size_t line =
      getValueBst(register_emu->symbol,
                  calcStringUtf((char *)operation_member.register_dest))
          ->value;

  struct Flags flags = register_emu->flags;

  if ((operation_member.operation_type == Jmp) ||
      (operation_member.operation_type == Jo && flags.of) ||
      (operation_member.operation_type == Jno && !flags.of) ||
      (operation_member.operation_type == Jb && flags.cf) ||
      (operation_member.operation_type == Jnb && !flags.cf) ||
      (operation_member.operation_type == Jz && flags.zf) ||
      (operation_member.operation_type == Jnz && !flags.zf) ||
      (operation_member.operation_type == Jbe && (flags.cf || flags.zf)) ||
      (operation_member.operation_type == Ja && !(flags.cf && flags.zf)) ||
      (operation_member.operation_type == Js && flags.sf) ||
      (operation_member.operation_type == Jns && !flags.sf) ||
      (operation_member.operation_type == Jp && flags.pf) ||
      (operation_member.operation_type == Jnp && !flags.pf) ||
      (operation_member.operation_type == Jl && flags.sf != flags.of) ||
      (operation_member.operation_type == Jnl && flags.sf == flags.of) ||
      (operation_member.operation_type == Jle && flags.zf &&
       flags.sf != flags.of) ||
      (operation_member.operation_type == Jnle && !flags.zf &&
       flags.sf == flags.of)) {
    *i = line - 1;
  }
}

void manageDestSrc(struct OperationMember operation_member,
                   struct RegisterEmu *register_emu) {
  SET_ZERO_FLAGS(register_emu->flags)

  if (operation_member.src_type == StringType &&
      operation_member.operation_type == Equ) {
    pushMemory(operation_member, register_emu);
    return;
  } else if (operation_member.operation_type == Cmp) {
    cmpValue(operation_member, register_emu);
    return;
  } else if (operation_member.src_type == StringType &&
             operation_member.operation_type != Equ) {
    freeRegister(register_emu);
    throwError(INCORRECT_VALUE, operation_member.location.filename,
               operation_member.location.line_content,
               operation_member.location.start.line,
               operation_member.location.start.column,
               operation_member.location.end.column);
  }

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
  } else if (value.node_value_type == Int8 ||
             value.node_value_type == Int8_Low) {
    register_value = ((int8_t *)value.node_value)[0];
  } else if (value.node_value_type == Int8_High) {
    register_value = ((int8_t *)value.node_value)[1];
  } else {
    freeRegister(register_emu);
    throwError(INCORRECT_VALUE, operation_member.location.filename,
               operation_member.location.line_content,
               operation_member.location.start.line,
               operation_member.location.start.column,
               operation_member.location.end.column);
  }

  if (operation_member.src_type == HexType ||
      operation_member.src_type == NumberType) {
    src_value = operation_member.src_value.hex_number;
  } else if (operation_member.src_type == RegisterType) {
    struct NodeValue register_src_value =
        getValue(&register_emu->hashmap,
                 operation_member.src_value.string_register_identifier);

    src_value = *(int64_t *)register_src_value.node_value;
  } else if (operation_member.src_type == IdentifierType) {
    src_value =
        calcStringUtf(operation_member.src_value.string_register_identifier);
  } else {
    freeRegister(register_emu);

    throwError(INCORRECT_VALUE, operation_member.location.filename,
               operation_member.location.line_content,
               operation_member.location.start.line,
               operation_member.location.start.column,
               operation_member.location.end.column);
  }

  if (operation_member.operation_type == Mov) {
    register_value = src_value;
  } else if (operation_member.operation_type == Add) {
    register_value += src_value;
    CHECK_IF_ADD_OVERFLOW(register_value, src_value, register_emu->flags,
                          value.node_value_type)
  } else if (operation_member.operation_type == Or) {
    register_value |= src_value;
  } else if (operation_member.operation_type == Sub) {
    register_value -= src_value;
    CHECK_IF_SUB_OVERFLOW(register_value, src_value, register_emu->flags,
                          value.node_value_type)
  } else if (operation_member.operation_type == Imul) {
    register_value *= src_value;
    CHECK_IF_MUL_OVERFLOW(register_value, src_value, register_emu->flags,
                          value.node_value_type)
  } else if (operation_member.operation_type == And) {
    register_value &= src_value;
  } else if (operation_member.operation_type == Xor) {
    register_value ^= src_value;
  }

  if (value.node_value_type == Int64) {
    *(int64_t *)value.node_value = register_value;
  } else if (value.node_value_type == Int32) {
    *(int32_t *)value.node_value = register_value;
  } else if (value.node_value_type == Int16) {
    *(int16_t *)value.node_value = register_value;
  } else if (value.node_value_type == Int8 ||
             value.node_value_type == Int8_Low) {
    *(int8_t *)value.node_value = register_value;
  } else if (value.node_value_type == Int8_High) {
    ((int8_t *)value.node_value)[1] = (int8_t)register_value;
  }
}

void pushStack(struct RegisterEmu *register_emu, int64_t value) {
  struct StackNode node;
  struct Stack *stack = &register_emu->stack;

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

void popStack(struct RegisterEmu *register_emu, int64_t *register_value,
              const struct OperationMember operation_member) {
  struct Stack *stack = &register_emu->stack;
  struct StackNode node = stack->node[stack->last - 2];

  if (node.byte_size == 4) {
    *register_value = node.value.value_32bit;
  } else if (node.byte_size == 8) {
    *register_value = node.value.value_64bit;
  } else {
    freeRegister(register_emu);
    throwError(INTERNAL_ERROR("Error with the stack node bit size"),
               operation_member.location.filename,
               operation_member.location.line_content,
               operation_member.location.start.line,
               operation_member.location.start.column,
               operation_member.location.end.column);
  }
}

void pushMemory(struct OperationMember operation_member,
                struct RegisterEmu *register_emu) {
  if (operation_member.src_type == NumberType ||
      operation_member.src_type == HexType) {
    addKeyValueBst(&register_emu->memory,
                   calcStringUtf(operation_member.register_dest), 0, NULL);

  } else if (operation_member.src_type == StringType) {
    addKeyValueBst(&register_emu->memory,
                   calcStringUtf(operation_member.register_dest), 0,
                   operation_member.src_value.string_register_identifier);
  } else {
    freeRegister(register_emu);
    throwError(INCORRECT_VALUE, operation_member.location.filename,
               operation_member.location.line_content,
               operation_member.location.start.line,
               operation_member.location.start.column,
               operation_member.location.end.column);
  }
}

void cmpValue(struct OperationMember operation_member,
              struct RegisterEmu *register_emu) {
  struct NodeValue register_node =
      getValue(&register_emu->hashmap, operation_member.register_dest);

  if (register_node.node_value_type == Null) {
    freeRegister(register_emu);
    throwError(EXPECT_REGISTER, operation_member.location.filename,
               operation_member.location.line_content,
               operation_member.location.start.line,
               operation_member.location.start.column,
               operation_member.location.end.column);
  }

  int64_t value = 0;
  int64_t register_value = 0;
  struct NodeValue *src_value = NULL;

  if (operation_member.src_type == StringType) {
    freeRegister(register_emu);
    throwError(INCORRECT_VALUE, operation_member.location.filename,
               operation_member.location.line_content,
               operation_member.location.start.line,
               operation_member.location.start.column,
               operation_member.location.end.column);
  } else if (operation_member.src_type == IdentifierType) {
    *(int32_t *)src_value =
        getValueBst(register_emu->symbol,
                    calcStringUtf(
                        operation_member.src_value.string_register_identifier))
            ->value;

  } else if (operation_member.src_type == RegisterType) {
    *src_value =
        getValue(&register_emu->hashmap,
                 operation_member.src_value.string_register_identifier);
  }

  if (src_value->node_value_type == Int64) {
    value = *(int64_t *)src_value->node_value;
  } else if (src_value->node_value_type == Int32) {
    value = *(int32_t *)src_value->node_value;
  } else if (src_value->node_value_type == Int16) {
    value = *(int16_t *)src_value->node_value;
  } else if (src_value->node_value_type == Int8 ||
             src_value->node_value_type == Int8_Low ||
             src_value->node_value_type == Int8_High) {
    value = *(int8_t *)src_value->node_value;
  } else {
    freeRegister(register_emu);
    throwError(INCORRECT_VALUE, operation_member.location.filename,
               operation_member.location.line_content,
               operation_member.location.start.line,
               operation_member.location.start.column,
               operation_member.location.end.column);
  }

  if (register_node.node_value_type == Int64) {
    register_value = *(int64_t *)register_node.node_value;
  } else if (register_node.node_value_type == Int32) {
    register_value = *(int32_t *)register_node.node_value;
  } else if (register_node.node_value_type == Int16) {
    register_value = *(int16_t *)register_node.node_value;
  } else if (register_node.node_value_type == Int8 ||
             register_node.node_value_type == Int8_Low) {
    register_value = *(int8_t *)register_node.node_value;
  } else if (register_node.node_value_type == Int8_High) {
    register_value = ((int8_t *)register_node.node_value)[1];
  } else {
    freeRegister(register_emu);
    throwError(INCORRECT_VALUE, operation_member.location.filename,
               operation_member.location.line_content,
               operation_member.location.start.line,
               operation_member.location.start.column,
               operation_member.location.end.column);
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

  struct Program program = parse(&token);

  int status_code = runScript(&program);

  free(program.member_list);
  freeToken(&token);

  return status_code;
}
