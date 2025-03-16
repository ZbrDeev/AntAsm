#ifndef THROW_H
#define THROW_H

#define INTERNAL_BAD_ALLOC                                                     \
  1, "Bad allocation of memory, please report the problem at: "                \
     "https://github.com/ZbrDeev/AntAsm/issues"
#define WRONG_KEYWORD 2, "Expect keyword"
#define WRONG_VALUE 3, "Expect value"
#define EXPECT_SEMICOLON 4, "Expect semicolon ':'"
#define EXPECT_REGISTER 5, "Expect register"
#define EXPECT_COMMA 6, "Expect comma ','"
#define STRING_NOT_IMPLEMENTED_YET 7, "String are not implemented yet"
#define INTERNAL_ERROR(message)                                                \
  8, "Internal error: " message                                                \
     " please report the problem at: https://github.com/ZbrDeev/AntAsm/issues"

void throwError(int status, const char *reason);

#endif // THROW_H
