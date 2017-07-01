#ifndef LIST_H
#define LIST_H

node_t *node_create (char *, char *, int);
list_t *list_create (void);
void list_insert (list_t *, node_t *);
void list_remove (list_t *, int);
node_t *list_find (list_t *, int);
bool list_is_empty (list_t *);

#endif // LIST_H
