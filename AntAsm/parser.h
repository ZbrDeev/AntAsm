#ifndef PARSER_H
#define PARSER_H

#include "ast.h"
#include "token.h"

enum OperationType stringToOperationType(const char *string);

int intStringToInt(const char *int_value);

int hexStringToInt(const char *hex_value);

void literalToValueType(struct OperationMember *operation_member,
                        enum TokenType token_type, const char *value);

struct Program parse(const struct TokenArray *token_array);

struct LabelMember parseLabel(const struct TokenArray *token_array, size_t *i);

struct OperationMember
parseOperationMember(const struct TokenArray *token_array, size_t *i);

void parseOnlyDestOperation(const struct TokenArray *token_array, size_t i,
                            struct OperationMember *operation_member);

void parseSrcDestOperation(const struct TokenArray *token_array, size_t *i,
                           struct OperationMember *operation_member);

#endif // PARSER_H
