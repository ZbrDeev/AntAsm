#ifndef PARSER_H
#define PARSER_H

#include "ast.h"
#include "token.h"

enum OperationType stringToOperationType(const char *string);

int intStringToInt(struct TokenArray *token_array, size_t index);

int hexStringToInt(struct TokenArray *token_array, size_t index);

int literalToValueType(struct OperationMember *operation_member,
                       struct TokenArray *token_array, size_t i);

struct Program *parse(struct TokenArray *token_array,
                      struct Bst *symb_repl_bst);

struct LabelMember *parseLabel(struct TokenArray *token_array, size_t *i,
                               struct MemberList *member_list);

struct OperationMember *parseOperationMember(struct TokenArray *token_array,
                                             size_t *i);

void parseOnlyDestOperation(struct TokenArray *token_array, size_t i,
                            struct OperationMember *operation_member);

int parseSrcDestOperation(struct TokenArray *token_array, size_t *i,
                          struct OperationMember *operation_member);

void freeProgram(struct Program *program);

#endif // PARSER_H
