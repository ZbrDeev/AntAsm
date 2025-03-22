#ifndef RUN_H
#define RUN_H

#include "ast.h"
#include "bst.h"
#include "hashmap.h"
#include <stdint.h>
#include <stdlib.h>

#define INIT_REGISTER_EMU(register)                                            \
  register.reg_A = 0;                                                          \
  register.reg_B = 0;                                                          \
  register.reg_C = 0;                                                          \
  register.reg_D = 0;                                                          \
  register.reg_SI = 0;                                                         \
  register.reg_DI = 0;                                                         \
  register.reg_SP = 0;                                                         \
  register.reg_BP = 0;                                                         \
  register.reg_8 = 0;                                                          \
  register.reg_9 = 0;                                                          \
  register.reg_10 = 0;                                                         \
  register.reg_11 = 0;                                                         \
  register.reg_12 = 0;                                                         \
  register.reg_13 = 0;                                                         \
  register.reg_14 = 0;                                                         \
  register.reg_15 = 0;

#define INIT_HASHMAP_REGISTER_EMU(register)                                    \
  register.hashmap = initHashMap(10);                                          \
  addKeyValue(&register.hashmap, "rax", (void *)&register.reg_A, Int64);       \
  addKeyValue(&register.hashmap, "eax", (void *)&register.reg_A, Int32);       \
  addKeyValue(&register.hashmap, "ax", (void *)&register.reg_A, Int16);        \
  addKeyValue(&register.hashmap, "ah", (void *)&register.reg_A, Int8_High);    \
  addKeyValue(&register.hashmap, "al", (void *)&register.reg_A, Int8_Low);     \
  addKeyValue(&register.hashmap, "rbx", (void *)&register.reg_B, Int64);       \
  addKeyValue(&register.hashmap, "ebx", (void *)&register.reg_B, Int32);       \
  addKeyValue(&register.hashmap, "bx", (void *)&register.reg_B, Int16);        \
  addKeyValue(&register.hashmap, "bh", (void *)&register.reg_B, Int8_High);    \
  addKeyValue(&register.hashmap, "bl", (void *)&register.reg_B, Int8_Low);     \
  addKeyValue(&register.hashmap, "rcx", (void *)&register.reg_C, Int64);       \
  addKeyValue(&register.hashmap, "ecx", (void *)&register.reg_C, Int32);       \
  addKeyValue(&register.hashmap, "cx", (void *)&register.reg_C, Int16);        \
  addKeyValue(&register.hashmap, "ch", (void *)&register.reg_C, Int8_High);    \
  addKeyValue(&register.hashmap, "cl", (void *)&register.reg_C, Int8_Low);     \
  addKeyValue(&register.hashmap, "rdx", (void *)&register.reg_D, Int64);       \
  addKeyValue(&register.hashmap, "edx", (void *)&register.reg_D, Int32);       \
  addKeyValue(&register.hashmap, "dx", (void *)&register.reg_D, Int16);        \
  addKeyValue(&register.hashmap, "dh", (void *)&register.reg_D, Int8_High);    \
  addKeyValue(&register.hashmap, "dl", (void *)&register.reg_D, Int8_Low);     \
  addKeyValue(&register.hashmap, "rsi", (void *)&register.reg_SI, Int64);      \
  addKeyValue(&register.hashmap, "esi", (void *)&register.reg_SI, Int32);      \
  addKeyValue(&register.hashmap, "si", (void *)&register.reg_SI, Int16);       \
  addKeyValue(&register.hashmap, "sil", (void *)&register.reg_SI, Int8);       \
  addKeyValue(&register.hashmap, "rdi", (void *)&register.reg_DI, Int64);      \
  addKeyValue(&register.hashmap, "edi", (void *)&register.reg_DI, Int32);      \
  addKeyValue(&register.hashmap, "di", (void *)&register.reg_DI, Int16);       \
  addKeyValue(&register.hashmap, "dil", (void *)&register.reg_DI, Int8);       \
  addKeyValue(&register.hashmap, "rsp", (void *)&register.reg_SP, Int64);      \
  addKeyValue(&register.hashmap, "esp", (void *)&register.reg_SP, Int32);      \
  addKeyValue(&register.hashmap, "sp", (void *)&register.reg_SP, Int16);       \
  addKeyValue(&register.hashmap, "spl", (void *)&register.reg_SP, Int8);       \
  addKeyValue(&register.hashmap, "rbp", (void *)&register.reg_BP, Int64);      \
  addKeyValue(&register.hashmap, "ebp", (void *)&register.reg_BP, Int32);      \
  addKeyValue(&register.hashmap, "bp", (void *)&register.reg_BP, Int16);       \
  addKeyValue(&register.hashmap, "bpl", (void *)&register.reg_BP, Int8);       \
  addKeyValue(&register.hashmap, "r8", (void *)&register.reg_8, Int64);        \
  addKeyValue(&register.hashmap, "r8d", (void *)&register.reg_8, Int32);       \
  addKeyValue(&register.hashmap, "r8w", (void *)&register.reg_8, Int16);       \
  addKeyValue(&register.hashmap, "r8b", (void *)&register.reg_8, Int8);        \
  addKeyValue(&register.hashmap, "r9", (void *)&register.reg_9, Int64);        \
  addKeyValue(&register.hashmap, "r9d", (void *)&register.reg_9, Int32);       \
  addKeyValue(&register.hashmap, "r9w", (void *)&register.reg_9, Int16);       \
  addKeyValue(&register.hashmap, "r9b", (void *)&register.reg_9, Int8);        \
  addKeyValue(&register.hashmap, "r10", (void *)&register.reg_10, Int64);      \
  addKeyValue(&register.hashmap, "r10d", (void *)&register.reg_10, Int32);     \
  addKeyValue(&register.hashmap, "r10w", (void *)&register.reg_10, Int16);     \
  addKeyValue(&register.hashmap, "r10b", (void *)&register.reg_10, Int8);      \
  addKeyValue(&register.hashmap, "r11", (void *)&register.reg_11, Int64);      \
  addKeyValue(&register.hashmap, "r11d", (void *)&register.reg_11, Int32);     \
  addKeyValue(&register.hashmap, "r11w", (void *)&register.reg_11, Int16);     \
  addKeyValue(&register.hashmap, "r11b", (void *)&register.reg_11, Int8);      \
  addKeyValue(&register.hashmap, "r12", (void *)&register.reg_12, Int64);      \
  addKeyValue(&register.hashmap, "r12d", (void *)&register.reg_12, Int32);     \
  addKeyValue(&register.hashmap, "r12w", (void *)&register.reg_12, Int16);     \
  addKeyValue(&register.hashmap, "r12b", (void *)&register.reg_12, Int8);      \
  addKeyValue(&register.hashmap, "r13", (void *)&register.reg_13, Int64);      \
  addKeyValue(&register.hashmap, "r13d", (void *)&register.reg_13, Int32);     \
  addKeyValue(&register.hashmap, "r13w", (void *)&register.reg_13, Int16);     \
  addKeyValue(&register.hashmap, "r13b", (void *)&register.reg_13, Int8);      \
  addKeyValue(&register.hashmap, "r14", (void *)&register.reg_14, Int64);      \
  addKeyValue(&register.hashmap, "r14d", (void *)&register.reg_14, Int32);     \
  addKeyValue(&register.hashmap, "r14w", (void *)&register.reg_14, Int16);     \
  addKeyValue(&register.hashmap, "r14b", (void *)&register.reg_14, Int8);      \
  addKeyValue(&register.hashmap, "r15", (void *)&register.reg_15, Int64);      \
  addKeyValue(&register.hashmap, "r15d", (void *)&register.reg_15, Int32);     \
  addKeyValue(&register.hashmap, "r15w", (void *)&register.reg_15, Int16);     \
  addKeyValue(&register.hashmap, "r15b", (void *)&register.reg_15, Int8);

