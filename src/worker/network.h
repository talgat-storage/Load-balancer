#ifndef NETWORK_H
#define NETWORK_H

#include "structures.h"

int network_listen_to (char *);
int network_wait_for_ready_fd (int);
void network_send_packet (int, packet_t *);
void network_recv_packet (int, packet_t *);

#endif // NETWORK_H
