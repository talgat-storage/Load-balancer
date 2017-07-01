#ifndef PACKET_H
#define PACKET_H

#include "structures.h"

void packet_parse_header (packet_t *, uint8_t *);
void packet_host_to_network (packet_t *);
void packet_network_to_host (packet_t *);
void packet_build_short (packet_t *, packet_t *, uint16_t, uint16_t);
void packet_build_get (packet_t *, packet_t *, node_t *);

#endif // PACKET_H
