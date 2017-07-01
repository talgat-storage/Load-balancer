#include "libraries.h"
#include "definitions.h"
#include "structures.h"

#include "hash.h"

node_t *node_create (char *ip, char *port, uint32_t hash_value)
{
  node_t *node = (node_t *) malloc (sizeof (node_t));

  node->socket_fd = 0;

  if (ip == NULL)
    node->ip[0] = '\0';
  else
    strcpy (node->ip, ip);

  if (port == NULL)
    node->port[0] = '\0';
  else
    strcpy (node->port, port);

  node->hash_value = hash_value;

  node->next = NULL;

  return node;
}

list_t *list_create (void)
{
  list_t *list = (list_t *) malloc (sizeof (list_t));

  list->head = node_create (NULL, NULL, 0);

  return list;
}

static bool list_is_empty (list_t *list)
{
  if (list->head->next == NULL)
    return true;

  return false;
}

static void list_insert (list_t *list, node_t *node)
{
  node_t *cur_node;

  for (cur_node = list->head; cur_node->next != NULL; cur_node = cur_node->next)
  ;

  cur_node->next = node;
}

void list_read_file (list_t *list, char *filename)
{
  if (!list_is_empty (list))
  {
    fprintf (stderr, "Error: list is not empty\n");
    return;
  }

  FILE *file = fopen (filename, "r");

  if (file == NULL)
  {
    fprintf (stderr, "Error: could not open a file\n");
    exit (EXIT_FAILURE);
  }

  char line[INFO_LINE_LENGTH];

  while (fgets (line, INFO_LINE_LENGTH, file) != NULL)
  {
    char *ip = strtok (line, INFO_DELIMITERS);

    if (ip == NULL)
      break;

    char *port = strtok (NULL, INFO_DELIMITERS);

    if (port == NULL)
      break;

    node_t *node = node_create (ip, port, 0);

    list_insert (list, node);
  }

  if (list_is_empty (list))
  {
    fprintf (stderr, "Error: list is empty\n");
    exit (EXIT_FAILURE);
  }
}

node_t *list_get_node (list_t *list, int id)
{
  node_t *cur_node;

  int i = 0;
  for (cur_node = list->head->next; cur_node != NULL; cur_node = cur_node->next)
  {
    if (i == id)
      break;

    i++;
  }

  return cur_node;
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

//  if (cur_node == NULL)
//    prev_node->next = NULL;
//
//  else
    prev_node->next = cur_node->next;
}

void node_destroy (node_t *node)
{
  free ((void *) node);
}

int list_count_nodes (list_t *workers)
{
  int i = 0;

  node_t *cur_node;

  for (cur_node = workers->head->next; cur_node != NULL;
       cur_node = cur_node->next)
  {
    i++;
  }

  return i;
}

hash_table_t *hash_table_create (int number_of_workers)
{
  hash_table_t *hash_table = (hash_table_t *) malloc (sizeof (hash_table_t));

  hash_table->lists = (list_t *) malloc (number_of_workers * sizeof (list_t));

  for (int i = 0; i < number_of_workers; i++)
  {
    hash_table->lists[i].head = node_create (NULL, NULL, 0);
  }

  return hash_table;
}

//void list_destroy (list_t *list)
//{
//  node_t *cur_node;
//  node_t *next_node;
//
//  for (cur_node = list->head->next; cur_node != NULL; cur_node = next_node)
//  {
//    next_node = cur_node->next;
//
//    node_destroy (cur_node);
//  }
//
//  node_destroy (list->head);
//
//  free ((void *) list);
//}

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

int hash_calculate_worker_id (packet_t *packet, int number_of_workers)
{
  uint32_t hash_value = jenkins_hash ((uint8_t *) packet->key,
                                      (size_t) packet->key_length);

  return (int) (hash_value % (uint32_t) number_of_workers);
}

static node_t *list_find_node (list_t *list, uint32_t hash_value)
{
  node_t *cur_node;

  for (cur_node = list->head->next; cur_node != NULL; cur_node = cur_node->next)
  {
    if (cur_node->hash_value == hash_value)
      break;
  }

  return cur_node;
}

void hash_table_update (hash_table_t *hash_table, list_t *workers,
                        int number_of_workers, packet_t *packet)
{
  uint32_t hash_value = jenkins_hash ((uint8_t *) packet->key, (size_t) packet->key_length);

  int worker_id = (int) (hash_value % (uint32_t) number_of_workers);

  if (packet->cmd == PUT_CMD)
  {
    node_t *node = node_create (NULL, NULL, hash_value);

    list_insert (&hash_table->lists[worker_id], node);
  }

  else if (packet->cmd == DEL_CMD)
  {
    node_t *node = list_find_node (&hash_table->lists[worker_id], hash_value);

    list_remove (&hash_table->lists[worker_id], node);
  }

  else if (packet->cmd == GET_CMD)
  {
    return;
  }

  else
  {
    abort ();
  }
}
