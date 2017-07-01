#include "libraries.h"
#include "definitions.h"
#include "structures.h"

#include "output.h"

#include "input.h"

void input_list (list_t *handlers)
{
  char input_string[INPUT_LENGTH];

  fgets (input_string, INPUT_LENGTH, stdin);

  char *ptr = strstr (input_string, LIST_STR);

  if (ptr == NULL)
  {
    fprintf (stderr, "Error: invalid input\n");
    fflush (stderr);
    return;
  }

  output_print_handlers (handlers);
}
