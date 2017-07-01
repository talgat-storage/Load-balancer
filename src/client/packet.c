#include "libraries.h"
#include "definitions.h"
#include "structures.h"

#include "packet.h"

void packet_build (packet_t *packet, uint32_t client_id, uint32_t transaction_id, input_t *input)
{
  packet->client_id = client_id;

  packet->transaction_id = transaction_id;

  packet->cmd = input->first_input;

  packet->code = CODE_NONE;

  packet->key_length = strlen (input->second_string);

  packet->value_length = strlen (input->third_string);

  strcpy (packet->key, input->second_string);

  strcpy (packet->value, input->third_string);
}

void packet_host_to_network (packet_t *packet)
{
  packet->client_id = htonl (packet->client_id);

  packet->transaction_id = htonl (packet->transaction_id);

  packet->cmd = htons (packet->cmd);

  packet->code = htons (packet->code);

  packet->key_length = htons (packet->key_length);

  packet->value_length = htons (packet->value_length);
}

void packet_network_to_host (packet_t *packet)
{
  packet->client_id = ntohl (packet->client_id);

  packet->transaction_id = ntohl (packet->transaction_id);

  packet->cmd = ntohs (packet->cmd);

  packet->code = ntohs (packet->code);

  packet->key_length = ntohs (packet->key_length);

  packet->value_length = ntohs (packet->value_length);
}

void packet_parse_header (packet_t *packet, uint8_t *header)
{
  packet->client_id = (*( (uint32_t *) (header) ));

  packet->transaction_id = (*( (uint32_t *) (header + FOUR_BYTES) ));

  packet->cmd = (*( (uint16_t *) (header + 2 * FOUR_BYTES) ));

  packet->code = (*( (uint16_t *) (header + 2 * FOUR_BYTES + TWO_BYTES) ));

  packet->key_length = (*( (uint16_t *) (header + 3 * FOUR_BYTES) ));

  packet->value_length = (*( (uint16_t *) (header + 3 * FOUR_BYTES + TWO_BYTES) ));
}
