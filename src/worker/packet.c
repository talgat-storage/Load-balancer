#include "libraries.h"
#include "definitions.h"
#include "structures.h"

#include "packet.h"

void packet_parse_header (packet_t *packet, uint8_t *header)
{
  packet->client_id = (*( (uint32_t *) (header) ));

  packet->transaction_id = (*( (uint32_t *) (header + FOUR_BYTES) ));

  packet->cmd = (*( (uint16_t *) (header + 2 * FOUR_BYTES) ));

  packet->code = (*( (uint16_t *) (header + 2 * FOUR_BYTES + TWO_BYTES) ));

  packet->key_length = (*( (uint16_t *) (header + 3 * FOUR_BYTES) ));

  packet->value_length = (*( (uint16_t *) (header + 3 * FOUR_BYTES + TWO_BYTES) ));
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

void packet_build_short (packet_t *packet_reply, packet_t *packet_request, uint16_t cmd, uint16_t code)
{
  packet_reply->client_id = packet_request->client_id;

  packet_reply->transaction_id = packet_request->transaction_id;

  packet_reply->cmd = cmd;

  packet_reply->code = code;

  packet_reply->key_length = 0;

  packet_reply->value_length = 0;

  packet_reply->key[0] = '\0';

  packet_reply->value[0] = '\0';
}

void packet_build_get (packet_t *packet_reply, packet_t *packet_request, node_t *node)
{
  packet_reply->client_id = packet_request->client_id;

  packet_reply->transaction_id = packet_request->transaction_id;

  packet_reply->cmd = GET_ACK_CMD;

  if (node == NULL)
  {
    packet_reply->code = CODE_NOT_EXIST;

    packet_reply->key_length = packet_request->key_length;

    packet_reply->value_length = 0;

    strcpy (packet_reply->key, packet_request->key);

    packet_reply->value[0] = '\0';
  }

  else
  {
    packet_reply->code = CODE_SUCCESS;

    packet_reply->key_length = strlen (node->key);

    packet_reply->value_length = strlen (node->value);

    strcpy (packet_reply->key, node->key);

    strcpy (packet_reply->value, node->value);
  }
}
