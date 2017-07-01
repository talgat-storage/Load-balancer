#ifndef NETWORK_H
#define NETWORK_H

#include "structures.h"

int newtork_connect_to (char *, char *);
void network_send_packet (int, packet_t *);
void network_recv_packet (int, packet_t *);

#endif // NETWORK_H
