#ifndef THROW_H
#define THROW_H

#include "ast.h"
#include "token.h"

#define EXPECT_OPERAND 1, "Expect Operand"
#define EXPECT_KEYWORD 2, "Expect keyword"
#define EXPECT_VALUE 3, "Expect value"
#define EXPECT_SEMICOLON 4, "Expect semicolon ':'"
#define EXPECT_REGISTER 5, "Expect register"
#define EXPECT_COMMA 6, "Expect comma ','"
#define INCORRECT_VALUE 7, "Incorrect value"
#define INVALID_STRING 8, "Invalid string value"

#define STRING_NOT_IMPLEMENTED_YET 100, "String are not implemented yet"

#define INTERNAL_ERROR(message)                                                \
  300, "Internal error: " message " please report the problem at: "            \
       "https://github.com/ZbrDeev/AntAsm/issues"

enum ErrorType {
  Error_Arrow,
  Error_Plus,
};

void printError(int status, const char *reason, const char *filename,
                const char *line_content, size_t line, size_t start,
                size_t end);

#endif // THROW_H
