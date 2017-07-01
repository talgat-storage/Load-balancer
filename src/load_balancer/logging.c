#include "libraries.h"
#include "definitions.h"
#include "structures.h"

#include "logging.h"

void logging_create (void)
{
  pthread_mutex_init (&logging_file_lock, NULL);

  char full_filename[MAX_FILENAME_LENGTH];

  sprintf (full_filename, "%s.log", LOGGING_FILENAME);

  logging_file = fopen (full_filename, "w+");

  if (logging_file == NULL)
  {
    fprintf (stderr, "Error: could not create a logging file\n");
    exit (EXIT_FAILURE);
  }
}

void logging_packet (packet_t *packet)
{
  pthread_mutex_lock (&logging_file_lock);

  time_t time_in_sec = time (NULL);
  struct tm cur_time = *localtime (&time_in_sec);

  fprintf (logging_file, "%d:%d:%d ",
           cur_time.tm_hour, cur_time.tm_min, cur_time.tm_sec);

           (int) packet->cmd, (int) packet->code,

  fprintf (logging_file, "%lu %lu ",
           (unsigned long) packet->client_id, (unsigned long) packet->transaction_id);

  switch (packet->cmd)
  {
    case PUT_CMD:
      fprintf (logging_file, "%s ", PUT_STR);
      break;

    case PUT_ACK_CMD:
      fprintf (logging_file, "%s ", PUT_ACK_STR);
      break;

    case GET_CMD:
      fprintf (logging_file, "%s ", GET_STR);
      break;

    case GET_ACK_CMD:
      fprintf (logging_file, "%s ", GET_ACK_STR);
      break;

    case DEL_CMD:
      fprintf (logging_file, "%s ", DEL_STR);
      break;

    case DEL_ACK_CMD:
      fprintf (logging_file, "%s ", DEL_ACK_STR);
      break;

    default:
      abort ();
  }

  switch (packet->code)
  {
    case CODE_NONE:
      fprintf (logging_file, "%s ", CODE_NONE_STR);
      break;

    case CODE_SUCCESS:
      fprintf (logging_file, "%s ", CODE_SUCCESS_STR);
      break;

    case CODE_EXIST:
      fprintf (logging_file, "%s ", CODE_EXIST_STR);
      break;

    case CODE_NOT_EXIST:
      fprintf (logging_file, "%s ", CODE_NOT_EXIST_STR);
      break;

    default:
      abort ();
  }

  fprintf (logging_file, "%d %d %s %s\n",
           (int) packet->key_length, (int) packet->value_length,
           packet->key, packet->value);

  fflush (logging_file);

  pthread_mutex_unlock (&logging_file_lock);
}
