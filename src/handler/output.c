#include "libraries.h"
#include "definitions.h"
#include "structures.h"

#include "output.h"

void output_print_prompt (int handler_id)
{
  printf ("Handler #%d> ", handler_id);

  fflush (stdout);
}

static void show_list (list_t *list)
{
  node_t *cur_node;

  for (cur_node = list->head->next; cur_node != NULL; cur_node = cur_node->next)
  {
    printf ("%lu ", (unsigned long) cur_node->hash_value);
  }

  printf ("\n");

  fflush (stdout);
}

void output_show_hash_table (hash_table_t *hash_table, int number_of_workers)
{
  for (int i = 0; i < number_of_workers; i++)
  {
    printf ("Worker %d / ", i + 1);

    show_list (&hash_table->lists[i]);
  }
}

void output_show_worker_hash (hash_table_t *hash_table, int number_of_workers, int worker_id)
{
  if (worker_id > number_of_workers)
  {
    fprintf (stderr, "Error: invalid input\n");
    return;
  }

  show_list (&hash_table->lists[worker_id - 1]);
}

void output_list (list_t *list)
{
  int i = 0;
  for (node_t *cur_node = list->head->next; cur_node != NULL; cur_node = cur_node->next)
  {
    printf ("%d / %s / %s / socket: %d\n", i + 1, cur_node->ip, cur_node->port, cur_node->socket_fd);
    i++;
  }

  fflush (stdout);
}
