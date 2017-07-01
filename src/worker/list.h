#ifndef LIST_H
#define LIST_H

#include "structures.h"

list_t *list_create (void);
void list_insert (list_t *, packet_t *);
void list_remove (list_t *, node_t *);
node_t *list_get_node (list_t *, char *);

#endif // LIST_H
