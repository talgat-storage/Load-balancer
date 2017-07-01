#include "libraries.h"
#include "definitions.h"
#include "structures.h"

#include "list.h"

node_t *node_create (char *ip, char *port, int socket_fd)
{
  node_t *node = (node_t *) malloc (sizeof (node_t));

  pthread_mutex_init (&node->lock, NULL);

  node->socket_fd = socket_fd;

  node->busy = false;

  if (ip == NULL)
    node->ip[0] = '\0';
  else
    strcpy (node->ip, ip);

  if (port == NULL)
    node->port[0] = '\0';
  else
    strcpy (node->port, port);

  node->number_of_requests = 0;

  node->client = NULL;

  node->next = NULL;

  return node;
}

list_t *list_create (void)
{
  list_t *list = (list_t *) malloc (sizeof (list_t));

  list->head = node_create (NULL, NULL, 0);

  return list;
}

void list_insert (list_t *list, node_t *node)
{
  node_t *cur_node;

  for (cur_node = list->head; cur_node->next != NULL; cur_node = cur_node->next)
  ;

  cur_node->next = node;
}

bool list_is_empty (list_t *list)
{
  if (list->head->next == NULL)
    return true;

  return false;
}

node_t *list_find (list_t *list, int socket_fd)
{
  node_t *cur_node;

  for (cur_node = list->head->next; cur_node != NULL; cur_node = cur_node->next)
  {
    if (cur_node->socket_fd == socket_fd)
      break;
  }

  return cur_node;
}

void list_remove (list_t *list, int socket_fd)
{
  node_t *prev_node = list->head;
  node_t *cur_node = list->head->next;

  for (; cur_node != NULL; prev_node = cur_node, cur_node = cur_node->next)
  {
    if (cur_node->socket_fd == socket_fd)
      break;
  }

  if (cur_node == NULL)
    return;

  prev_node->next = cur_node->next;

  free ((void *) cur_node);
}