#define CHECK_IF_ADD_OVERFLOW(a, b, flags, type)                               \
  if ((type == Int64 && __builtin_add_overflow_p(a, b, (int64_t)a)) ||         \
      (type == Int32 && __builtin_add_overflow_p(a, b, (int32_t)a)) ||         \
      (type == Int16 && __builtin_add_overflow_p(a, b, (int16_t)a)) ||         \
      ((type == Int8 || type == Int8_Low || type == Int8_High) &&              \
       __builtin_add_overflow_p(a, b, (int8_t)a))) {                           \
    flags.of = 1;                                                              \
  }

#define CHECK_IF_SUB_OVERFLOW(a, b, flags, type)                               \
  if ((type == Int64 && __builtin_sub_overflow_p(a, b, (int64_t)a)) ||         \
      (type == Int32 && __builtin_sub_overflow_p(a, b, (int32_t)a)) ||         \
      (type == Int16 && __builtin_sub_overflow_p(a, b, (int16_t)a)) ||         \
      ((type == Int8 || type == Int8_Low || type == Int8_High) &&              \
       __builtin_sub_overflow_p(a, b, (int8_t)a))) {                           \
    flags.of = 1;                                                              \
  }

#define CHECK_IF_MUL_OVERFLOW(a, b, flags, type)                               \
  if ((type == Int64 && __builtin_mul_overflow_p(a, b, (int64_t)a)) ||         \
      (type == Int32 && __builtin_mul_overflow_p(a, b, (int32_t)a)) ||         \
      (type == Int16 && __builtin_mul_overflow_p(a, b, (int16_t)a)) ||         \
      ((type == Int8 || type == Int8_Low || type == Int8_High) &&              \
       __builtin_mul_overflow_p(a, b, (int8_t)a))) {                           \
    flags.of = 1;                                                              \
  }

