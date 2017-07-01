#include "libraries.h"
#include "definitions.h"
#include "structures.h"

#include "output.h"

void output_print_prompt (void)
{
  printf ("Load balancer> ");

  fflush (stdout);
}

void output_print_handlers (list_t *handlers)
{
  printf ("Handlers:\n");

  int i = 1;
  for (node_t *cur_node = handlers->head->next; cur_node != NULL; cur_node = cur_node->next, i++)
  {
    printf ("%d / %s / %s / requests: %d\n",
            i, cur_node->ip, cur_node->port, cur_node->number_of_requests);
  }

  fflush (stdout);
}
