#include "libraries.h"
#include "definitions.h"
#include "structures.h"

#include "packet.h"

#include "network.h"

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
    rv = setsockopt (listen_fd, SOL_SOCKET, SO_REUSEADDR, &yes, sizeof (int));
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

int network_wait_for_ready_fd (int listen_fd)
{
  fd_set read_fds;

  FD_ZERO (&read_fds);

  FD_SET (STDIN_FILENO, &read_fds);

  FD_SET (listen_fd, &read_fds);

  int max_fd = listen_fd > STDIN_FILENO ? listen_fd : STDIN_FILENO;

  int rv = select (max_fd + 1, &read_fds, NULL, NULL, NULL);

  if (rv == -1)
  {
    fprintf (stderr, "Error: select ()\n");
    exit (EXIT_FAILURE);
  }

  if (FD_ISSET (STDIN_FILENO, &read_fds))
  {
    return STDIN_FILENO;
  }

  return listen_fd;
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
