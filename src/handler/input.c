#include "libraries.h"
#include "definitions.h"
#include "structures.h"

#include "input.h"

void input_parse (input_t *input)
{
  input->valid = false;

  char input_string[INPUT_LENGTH];
  fgets (input_string, INPUT_LENGTH, stdin);
  if (input_string == NULL)
    return;

  char *first_string = strtok (input_string, INPUT_DELIMITERS);
  if (first_string == NULL)
    return;

  if (strcmp (first_string, LIST_STR) == 0)
  {
    input->cmd = LIST_CMD;
  }

  else if (strcmp (first_string, SHOW_STR) == 0)
  {
    input->cmd = SHOW_CMD;

    char *second_string = strtok (NULL, INPUT_DELIMITERS);

    if (second_string == NULL)
      return;

    input->worker_id = atoi (second_string);

    if (input->worker_id == 0)
    {
      return;
    }
  }

  else
  {
    return;
  }

  input->valid = true;
}
