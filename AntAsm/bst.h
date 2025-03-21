#ifndef BST_H
#define BST_H

#include <stdbool.h>
#include <stddef.h>

struct Bst {
  size_t sum_of_utf_key;
  const char *key;
  size_t value;

  struct Bst *left;
  struct Bst *right;
};

size_t calcStringUtf(const char *value);

struct Bst *preorderTraversal(struct Bst *bst, size_t sum_of_key,
                              bool add_node);

void addKeyValueBst(struct Bst *bst, const char *key, size_t value);

size_t getValueBst(struct Bst *bst, const char *key);

void freeBst(struct Bst *bst);

#endif // BST_H