#define SET_ZERO_FLAGS(flags)                                                  \
  flags.of = 0;                                                                \
  flags.sf = 0;                                                                \
  flags.zf = 0;                                                                \
  flags.cf = 0;                                                                \
  flags.pf = 0;

union StackNodeValue {
  int32_t value_32bit;
  int64_t value_64bit;
};

struct StackNode {
  int8_t byte_size;

  union StackNodeValue value;
};

struct Stack {
  size_t last;

  struct StackNode *node;
};

struct Flags {
  bool of;
  bool sf;
  bool zf;
  bool cf;
  bool pf;
};

struct RegisterEmu {
  int64_t reg_A;
  int64_t reg_B;
  int64_t reg_C;
  int64_t reg_D;
  int64_t reg_SI;
  int64_t reg_DI;
  int64_t reg_SP;
  int64_t reg_BP;
  int64_t reg_8;
  int64_t reg_9;
  int64_t reg_10;
  int64_t reg_11;
  int64_t reg_12;
  int64_t reg_13;
  int64_t reg_14;
  int64_t reg_15;

  struct HashMap hashmap;
  struct Stack stack;
  struct HashMap memory;
  struct Bst *symbol;
  struct Flags flags;

  bool exit;
};

void runScript(struct Program *program);

void manageOperationType(struct OperationMember operation_member,
                         struct RegisterEmu *register_emu, size_t *i);

void manageOnlyRegisterDest(struct OperationMember operation_member,
                            struct RegisterEmu *register_emu);

void manageIdentifierAsADest(struct OperationMember operation_member,
                             struct RegisterEmu *register_emu, size_t *i);

void manageDestSrc(struct OperationMember operation_member,
                   struct RegisterEmu *register_emu);

void pushStack(struct RegisterEmu *register_emu, int64_t value);

void popStack(struct RegisterEmu *register_emu, int64_t *register_value,
              const struct OperationMember operation_member);

void pushMemory(struct OperationMember operation_member,
                struct RegisterEmu *register_emu);

void cmpValue(struct OperationMember operation_member,
              struct RegisterEmu *register_emu);

void freeRegister(struct RegisterEmu *register_emu);

void doAllProcess(const char *file);

#endif // RUN_H
