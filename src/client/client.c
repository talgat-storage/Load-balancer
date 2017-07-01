#include "libraries.h"
#include "definitions.h"
#include "structures.h"

#include "output.h"
#include "input.h"
#include "logging.h"
#include "network.h"
#include "packet.h"

int main ()
{
  uint32_t client_id = -1;

  balancer_t balancer;
  balancer.connected = false;

  uint32_t transaction_id;

  FILE *logging_file = NULL;

  while (true)
  {
    output_display_message (client_id);

    input_t input;

    input_parse (&input);

    if (input.valid == false)
    {
      fprintf (stderr, "Error: invalid value\n");
      fflush (stderr);
      continue;
    }

    if (input.first_input == CON_CMD)
    {
      strcpy (balancer.ip, input.second_string);

      if (balancer.connected == true)
      {
        close (balancer.fd);
      }

      balancer.fd = newtork_connect_to (balancer.ip, BALANCER_PORT);

      balancer.connected = true;

      transaction_id = 0;

      continue;
    }

    if (balancer.connected == false)
    {
      fprintf (stderr, "Error: not connected to balancer\n");
      fflush (stderr);

      continue;
    }

    if (input.first_input == SET_CMD)
    {
      client_id = (uint32_t) atoi (input.second_string);

      if (logging_file != NULL)
      {
        fclose (logging_file);
      }

      logging_file = logging_create (client_id);

      continue;
    }

    if (client_id == -1)
    {
      output_client_id_not_set (client_id);

      continue;
    }

    transaction_id++;

    packet_t packet_request;

    packet_build (&packet_request, client_id, transaction_id, &input);

    network_send_packet (balancer.fd, &packet_request);

    logging_packet (logging_file, &packet_request);

    packet_t packet_reply;

    network_recv_packet (balancer.fd, &packet_reply);

    logging_packet (logging_file, &packet_reply);

    output_show_result (&packet_reply);
  }

  return 0;
}
