#include "libraries.h"
#include "definitions.h"
#include "structures.h"

#include "logging.h"
#include "packet.h"

#include "network.h"

static int connect_to (char *ip_address, char *port)
{
  struct addrinfo hints, *servinfo, *p;
  int rv;
  int socket_fd;

  memset ((void *) &hints, 0, sizeof (struct addrinfo));
  hints.ai_family = AF_UNSPEC;
  hints.ai_socktype = SOCK_STREAM;

  rv = getaddrinfo (ip_address, port, &hints, &servinfo);

  if (rv != 0)
  {
    fprintf (stderr, "Error: getaddrinfo () %s\n", gai_strerror (rv));
    exit (EXIT_FAILURE);
  }

  for (p = servinfo; p != NULL; p = p->ai_next)
  {
    socket_fd = socket (p->ai_family, p->ai_socktype, p->ai_protocol);

    if (socket_fd == -1)
    {
      continue;
    }

    rv = connect (socket_fd, p->ai_addr, p->ai_addrlen);

    if (rv == -1)
    {
      close (socket_fd);
      continue;
    }

    break;
  }

  freeaddrinfo (servinfo);

  if (p == NULL)
  {
    fprintf (stderr, "Error: Could not connect to a handler\n");
    exit (EXIT_FAILURE);
  }

  return socket_fd;
}

void network_connect_to_handlers (list_t *handlers)
{
  for (node_t *cur_node = handlers->head->next; cur_node != NULL; cur_node = cur_node->next)
  {
    cur_node->socket_fd = connect_to (cur_node->ip, cur_node->port);
  }
}

int network_listen_to (char *port)
{
  int listen_fd;

  struct addrinfo hints, *servinfo, *p;
  int rv;

  memset ((void *) &hints, 0, sizeof (struct addrinfo));
  hints.ai_family = AF_UNSPEC;
  hints.ai_socktype = SOCK_STREAM;
  hints.ai_flags = AI_PASSIVE;

  rv = getaddrinfo (NULL, port, &hints, &servinfo);

  if (rv != 0)
  {
    fprintf (stderr, "Error: getaddrinfo () %s\n", gai_strerror (rv));
    exit (EXIT_FAILURE);
  }

  for (p = servinfo; p != NULL; p = p->ai_next)
  {
    listen_fd = socket (p->ai_family, p->ai_socktype, p->ai_protocol);
    if (listen_fd == -1)
    {
      continue;
    }

    int yes = 1;
    rv = setsockopt (listen_fd,SOL_SOCKET, SO_REUSEADDR, &yes, sizeof (int));
    if (rv == -1)
    {
      fprintf (stderr, "Error: setsockopt ()\n");
      exit (EXIT_FAILURE);
    }

    rv = bind (listen_fd, p->ai_addr, p->ai_addrlen);
    if (rv == -1)
    {
      close (listen_fd);
      continue;
    }

    break;
  }

  freeaddrinfo (servinfo);

  if (p == NULL)
  {
    fprintf (stderr, "Error: failed to bind\n");
    exit (EXIT_FAILURE);
  }

  rv = listen (listen_fd, BACKLOG);

  if (rv == -1)
  {
    fprintf (stderr, "Error: listen ()\n");
    exit (EXIT_FAILURE);
  }

  return listen_fd;
}

int network_wait_for_ready_fd (int listen_fd, list_t *clients)
{
  int socket_fd;

  while (true) /* Repeats the loop only if one of the busy clients sends something */
  {
    fd_set read_fds;

    FD_ZERO (&read_fds);

    FD_SET (STDIN_FILENO, &read_fds);

    FD_SET (listen_fd, &read_fds);

    int max_fd = STDIN_FILENO > listen_fd ? STDIN_FILENO : listen_fd;

    for (node_t *cur_node = clients->head->next; cur_node != NULL; cur_node = cur_node->next)
    {
      FD_SET (cur_node->socket_fd, &read_fds);

      max_fd = max_fd > cur_node->socket_fd ? max_fd : cur_node->socket_fd;
    }

    int rv = select (max_fd + 1, &read_fds, NULL, NULL, NULL);

    if (rv == -1)
    {
      fprintf (stderr, "Error: select ()\n");
      exit (EXIT_FAILURE);
    }

    if (FD_ISSET (STDIN_FILENO, &read_fds))
    {
      socket_fd = STDIN_FILENO;
      break;
    }

    if (FD_ISSET (listen_fd, &read_fds))
    {
      socket_fd = listen_fd;
      break;
    }

    node_t *cur_node;
    for (cur_node = clients->head->next; cur_node != NULL; cur_node = cur_node->next)
    {
      if (!FD_ISSET (cur_node->socket_fd, &read_fds))
        continue;

      pthread_mutex_lock (&cur_node->lock);

      if (cur_node->busy == true)
      {
        pthread_mutex_unlock (&cur_node->lock);

        continue;
      }

      cur_node->busy = true;

      pthread_mutex_unlock (&cur_node->lock);

      socket_fd = cur_node->socket_fd;

      break;
    }

    if (cur_node == NULL)
      continue;

    break;
  }

  return socket_fd;
}

