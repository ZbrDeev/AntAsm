#include "bst.h"
#include <stdlib.h>
#include <string.h>

size_t calcStringUtf(char *value) {
  size_t result = 0;

  for (size_t i = 0; i < strlen(value); ++i) {
    result += value[i];
  }

  return result;
}

struct Bst *createBstNode(size_t sum_of_key, size_t value, char *string_value) {
  struct Bst *bst_node = (struct Bst *)malloc(sizeof(struct Bst));
  bst_node->left = NULL;
  bst_node->right = NULL;
  bst_node->sum_of_utf_key = sum_of_key;
  bst_node->value = value;
  bst_node->string_value = string_value;

  return bst_node;
}

void addKeyValueBst(struct Bst **bst, size_t sum_of_key, size_t value,
                    char *string_value) {
  if (*bst == NULL || (*bst)->sum_of_utf_key == 0) {
    *bst = createBstNode(sum_of_key, value, string_value);
  } else if ((*bst)->sum_of_utf_key < sum_of_key) {
    addKeyValueBst(&(*bst)->left, sum_of_key, value, string_value);
  } else {
    addKeyValueBst(&(*bst)->right, sum_of_key, value, string_value);
  }
}

struct Bst *getValueBst(struct Bst *bst, size_t sum_of_key) {
  if (bst == NULL) {
    return NULL;
  } else if (bst->sum_of_utf_key == sum_of_key) {
    return bst;
  } else if (bst->sum_of_utf_key < sum_of_key) {
    return getValueBst(bst->left, sum_of_key);
  } else {
    return getValueBst(bst->right, sum_of_key);
  }
}

void freeBst(struct Bst *bst) {
  if (bst == NULL) {
    return;
  }

  if (bst->left != NULL) {
    freeBst(bst->left);
  }

  if (bst->right != NULL) {
    freeBst(bst);
  }

  free(bst->string_value);
  free(bst);
}
