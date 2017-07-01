#ifndef NETWORK_H
#define NETWORK_H

#include "structures.h"

void network_connect_to_handlers (list_t *);
int network_listen_to (char *);

int network_wait_for_ready_fd (int, list_t *);

void network_wait_handler (node_t *);

void *network_thread (void *);

void network_send_packet (int, packet_t *);
void network_recv_packet (int, packet_t *);

#endif // NETWORK_H
