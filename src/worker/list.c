#include "libraries.h"
#include "definitions.h"
#include "structures.h"

#include "list.h"

static uint32_t jenkins_hash (uint8_t *key, size_t length)
{
  size_t i = 0;
  uint32_t hash = 0;

  while (i != length)
  {
    hash += key[i++];
    hash += hash << 10;
    hash ^= hash >> 6;
  }
  hash += hash << 3;
  hash ^= hash >> 11;
  hash += hash << 15;

  return hash;
}

static node_t *node_create (char *key, char *value)
{
  node_t *node = (node_t *) malloc (sizeof (node_t));

  if (key == NULL)
    node->key[0] = '\0';
  else
  {
    strcpy (node->key, key);

    node->hash_value = jenkins_hash ((uint8_t *) key, strlen (key));
  }

  if (value == NULL)
    node->value[0] = '\0';
  else
    strcpy (node->value, value);

  node->next = NULL;

  return node;
}

list_t *list_create (void)
{
  list_t *list = (list_t *) malloc (sizeof (list_t));

  list->head = node_create (NULL, NULL);

  return list;
}

void list_insert (list_t *list, packet_t *packet)
{
  node_t *new_node = node_create (packet->key, packet->value);

  node_t *cur_node;

  for (cur_node = list->head; cur_node->next != NULL; cur_node = cur_node->next)
  ;

  cur_node->next = new_node;
}

void list_remove (list_t *list, node_t *node)
{
  node_t *prev_node = list->head;
  node_t *cur_node = list->head->next;

  while (cur_node != NULL)
  {
    if (cur_node == node)
      break;

    prev_node = cur_node;
    cur_node = cur_node->next;
  }

  prev_node->next = cur_node->next;

  free (cur_node);
}

node_t *list_get_node (list_t *list, char *key)
{
  uint32_t hash_value = jenkins_hash ((uint8_t *) key, strlen (key));

  node_t *cur_node;

  for (cur_node = list->head->next; cur_node != NULL; cur_node = cur_node->next)
  {
    if (cur_node->hash_value == hash_value)
    {
      break;
    }
  }

  return cur_node;
}
