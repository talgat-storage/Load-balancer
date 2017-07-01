#include "libraries.h"
#include "definitions.h"
#include "structures.h"

#include "list.h"

#include "info.h"

void info_handlers (list_t *handlers)
{
  FILE *file = fopen (INFO_FILENAME, "r");

  if (file == NULL)
  {
    fprintf (stderr, "Error: could not open info file of handlers\n");
    exit (EXIT_FAILURE);
  }

  char line[INFO_LINE_LENGTH];

  while (fgets (line, INFO_LINE_LENGTH, file) != NULL)
  {
    char *ip = strtok (line, INFO_DELIMITERS);

    if (ip == NULL)
      break;

    char *port = strtok (NULL, INFO_DELIMITERS);

    if (port == NULL)
      break;

    node_t *node = node_create (ip, port, 0);

    list_insert (handlers, node);
  }

  if (list_is_empty (handlers))
  {
    fprintf (stderr, "Error: file does not have any information\n");
    exit (EXIT_FAILURE);
  }

  fclose (file);
}
