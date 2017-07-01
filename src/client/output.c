#include "libraries.h"
#include "definitions.h"
#include "structures.h"

#include "output.h"

void output_display_message (uint32_t client_id)
{
  printf ("Client");

  if (client_id != -1)
  {
    printf (" #%lu", (unsigned long) client_id);
  }

  printf ("> ");

  fflush (stdout);
}

void output_client_id_not_set (int client_id)
{
  fprintf (stderr, "Client ID is not set\n");
  fflush (stderr);
}

void output_show_result (packet_t *packet)
{
  if (packet->cmd == PUT_ACK_CMD)
  {
    if (packet->code == CODE_SUCCESS)
    {
      printf ("Success\n");
    }

    else
    {
      printf ("Fail\n");
    }
  }

  else if (packet->cmd == GET_ACK_CMD)
  {
    if (packet->code == CODE_SUCCESS)
    {
      printf ("%s\n", packet->value);
    }

    else
    {
      printf ("Fail\n");
    }
  }

  else if (packet->cmd == DEL_ACK_CMD)
  {
    if (packet->code == CODE_SUCCESS)
    {
      printf ("Success\n");
    }

    else
    {
      printf ("Fail\n");
    }
  }

  else
  {
    fprintf (stderr, "Error: packet command is invalid\n");
    fflush (stderr);
    return;
  }

  fflush (stdout);
}
