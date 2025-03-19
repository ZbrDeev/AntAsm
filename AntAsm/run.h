#ifndef RUN_H
#define RUN_H

#include "ast.h"
#include "hashmap.h"
#include <stdint.h>
#include <stdlib.h>

#define INIT_REGISTER_EMU(register)                                            \
  register.rax = 0;                                                            \
  register.eax = 0;                                                            \
  register.ax = 0;                                                             \
  register.ah = 0;                                                             \
  register.al = 0;                                                             \
  register.rbx = 0;                                                            \
  register.ebx = 0;                                                            \
  register.bx = 0;                                                             \
  register.bh = 0;                                                             \
  register.bl = 0;                                                             \
  register.rcx = 0;                                                            \
  register.ecx = 0;                                                            \
  register.cx = 0;                                                             \
  register.ch = 0;                                                             \
  register.cl = 0;                                                             \
  register.rdx = 0;                                                            \
  register.edx = 0;                                                            \
  register.dx = 0;                                                             \
  register.dh = 0;                                                             \
  register.dl = 0;                                                             \
  register.rsi = 0;                                                            \
  register.esi = 0;                                                            \
  register.si = 0;                                                             \
  register.sil = 0;                                                            \
  register.rdi = 0;                                                            \
  register.edi = 0;                                                            \
  register.di = 0;                                                             \
  register.dil = 0;                                                            \
  register.rsp = 0;                                                            \
  register.esp = 0;                                                            \
  register.sp = 0;                                                             \
  register.spl = 0;                                                            \
  register.rbp = 0;                                                            \
  register.ebp = 0;                                                            \
  register.bp = 0;                                                             \
  register.bpl = 0;                                                            \
  register.r8 = 0;                                                             \
  register.r8d = 0;                                                            \
  register.r8w = 0;                                                            \
  register.r8b = 0;                                                            \
  register.r9 = 0;                                                             \
  register.r9d = 0;                                                            \
  register.r9w = 0;                                                            \
  register.r9b = 0;                                                            \
  register.r10 = 0;                                                            \
  register.r10d = 0;                                                           \
  register.r10w = 0;                                                           \
  register.r10b = 0;                                                           \
  register.r11 = 0;                                                            \
  register.r11d = 0;                                                           \
  register.r11w = 0;                                                           \
  register.r11b = 0;                                                           \
  register.r12 = 0;                                                            \
  register.r12d = 0;                                                           \
  register.r12w = 0;                                                           \
  register.r12b = 0;                                                           \
  register.r13 = 0;                                                            \
  register.r13d = 0;                                                           \
  register.r13w = 0;                                                           \
  register.r13b = 0;                                                           \
  register.r14 = 0;                                                            \
  register.r14d = 0;                                                           \
  register.r14w = 0;                                                           \
  register.r14b = 0;                                                           \
  register.r15 = 0;                                                            \
  register.r15d = 0;                                                           \
  register.r15w = 0;                                                           \
  register.r15b = 0;

