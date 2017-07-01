#include "libraries.h"
#include "definitions.h"
#include "structures.h"

#include "network.h"
#include "hash.h"
#include "logging.h"
#include "output.h"
#include "input.h"
#include "packet.h"

int main ()
{
  int listen_fd;
  int handler_id;

  list_t *handlers = network_connect_with_handlers (&listen_fd, &handler_id);

  int number_of_workers;

  list_t *workers = network_learn_workers (&number_of_workers);

  int balancer_fd = network_connect_from_balancer (listen_fd);

  hash_table_t *hash_table = hash_table_create (number_of_workers);

  FILE *logging_file = logging_create (LOGGING_FILENAME, handler_id);

  bool need_to_print_prompt = true;

  while (true)
  {
    if (need_to_print_prompt == true)
    {
      output_print_prompt (handler_id);

      need_to_print_prompt = false;
    }

    int ready_fd = network_wait_for_ready_fd (balancer_fd, handlers);

    if (ready_fd == STDIN_FILENO)
    {
      input_t input;

      input_parse (&input);

      if (input.valid == false)
      {
        fprintf (stderr, "Error: invalid input\n");

        need_to_print_prompt = true;

        continue;
      }

      if (input.cmd == LIST_CMD)
      {
        output_show_hash_table (hash_table, number_of_workers);
      }

      else /* show */
      {
        output_show_worker_hash (hash_table, number_of_workers, input.worker_id);
      }

      need_to_print_prompt = true;
    }

    else if (ready_fd == balancer_fd) /* Balancer */
    {
      packet_t packet_request;

      network_recv_packet (balancer_fd, &packet_request);

      int worker_id = hash_calculate_worker_id (&packet_request, number_of_workers);

      node_t *worker_node =  list_get_node (workers, worker_id);

      worker_node->socket_fd = connect_to (worker_node->ip, worker_node->port);

      network_send_packet (worker_node->socket_fd, &packet_request);

      logging_packet (logging_file, &packet_request);

      packet_t packet_reply;

      network_recv_packet (worker_node->socket_fd, &packet_reply);

      close (worker_node->socket_fd);

      network_send_packet (balancer_fd, &packet_reply);

      logging_packet (logging_file, &packet_reply);

      if (packet_reply.code == CODE_SUCCESS)
      {
        hash_table_update (hash_table, workers, number_of_workers, &packet_request);

        network_broadcast (handlers, &packet_request, logging_file);
      }
    }

    else /* Handler */
    {
      packet_t packet_broadcast;

      network_recv_packet (ready_fd, &packet_broadcast);

      logging_packet (logging_file, &packet_broadcast);

      hash_table_update (hash_table, workers, number_of_workers, &packet_broadcast);
    }
  }
}
