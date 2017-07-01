#ifndef PACKET_H
#define PACKET_H

#include "structures.h"

void packet_build (packet_t *, uint32_t, uint32_t, input_t *);
void packet_parse_header (packet_t *, uint8_t *);
void packet_host_to_network (packet_t *);
void packet_network_to_host (packet_t *);

#endif // PACKET_H