#define INIT_HASHMAP_REGISTER_EMU(register)                                    \
  register.hashmap = initHashMap(10);                                          \
  addKeyValue(&register.hashmap, "rax", (void *)&register.rax, Int64);         \
  addKeyValue(&register.hashmap, "eax", (void *)&register.eax, Int32);         \
  addKeyValue(&register.hashmap, "ax", (void *)&register.ax, Int16);           \
  addKeyValue(&register.hashmap, "ah", (void *)&register.ah, Int8);            \
  addKeyValue(&register.hashmap, "al", (void *)&register.al, Int8);            \
  addKeyValue(&register.hashmap, "rbx", (void *)&register.rbx, Int64);         \
  addKeyValue(&register.hashmap, "ebx", (void *)&register.ebx, Int32);         \
  addKeyValue(&register.hashmap, "bx", (void *)&register.bx, Int16);           \
  addKeyValue(&register.hashmap, "bh", (void *)&register.bh, Int8);            \
  addKeyValue(&register.hashmap, "bl", (void *)&register.bl, Int8);            \
  addKeyValue(&register.hashmap, "rcx", (void *)&register.rcx, Int64);         \
  addKeyValue(&register.hashmap, "ecx", (void *)&register.ecx, Int32);         \
  addKeyValue(&register.hashmap, "cx", (void *)&register.cx, Int16);           \
  addKeyValue(&register.hashmap, "ch", (void *)&register.ch, Int8);            \
  addKeyValue(&register.hashmap, "cl", (void *)&register.cl, Int8);            \
  addKeyValue(&register.hashmap, "rdx", (void *)&register.rdx, Int64);         \
  addKeyValue(&register.hashmap, "edx", (void *)&register.edx, Int32);         \
  addKeyValue(&register.hashmap, "dx", (void *)&register.dx, Int16);           \
  addKeyValue(&register.hashmap, "dh", (void *)&register.dh, Int8);            \
  addKeyValue(&register.hashmap, "dl", (void *)&register.dl, Int8);            \
  addKeyValue(&register.hashmap, "rsi", (void *)&register.rsi, Int64);         \
  addKeyValue(&register.hashmap, "esi", (void *)&register.esi, Int32);         \
  addKeyValue(&register.hashmap, "si", (void *)&register.si, Int16);           \
  addKeyValue(&register.hashmap, "sil", (void *)&register.sil, Int8);          \
  addKeyValue(&register.hashmap, "rdi", (void *)&register.rdi, Int64);         \
  addKeyValue(&register.hashmap, "edi", (void *)&register.edi, Int32);         \
  addKeyValue(&register.hashmap, "di", (void *)&register.di, Int16);           \
  addKeyValue(&register.hashmap, "dil", (void *)&register.dil, Int8);          \
  addKeyValue(&register.hashmap, "rsp", (void *)&register.rsp, Int64);         \
  addKeyValue(&register.hashmap, "esp", (void *)&register.esp, Int32);         \
  addKeyValue(&register.hashmap, "sp", (void *)&register.sp, Int16);           \
  addKeyValue(&register.hashmap, "spl", (void *)&register.spl, Int8);          \
  addKeyValue(&register.hashmap, "rbp", (void *)&register.rbp, Int64);         \
  addKeyValue(&register.hashmap, "ebp", (void *)&register.ebp, Int32);         \
  addKeyValue(&register.hashmap, "bp", (void *)&register.bp, Int16);           \
  addKeyValue(&register.hashmap, "bpl", (void *)&register.bpl, Int8);          \
  addKeyValue(&register.hashmap, "r8", (void *)&register.r8, Int64);           \
  addKeyValue(&register.hashmap, "r8d", (void *)&register.r8d, Int32);         \
  addKeyValue(&register.hashmap, "r8w", (void *)&register.r8w, Int16);         \
  addKeyValue(&register.hashmap, "r8b", (void *)&register.r8b, Int8);          \
  addKeyValue(&register.hashmap, "r9", (void *)&register.r9, Int64);           \
  addKeyValue(&register.hashmap, "r9d", (void *)&register.r9d, Int32);         \
  addKeyValue(&register.hashmap, "r9w", (void *)&register.r9w, Int16);         \
  addKeyValue(&register.hashmap, "r9b", (void *)&register.r9b, Int8);          \
  addKeyValue(&register.hashmap, "r10", (void *)&register.r10, Int64);         \
  addKeyValue(&register.hashmap, "r10d", (void *)&register.r10d, Int32);       \
  addKeyValue(&register.hashmap, "r10w", (void *)&register.r10w, Int16);       \
  addKeyValue(&register.hashmap, "r10b", (void *)&register.r10b, Int8);        \
  addKeyValue(&register.hashmap, "r11", (void *)&register.r11, Int64);         \
  addKeyValue(&register.hashmap, "r11d", (void *)&register.r11d, Int32);       \
  addKeyValue(&register.hashmap, "r11w", (void *)&register.r11w, Int16);       \
  addKeyValue(&register.hashmap, "r11b", (void *)&register.r11b, Int8);        \
  addKeyValue(&register.hashmap, "r12", (void *)&register.r12, Int64);         \
  addKeyValue(&register.hashmap, "r12d", (void *)&register.r12d, Int32);       \
  addKeyValue(&register.hashmap, "r12w", (void *)&register.r12w, Int16);       \
  addKeyValue(&register.hashmap, "r12b", (void *)&register.r12b, Int8);        \
  addKeyValue(&register.hashmap, "r13", (void *)&register.r13, Int64);         \
  addKeyValue(&register.hashmap, "r13d", (void *)&register.r13d, Int32);       \
  addKeyValue(&register.hashmap, "r13w", (void *)&register.r13w, Int16);       \
  addKeyValue(&register.hashmap, "r13b", (void *)&register.r13b, Int8);        \
  addKeyValue(&register.hashmap, "r14", (void *)&register.r14, Int64);         \
  addKeyValue(&register.hashmap, "r14d", (void *)&register.r14d, Int32);       \
  addKeyValue(&register.hashmap, "r14w", (void *)&register.r14w, Int16);       \
  addKeyValue(&register.hashmap, "r14b", (void *)&register.r14b, Int8);        \
  addKeyValue(&register.hashmap, "r15", (void *)&register.r15, Int64);         \
  addKeyValue(&register.hashmap, "r15d", (void *)&register.r15d, Int32);       \
  addKeyValue(&register.hashmap, "r15w", (void *)&register.r15w, Int16);       \
  addKeyValue(&register.hashmap, "r15b", (void *)&register.r15b, Int8);

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

