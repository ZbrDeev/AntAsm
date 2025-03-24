#ifndef BST_H
#define BST_H

#include <stdbool.h>
#include <stddef.h>

struct Bst {
  size_t sum_of_utf_key;
  size_t value;
  char *string_value;

  struct Bst *left;
  struct Bst *right;
};

size_t calcStringUtf(char *value);

struct Bst *createBstNode(size_t sum_of_key, size_t value, char *string_value);

void addKeyValueBst(struct Bst **bst, size_t sum_of_key, size_t value,
                    char *string_value);

struct Bst *getValueBst(struct Bst *bst, size_t sum_of_key);

void freeBst(struct Bst *bst);

#endif // BST_H
