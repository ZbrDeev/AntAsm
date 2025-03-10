#ifndef AST_H
#define AST_H

#define SRC_DEST_OPERATION                                                     \
  const char *register_dest;                                                   \
  ValueType value_src;

#define ONLY_DEST_OPERATION const char *register_dest;

enum ValueType {
  String,
  Number,
  Hex,
  Register,
};

struct Position {
  unsigned int line;
  unsigned int column;
};

struct SourceLocation {
  Position start;
  Position end;
};

struct MovOperation {
  SRC_DEST_OPERATION
};

struct AddOperation {
  SRC_DEST_OPERATION
};

struct CmpOperation {
  SRC_DEST_OPERATION
};

struct OrOperation {
  SRC_DEST_OPERATION
};

struct JmpOperation {
  ONLY_DEST_OPERATION
};

struct SubOperation {
  SRC_DEST_OPERATION
};

struct ImulOperation {
  SRC_DEST_OPERATION
};

struct PushOperation {
  ONLY_DEST_OPERATION
};

struct PopOperation {
  ONLY_DEST_OPERATION
};

struct IncOperation {
  ONLY_DEST_OPERATION
};

struct DecOperation {
  ONLY_DEST_OPERATION
};

struct AndOperation {
  SRC_DEST_OPERATION
};

struct XorOperation {
  SRC_DEST_OPERATION
};

struct JoOperation {
  ONLY_DEST_OPERATION
};

struct JnoOperation {
  ONLY_DEST_OPERATION
};

struct JbOperation {
  ONLY_DEST_OPERATION
};

struct JnbOperation {
  ONLY_DEST_OPERATION
};

struct JzOperation {
  ONLY_DEST_OPERATION
};

struct JnzOperation {
  ONLY_DEST_OPERATION
};

struct JbeOperation {
  ONLY_DEST_OPERATION
};

struct JaOperation {
  ONLY_DEST_OPERATION
};

struct JsOperation {
  ONLY_DEST_OPERATION
};

struct JnsOperation {
  ONLY_DEST_OPERATION
};

struct JpOperation {
  ONLY_DEST_OPERATION
};

struct JnpOperation {
  ONLY_DEST_OPERATION
};

struct JlOperation {
  ONLY_DEST_OPERATION
};

struct JnlOperation {
  ONLY_DEST_OPERATION
};

struct JleOperation {
  ONLY_DEST_OPERATION
};

struct JnleOperation {
  ONLY_DEST_OPERATION
};

struct EquOperation {
  const char *identifier;
  enum {
    String,
    Number,
    Hex,
  } value;
};

#endif // AST_H
