#ifndef NETWORK_H
#define NETWORK_H

#include "structures.h"

int connect_to (char *, char *);
list_t *network_connect_with_handlers (int *, int *);
list_t *network_learn_workers (int *);
int network_connect_from_balancer (int);
int network_wait_for_ready_fd (int, list_t *);
void network_broadcast (list_t *, packet_t *, FILE *);
void network_send_packet (int, packet_t *);
void network_recv_packet (int, packet_t *);

#endif // NETWORK_H
