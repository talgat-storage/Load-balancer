#include "libraries.h"
#include "definitions.h"
#include "structures.h"

#include "output.h"

void output_print_prompt (int worker_id)
{
  printf ("Worker #%d> ", worker_id);

  fflush (stdout);
}

//void print_packet (packet_t *packet)
//{
//  printf ("Packet contents:\n");
//
//  printf ("Client id: %lu\n", (unsigned long) packet->client_id);
//
//  printf ("Transaction id: %lu\n", (unsigned long) packet->transaction_id);
//
//  printf ("Cmd: %lu\n", (unsigned long) packet->cmd);
//
//  printf ("Code: %lu\n", (unsigned long) packet->code);
//
//  printf ("Key length: %lu\n", (unsigned long) packet->key_length);
//
//  printf ("Value length: %lu\n", (unsigned long) packet->value_length);
//
//  printf ("Key: %s\n", packet->key);
//
//  printf ("Value: %s\n", packet->value);
//
//  fflush (stdout);
//}

void output_print_node (node_t *node)
{
  printf ("%lu / %s / %s\n", (unsigned long) node->hash_value, node->key, node->value);

  fflush (stdout);
}

void output_print_list (list_t *list)
{
  node_t *node;

  for (node = list->head->next; node != NULL; node = node->next)
  {
    output_print_node (node);
  }

  fflush (stdout);
}

void output_no_key (void)
{
  printf ("No such key\n");

  fflush (stdout);
}
