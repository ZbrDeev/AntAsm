#include "hashmap.h"
#include <stdlib.h>
#include <string.h>

int stringToSum(const char *string) {
  size_t size = strlen(string);
  int result = 0;

  for (size_t i = 0; i < size; ++i) {
    result += string[i];
  }

  return result;
}

struct HashMap initHashMap(unsigned int capacity) {
  struct HashMap hashmap;

  hashmap.capacity = capacity;
  hashmap.nodeList = (struct Node *)malloc(capacity * sizeof(struct Node));

  for (size_t i = 0; i < capacity; ++i) {
    hashmap.nodeList[i].next = NULL;
    hashmap.nodeList[i].is_empty = true;
  }

  return hashmap;
}

static struct NodeValue navigateIntoNode(struct Node *node, const char *key) {
  struct Node *node_it = node;

  while (node_it->next != NULL) {
    if (strcmp(key, node_it->key) == 0) {
      return node_it->value;
    }

    node_it = node_it->next;
  }

  if (strcmp(key, node_it->key) == 0) {
    return node_it->value;
  }

  struct NodeValue node_null_value = {.node_value_type = Null,
                                      .node_value = NULL};
  return node_null_value;
}

struct NodeValue getValue(struct HashMap *hashmap, const char *key) {
  int sum_of_key = stringToSum(key);
  int index = CALC_KEY_INDEX(sum_of_key, hashmap->capacity);

  struct Node node = hashmap->nodeList[index];

  if (node.is_empty) {
    return node.value;
  }

  return navigateIntoNode(&node, key);
}

void addKeyValue(struct HashMap *hashmap, const char *key, void *value_ptr,
                 const enum NodeValueType value_type) {
  struct NodeValue value = {.node_value = value_ptr,
                            .node_value_type = value_type};
  struct Node node = {.key = key, .value = value};
  int sum_of_key = stringToSum(key);
  int index = CALC_KEY_INDEX(sum_of_key, hashmap->capacity);

  struct Node *node_it = &hashmap->nodeList[index];

  if (node_it->is_empty) {
    node_it->is_empty = false;
    node_it->key = key;
    node_it->value = value;
  }

  while ((node_it = node_it->next) != NULL) {
  }

  node_it = (struct Node *)malloc(sizeof(struct Node));
  node_it->next = NULL;
  node_it->is_empty = false;
  node_it->key = key;
  node_it->value = value;
}
