#ifndef PARSER_H
#define PARSER_H

#include "ast.h"
#include "token.h"

enum OperationType stringToOperationType(const char *string);

int intStringToInt(struct TokenArray *token_array, size_t i);

int hexStringToInt(struct TokenArray *token_array, size_t i,
                   struct MemberList *member_list);

void literalToValueType(struct OperationMember *operation_member,
                        struct TokenArray *token_array, size_t i,
                        struct MemberList *member_list);

struct Program parse(struct TokenArray *token_array);

struct LabelMember parseLabel(struct TokenArray *token_array, size_t *i,
                              struct MemberList *member_list);

struct OperationMember parseOperationMember(struct TokenArray *token_array,
                                            size_t *i,
                                            struct MemberList *member_list);

void parseOnlyDestOperation(struct TokenArray *token_array, size_t i,
                            struct OperationMember *operation_member);

void parseSrcDestOperation(struct TokenArray *token_array, size_t *i,
                           struct OperationMember *operation_member,
                           struct MemberList *member_list);

#endif // PARSER_H
