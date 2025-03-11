#ifndef AST_H
#define AST_H

#define SRC_DEST_OPERATION                                                     \
  const char *register_dest;                                                   \
  ValueType value_src;

#define ONLY_DEST_OPERATION const char *register_dest;

enum ValueType {
  StringType,
  NumberType,
  HexType,
  RegisterType,
};

struct Position {
  unsigned int line;
  unsigned int column;
};

struct SourceLocation {
  struct Position start;
  struct Position end;
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

struct OperationMember {
  enum OperationType operation_type;

  const char *register_dest;
  enum ValueType src_type;
  const char *src_value;

  struct SourceLocation *location;
};

struct Program {
  unsigned int size;

  struct OperationMember *member_list;
};

#endif // AST_H
