#ifndef STRUCTURES_H
#define STRUCTURES_H

#include "definitions.h"

typedef struct balancer_tag {
  int fd;
  char ip[MAX_KEY_LENGTH];
  bool connected;
} balancer_t;

typedef struct input_tag {
  uint16_t first_input;
  char second_string[MAX_KEY_LENGTH];
  char third_string[MAX_VALUE_LENGTH];
  bool valid;
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
