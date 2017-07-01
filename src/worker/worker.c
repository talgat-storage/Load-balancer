#include "libraries.h"
#include "definitions.h"
#include "structures.h"

#include "network.h"
#include "list.h"
#include "logging.h"
#include "output.h"
#include "input.h"
#include "packet.h"

void check_argc (int);

int main (int argc, char *argv[])
{
  check_argc (argc);

  int listen_fd = network_listen_to (argv[1]);

  list_t *list = list_create ();

  bool need_to_print_prompt = true;

  int worker_id = atoi (argv[2]);

  FILE *logging_file = logging_create (LOGGING_FILENAME, worker_id);

  while (true)
  {
    if (need_to_print_prompt == true)
    {
      output_print_prompt (worker_id);

      need_to_print_prompt = false;
    }

    int ready_fd = network_wait_for_ready_fd (listen_fd);

    if (ready_fd == STDIN_FILENO)
    {
      input_t input;

      input_parse (&input);

      if (input.valid = false)
      {
        fprintf (stderr, "Error: invalid input\n");
      }

      else if (input.cmd == LIST_CMD)
      {
        output_print_list (list);
      }

      else /* show */
      {
        node_t *node = list_get_node (list, input.key);

        if (node == NULL)
        {
          output_no_key ();
        }

        else
        {
          output_print_node (node);
        }
      }

      need_to_print_prompt = true;
    }

    else
    {
      int client_fd = accept (listen_fd, NULL, NULL);

      packet_t packet_request;

      network_recv_packet (client_fd, &packet_request);

      logging_packet (logging_file, &packet_request);

      node_t *node = list_get_node (list, packet_request.key);

      packet_t packet_reply;

      switch (packet_request.cmd)
      {
        case PUT_CMD:

          if (node == NULL)
          {
            list_insert (list, &packet_request);

            packet_build_short (&packet_reply, &packet_request, PUT_ACK_CMD, CODE_SUCCESS);
          }
          else
          {
            packet_build_short (&packet_reply, &packet_request, PUT_ACK_CMD, CODE_EXIST);
          }

          break;

        case GET_CMD:

          packet_build_get (&packet_reply, &packet_request, node);

          break;

        case DEL_CMD:

          if (node == NULL)
          {
            packet_build_short (&packet_reply, &packet_request, DEL_ACK_CMD, CODE_NOT_EXIST);
          }
          else
          {
            list_remove (list, node);

            packet_build_short (&packet_reply, &packet_request, DEL_ACK_CMD, CODE_SUCCESS);
          }

          break;

        default:

          abort ();
      }

      network_send_packet (client_fd, &packet_reply);

      logging_packet (logging_file, &packet_reply);
    }
  }

  return 0;
}

void check_argc (int argc)
{
  if (argc != 3)
  {
    fprintf (stderr, "Usage: ./<program> <port> <ID>\n");
    exit (EXIT_FAILURE);
  }
}