struct RegisterEmu {
  int64_t rax;
  int32_t eax;
  int16_t ax;
  int8_t ah;
  int8_t al;

  int64_t rbx;
  int32_t ebx;
  int16_t bx;
  int8_t bh;
  int8_t bl;

  int64_t rcx;
  int32_t ecx;
  int16_t cx;
  int8_t ch;
  int8_t cl;

  int64_t rdx;
  int32_t edx;
  int16_t dx;
  int8_t dh;
  int8_t dl;

  int64_t rsi;
  int32_t esi;
  int16_t si;
  int8_t sil;

  int64_t rdi;
  int32_t edi;
  int16_t di;
  int8_t dil;

  int64_t rsp;
  int32_t esp;
  int16_t sp;
  int8_t spl;

  int64_t rbp;
  int32_t ebp;
  int16_t bp;
  int8_t bpl;

  int64_t r8;
  int32_t r8d;
  int16_t r8w;
  int8_t r8b;

  int64_t r9;
  int32_t r9d;
  int16_t r9w;
  int8_t r9b;

  int64_t r10;
  int32_t r10d;
  int16_t r10w;
  int8_t r10b;

  int64_t r11;
  int32_t r11d;
  int16_t r11w;
  int8_t r11b;

  int64_t r12;
  int32_t r12d;
  int16_t r12w;
  int8_t r12b;

  int64_t r13;
  int32_t r13d;
  int16_t r13w;
  int8_t r13b;

  int64_t r14;
  int32_t r14d;
  int16_t r14w;
  int8_t r14b;

  int64_t r15;
  int32_t r15d;
  int16_t r15w;
  int8_t r15b;

  struct HashMap hashmap;
  struct Stack stack;
  struct HashMap memory;
};

void runScript(struct Program *program);

void manageOperationType(struct OperationMember operation_member,
                         struct RegisterEmu *register_emu);

void manageOnlyRegisterDest(struct OperationMember operation_member,
                            struct RegisterEmu *register_emu);

void manageIdentifierAsADest(struct OperationMember operation_member,
                             struct RegisterEmu *register_emu);

void manageDestSrc(struct OperationMember operation_member,
                   struct RegisterEmu *register_emu);

void pushStack(struct RegisterEmu *register_emu, int64_t value);

void popStack(struct RegisterEmu *register_emu, int64_t *register_value,
              const struct OperationMember operation_member);

void pushMemory(struct OperationMember operation_member,
                struct RegisterEmu *register_emu);

void doAllProcess(const char *file);

#endif // RUN_H
