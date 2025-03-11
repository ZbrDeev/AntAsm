#ifndef PARSER_H
#define PARSER_H

#include "ast.h"
#include "token.h"

enum OperationType stringToOperationType(const char *string);

enum ValueType literalToValue(enum TokenType token_type);

struct Program parse(const struct TokenArray *token_array);

void parseOnlyDestOperation(const struct TokenArray *token_array, size_t i,
                            struct OperationMember *operation_member);

void parseSrcDestOperation(const struct TokenArray *token_array, size_t *i,
                           struct OperationMember *operation_member);

#endif // PARSER_H
