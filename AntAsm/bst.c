#include "bst.h"
#include <stdlib.h>
#include <string.h>

size_t calcStringUtf(const char *value) {
  size_t result = 0;

  for (size_t i = 0; i < strlen(value); ++i) {
    result += value[i];
  }

  return result;
}

struct Bst *preorderTraversal(struct Bst *bst, size_t sum_of_key,
                              bool add_node) {
  if (add_node && bst == NULL) {
    bst = (struct Bst *)malloc(sizeof(struct Bst));
    bst->sum_of_utf_key = 0;
    bst->left = NULL;
    bst->right = NULL;
    return bst;
  }

  if (bst->sum_of_utf_key == 0 || bst->sum_of_utf_key == sum_of_key) {
    return bst;
  }
  if (bst->sum_of_utf_key > sum_of_key) {
    return preorderTraversal(bst->left, sum_of_key, add_node);
  } else if (bst->sum_of_utf_key < sum_of_key) {
    return preorderTraversal(bst->right, sum_of_key, add_node);
  }

  return bst;
}

void addKeyValueBst(struct Bst *bst, const char *key, size_t value) {
  size_t sum_of_key = calcStringUtf(key);

  struct Bst *current_node = preorderTraversal(bst, sum_of_key, true);
  current_node->key = key;
  current_node->value = value;
  current_node->sum_of_utf_key = sum_of_key;
}

size_t getValueBst(struct Bst *bst, const char *key) {
  size_t sum_of_key = calcStringUtf(key);

  struct Bst *current_node = preorderTraversal(bst, sum_of_key, false);

  if (current_node == NULL) {
    // FOR NOW RETURN 0
    return 0;
  }

  return current_node->value;
}

void freeBst(struct Bst *bst) {
  if (bst->left != NULL) {
    freeBst(bst->left);
  }

  if (bst->right != NULL) {
    freeBst(bst);
  }

  free(bst);
}
