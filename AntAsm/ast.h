#ifndef AST_H
#define AST_H

#include "bst.h"
#include "hashmap.h"
#include <stdint.h>

struct Position {
  size_t line;
  size_t column;
};

struct SourceLocation {
  struct Position start;
  struct Position end;
  const char *filename;
  const char *line_content;
};

enum OperationType {
  Operation_Mov,
  Operation_Add,
  Operation_Cmp,
  Operation_Or,
  Operation_Jmp,
  Operation_Sub,
  Operation_Imul,
  Operation_Push,
  Operation_Pop,
  Operation_Inc,
  Operation_Dec,
  Operation_And,
  Operation_Xor,
  Operation_Jo,
  Operation_Jno,
  Operation_Jb,
  Operation_Jnb,
  Operation_Jz,
  Operation_Jnz,
  Operation_Jbe,
  Operation_Ja,
  Operation_Js,
  Operation_Jns,
  Operation_Jp,
  Operation_Jnp,
  Operation_Jl,
  Operation_Jnl,
  Operation_Jle,
  Operation_Jnle,
  Operation_Equ,
  Operation_Syscall,
};

enum ValueType {
  Value_String,
  Value_Number,
  Value_Hex,
  Value_Register,
  Value_Identifier,
  Value_Null,
};

union ValueTypeUnion {
  int64_t hex_number;
  char *string_register_identifier;
};

struct OperationMember {
  enum OperationType operation_type;

  char *register_dest;
  enum ValueType src_type;
  union ValueTypeUnion src_value;

  struct SourceLocation location;
};

struct LabelMember {
  char *label_name;

  struct OperationMember operation_member;

  struct SourceLocation location;
};

union MemberListUnion {
  struct OperationMember operation_member;
  struct LabelMember label_member;
};

enum MemberListEnum {
  MemberList_OperationMember,
  MemberList_LabelMember,
};

struct MemberList {
  enum MemberListEnum member_list_type;
  union MemberListUnion member_list;

  struct Bst *symbol;
};

struct Program {
  size_t size;

  struct MemberList *member_list;
};

#endif // AST_H
