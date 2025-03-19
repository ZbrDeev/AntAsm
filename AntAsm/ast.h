#ifndef AST_H
#define AST_H

#include <stdint.h>

struct Position {
  unsigned int line;
  unsigned int column;
};

struct SourceLocation {
  struct Position start;
  struct Position end;
  const char *filename;
  const char *line_content;
};

enum OperationType {
  Mov,
  Add,
  Cmp,
  Or,
  Jmp,
  Sub,
  Imul,
  Push,
  Pop,
  Inc,
  Dec,
  And,
  Xor,
  Jo,
  Jno,
  Jb,
  Jnb,
  Jz,
  Jnz,
  Jbe,
  Ja,
  Js,
  Jns,
  Jp,
  Jnp,
  Jl,
  Jnl,
  Jle,
  Jnle,
  Equ,
};

enum ValueType {
  StringType,
  NumberType,
  HexType,
  RegisterType,
  IdentifierType,
  NullType,
};

union ValueTypeUnion {
  int64_t hex_number;
  const char *string_register_identifier;
};

struct OperationMember {
  enum OperationType operation_type;

  const char *register_dest;
  enum ValueType src_type;
  union ValueTypeUnion src_value;

  struct SourceLocation location;
};

struct LabelMember {
  const char *label_name;

  struct OperationMember operation_member;

  struct SourceLocation location;
};

union MemberListUnion {
  struct OperationMember operation_member;
  struct LabelMember label_member;
};

enum MemberListEnum {
  OperationMemberType,
  LabelMemberType,
};

struct MemberList {
  enum MemberListEnum member_list_type;

  union MemberListUnion member_list;
};

struct Program {
  unsigned int size;

  struct MemberList *member_list;
};

#endif // AST_H
