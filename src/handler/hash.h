#ifndef HASH_H
#define HASH_H

#include "structures.h"

node_t *node_create (char *, char *, uint32_t);
list_t *list_create (void);
void list_read_file (list_t *, char *);
node_t *list_get_node (list_t *, int);
void list_remove (list_t *, node_t *);
void node_destroy (node_t *);
int list_count_nodes (list_t *);
hash_table_t *hash_table_create (int);
int hash_calculate_worker_id (packet_t *, int);
void hash_table_update (hash_table_t *, list_t *, int, packet_t *);

#endif // HASH_H