void network_wait_handler (node_t *handler)
{
  while (true) /* Repeats while loop only if the handler is busy */
  {
    pthread_mutex_lock (&handler->lock);

    if (handler->busy == false)
    {
      break;
    }

    pthread_mutex_unlock (&handler->lock);
  }

  handler->busy = true;

  pthread_mutex_unlock (&handler->lock);
}

static void make_node_free (node_t *node)
{
  pthread_mutex_lock (&node->lock);

  node->busy = false;

  pthread_mutex_unlock (&node->lock);
}

void *network_thread (void *arg)
{
  node_t *handler = (node_t *) arg;

  handler->number_of_requests += 1;

  packet_t packet_request;

  network_recv_packet (handler->client->socket_fd, &packet_request);

  network_send_packet (handler->socket_fd, &packet_request);

  logging_packet (&packet_request);

  packet_t packet_reply;

  network_recv_packet (handler->socket_fd, &packet_reply);

  network_send_packet (handler->client->socket_fd, &packet_reply);

  logging_packet (&packet_reply);

  make_node_free (handler);

  make_node_free (handler->client);
}

static void send_message (int socket_fd, ssize_t message_size, void *message)
{
  ssize_t bytes_left = message_size;
  ssize_t just_sent;
  ssize_t bytes_sent = 0;

  while (bytes_left > 0)
  {
    just_sent = write (socket_fd, message + bytes_sent, bytes_left);

    if (just_sent == -1)
    {
      fprintf (stderr, "Error: write ()\n");
      fflush (stderr);
      continue;
    }

    bytes_left -= just_sent;
    bytes_sent += just_sent;
  }
}

void network_send_packet (int socket_fd, packet_t *packet)
{
  packet_host_to_network (packet);

  send_message (socket_fd, FOUR_BYTES, &packet->client_id);

  send_message (socket_fd, FOUR_BYTES, &packet->transaction_id);

  send_message (socket_fd, TWO_BYTES, &packet->cmd);

  send_message (socket_fd, TWO_BYTES, &packet->code);

  send_message (socket_fd, TWO_BYTES, &packet->key_length);

  send_message (socket_fd, TWO_BYTES, &packet->value_length);

  send_message (socket_fd, ntohs (packet->key_length), packet->key);

  send_message (socket_fd, ntohs (packet->value_length), packet->value);

  packet_network_to_host (packet);
}

static void recv_message (int socket_fd, ssize_t message_size, void *message)
{
  ssize_t bytes_left = message_size;
  ssize_t just_read;
  ssize_t bytes_read = 0;

  while (bytes_left > 0)
  {
    just_read = read (socket_fd, message + bytes_read, bytes_left);

    if (just_read == -1)
    {
      fprintf (stderr, "Error: read ()\n");
      fflush (stderr);
      continue;
    }

    bytes_left -= just_read;
    bytes_read += just_read;
  }
}

void network_recv_packet (int socket_fd, packet_t *packet)
{
  uint8_t header[HEADER_SIZE];

  recv_message (socket_fd, HEADER_SIZE, (void *) header);

  packet_parse_header (packet, header);

  packet_network_to_host (packet);

  char key[MAX_KEY_LENGTH];

  recv_message (socket_fd, packet->key_length, (void *) key);

  strncpy (packet->key, key, packet->key_length);

  packet->key[packet->key_length] = '\0';

  char value[MAX_VALUE_LENGTH];

  recv_message (socket_fd, packet->value_length, (void *) value);

  strncpy (packet->value, value, packet->value_length);

  packet->value[packet->value_length] = '\0';
}

