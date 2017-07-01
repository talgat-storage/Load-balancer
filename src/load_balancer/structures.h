#ifndef STRUCTURES_H
#define STRUCTURES_H

#include "definitions.h"

typedef struct node_tag {
  pthread_mutex_t lock;

  int socket_fd;

  bool busy;

  char ip[MAX_KEY_LENGTH]; /* Only for handlers */
  char port[MAX_KEY_LENGTH]; /* Only for handlers */
  int number_of_requests; /* Only for handlers */
  struct node_tag *client; /* Only for handlers */

  struct node_tag *next;
} node_t;

typedef struct list_tag {
  node_t *head;
} list_t;

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
