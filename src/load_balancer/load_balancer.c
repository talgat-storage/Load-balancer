#include "libraries.h"
#include "definitions.h"
#include "structures.h"

#include "list.h"
#include "info.h"
#include "network.h"
#include "packet.h"
#include "input.h"
#include "output.h"
#include "logging.h"

int main (int argc, char *argv[])
{
  list_t *handlers = list_create ();

  info_handlers (handlers);
  network_connect_to_handlers (handlers);
  node_t *next_handler = handlers->head->next;

  int listen_fd = network_listen_to (PORT);

  bool need_to_print_prompt = true;

  list_t *clients = list_create ();

  logging_create ();

  while (true)
  {
    if (need_to_print_prompt == true)
    {
      output_print_prompt ();
      need_to_print_prompt = false;
    }

    int ready_fd = network_wait_for_ready_fd (listen_fd, clients);

    pthread_attr_t attr;
    pthread_attr_init (&attr);
    pthread_attr_setdetachstate (&attr, PTHREAD_CREATE_DETACHED);
    pthread_t thread;

    if (ready_fd == STDIN_FILENO)
    {
      input_list (handlers);

      need_to_print_prompt = true;
    }

    else if (ready_fd == listen_fd) /* New client */
    {
      int client_fd = accept (listen_fd, NULL, NULL);

      node_t *new_client = node_create (NULL, NULL, client_fd);

      new_client->busy = true;

      list_insert (clients, new_client);

      node_t *handler = next_handler;

      if (next_handler->next == NULL)
      {
        next_handler = handlers->head->next;
      }
      else
      {
        next_handler = next_handler->next;
      }

      network_wait_handler (handler);

      handler->client = new_client;

      pthread_create (&thread, &attr, &network_thread, (void *) handler);
    }

    else /* Old client */
    {
      node_t *handler = next_handler;

      if (next_handler->next == NULL)
      {
        next_handler = handlers->head->next;
      }
      else
      {
        next_handler = next_handler->next;
      }

      network_wait_handler (handler);

      node_t *client = list_find (clients, ready_fd);

      handler->client = client;

      pthread_create (&thread, &attr, &network_thread, (void *) handler);
    }

    pthread_attr_destroy (&attr);
  }

  return 0;
}
