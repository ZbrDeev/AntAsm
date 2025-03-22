#include "hashmap.h"
#include <stdio.h>
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
  hashmap.nodeList = (struct Node **)malloc(capacity * sizeof(struct Node *));

  for (size_t i = 0; i < capacity; ++i) {
    hashmap.nodeList[i] = NULL;
  }

  return hashmap;
}

struct NodeValue navigateIntoNode(struct Node *node, const char *key) {
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

  struct Node *node = hashmap->nodeList[index];

  while (node != NULL) {
    if (strcmp(key, node->key) == 0) {
      return node->value;
    }

    node = node->next;
  }

  struct NodeValue node_null_value = {.node_value_type = Null,
                                      .node_value = NULL};
  return node_null_value;
}

void addKeyValue(struct HashMap *hashmap, const char *key, void *value_ptr,
                 const enum NodeValueType value_type) {
  struct Node *new_node = (struct Node *)malloc(sizeof(struct Node));
  new_node->key = key;
  new_node->value.node_value = value_ptr;
  new_node->value.node_value_type = value_type;
  new_node->next = NULL;

  int sum_of_key = stringToSum(key);
  int index = CALC_KEY_INDEX(sum_of_key, hashmap->capacity);

  if (hashmap->nodeList[index] == NULL) {
    hashmap->nodeList[index] = new_node;
  } else {

    new_node->next = hashmap->nodeList[index];
    hashmap->nodeList[index] = new_node;
  }
}

void freeHashMap(struct HashMap *hashmap) {
  for (size_t i = 0; i < hashmap->capacity; ++i) {
    if (hashmap->nodeList[i] != NULL) {
      freeNode(hashmap->nodeList[i]);
    }
  }

  free(hashmap->nodeList);
}

void freeNode(struct Node *node) {
  struct Node *tmp;

  while (node != NULL) {
    tmp = node;
    node = node->next;
    free(tmp);
  }
}
