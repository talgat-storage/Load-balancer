#ifndef STRUCTURES_H
#define STRUCTURES_H

#include "definitions.h"

typedef struct node_tag {
  /* Used for network nodes */
  int socket_fd;
  char ip[MAX_KEY_LENGTH];
  char port[MAX_KEY_LENGTH];
  /* ---------------------- */

  /* Used for hash table nodes */
  uint32_t hash_value;
  /* ------------------------- */

  struct node_tag *next;
} node_t;

typedef struct list_tag {
  node_t *head;
} list_t;

typedef struct hash_table_tag {
  list_t *lists;
} hash_table_t;

typedef struct input_tag {
  bool valid;

  int cmd;
  int worker_id;
} input_t;

typedef struct packet_tag {
  uint32_t client_id;
  uint32_t transaction_id;
  uint16_t cmd;
  uint16_t code;
  uint16_t key_length;
  uint16_t value_length;
  char key[MAX_KEY_LENGTH];
  char value[MAX_VALUE_LENGTH];
} packet_t;

#endif // STRUCTURES_H
