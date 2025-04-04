#ifndef HASHMAP_H
#define HASHMAP_H

#include <stdbool.h>
#include <stdint.h>

#define CALC_KEY_INDEX(key, capacity) (key % capacity)

int stringToSum(const char *string);

enum NodeValueType {
  // Numerical value
  NodeValue_Int64,
  NodeValue_Int32,
  NodeValue_Int16,
  NodeValue_Int8,

  // For register like AL for Low and AH for High
  NodeValue_Int8_Low,
  NodeValue_Int8_High,

  NodeValue_String,
  NodeValue_Null,
};

struct NodeValue {
  enum NodeValueType node_value_type;
  void *node_value;
};

struct Node {
  const char *key;
  struct NodeValue value;

  struct Node *next;
};

struct HashMap {
  unsigned int capacity;
  struct Node **nodeList;
};

struct HashMap initHashMap(unsigned int capacity);

struct NodeValue navigateIntoNode(struct Node *node, const char *key);

struct NodeValue getValue(struct HashMap *hashmap, const char *key);

void addKeyValue(struct HashMap *hashmap, const char *key, void *value_ptr,
                 const enum NodeValueType value_type);

void freeHashMap(struct HashMap *hashmap);

void freeNode(struct Node *node);

#endif // HASHMAP_H
