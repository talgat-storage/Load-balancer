#include "libraries.h"
#include "definitions.h"
#include "structures.h"

#include "hash.h"
#include "logging.h"
#include "packet.h"

#include "network.h"

static int get_my_number (char *filename)
{
  int control_file_fd = open (filename, O_RDWR);

  if (flock (control_file_fd, LOCK_EX) == -1)
  {
    fprintf (stderr, "Error: flock ()\n");
    exit (EXIT_FAILURE);
  }

  FILE *control_file = fdopen (control_file_fd, "r+");

  if (control_file == NULL)
  {
    fprintf (stderr, "Error: fopen ()\n");
    exit (EXIT_FAILURE);
  }

  char line[INFO_LINE_LENGTH];

  if (fgets (line, INFO_LINE_LENGTH, control_file) == NULL)
  {
    fprintf (stderr, "Error: first line of the control file is invalid\n");
    exit (EXIT_FAILURE);
  }

  int my_info_line = atoi (line);

  rewind (control_file);

  fprintf (control_file, "%d\n", my_info_line + 1);

  fflush (control_file);

  if (flock (control_file_fd, LOCK_UN) == -1)
  {
    fprintf (stderr, "Error: flock ()\n");
    exit (EXIT_FAILURE);
  }

  close (control_file_fd);

  return my_info_line;
}

static int listen_to (char *port)
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

int connect_to (char *ip_address, char *port)
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

  if (p == NULL)
  {
    fprintf (stderr, "Error: network.c network_connect_to ()\n");
    exit (EXIT_FAILURE);
  }

  freeaddrinfo (servinfo);

  return socket_fd;
}

static void connect_to_handlers (list_t *handlers_all, node_t *my_node)
{
  node_t *cur_node;

  for (cur_node = handlers_all->head->next;
       cur_node != NULL && cur_node != my_node;
       cur_node = cur_node->next)
  {
    cur_node->socket_fd = connect_to (cur_node->ip, cur_node->port);
  }

  if (cur_node != my_node)
  {
    fprintf (stderr, "Error: could not connect to all handlers\n");
    exit (EXIT_FAILURE);
  }
}

static void connect_from_handlers (list_t *handlers_all, node_t *my_node, int listen_fd)
{
  node_t *cur_node;

  for (cur_node = handlers_all->head->next;
       cur_node != NULL;
       cur_node = cur_node->next)
  {
    if (cur_node == my_node)
      break;
  }

  if (cur_node != my_node)
  {
    fprintf (stderr, "Error: could not connect from all handlers\n");
    exit (EXIT_FAILURE);
  }

  for (cur_node = cur_node->next; cur_node != NULL; cur_node = cur_node->next)
  {
    cur_node->socket_fd = accept (listen_fd, NULL, NULL);
  }
}

list_t *network_connect_with_handlers (int *listen_fd, int *handler_id)
{
  list_t *handlers_all = list_create ();

  list_read_file (handlers_all, INFO_HANDLERS);

  int my_number = get_my_number (INFO_CONTROL);

  *handler_id = my_number + 1;

  node_t *my_node = list_get_node (handlers_all, my_number);

  if (my_node == NULL)
  {
    fprintf (stderr, "Error: could not figure out my info\n");
    exit (EXIT_FAILURE);
  }

  *listen_fd = listen_to (my_node->port);

  connect_to_handlers (handlers_all, my_node);

  connect_from_handlers (handlers_all, my_node, *listen_fd);

  list_remove (handlers_all, my_node);

  node_destroy (my_node);

  return handlers_all;
}

list_t *network_learn_workers (int *number_of_workers)
{
  list_t *workers = list_create ();

  list_read_file (workers, INFO_WORKERS);

  *number_of_workers = list_count_nodes (workers);

  if (*number_of_workers == 0)
  {
    fprintf (stderr, "Error: no workers\n");
    exit (EXIT_FAILURE);
  }

  return workers;
}

int network_connect_from_balancer (int listen_fd)
{
  int balancer_fd = accept (listen_fd, NULL, NULL);

  return balancer_fd;
}

int network_wait_for_ready_fd (int balancer_fd, list_t *handlers)
{
  fd_set read_fds;

  FD_ZERO (&read_fds);

  FD_SET (STDIN_FILENO, &read_fds);

  FD_SET (balancer_fd, &read_fds);

  int max_fd = balancer_fd > STDIN_FILENO ? balancer_fd : STDIN_FILENO;

  for (node_t *cur_node = handlers->head->next;
       cur_node != NULL; cur_node = cur_node->next)
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
    return STDIN_FILENO;
  }

  if (FD_ISSET (balancer_fd, &read_fds))
  {
    return balancer_fd;
  }

  node_t *cur_node;
  for (cur_node = handlers->head->next; cur_node != NULL; cur_node = cur_node->next)
  {
    if (FD_ISSET (cur_node->socket_fd, &read_fds))
      break;
  }

  return cur_node->socket_fd;
}

void network_broadcast (list_t *handlers, packet_t *packet, FILE *logging_file)
{
  if (packet->cmd != PUT_CMD && packet->cmd != DEL_CMD)
    return;

  for (node_t *cur_node = handlers->head->next; cur_node != NULL; cur_node = cur_node->next)
  {
    network_send_packet (cur_node->socket_fd, packet);

    logging_packet (logging_file, packet);
  }
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
