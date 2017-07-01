#include "libraries.h"
#include "definitions.h"
#include "structures.h"

#include "input.h"

void input_parse (input_t *input)
{
  input->valid = false;

  char input_string[INPUT_LENGTH];

  char *first_string = NULL;
  char *second_string = NULL;

  fgets (input_string, INPUT_LENGTH, stdin);

  first_string = strtok (input_string, INPUT_DELIMITERS);

  if (first_string == NULL)
  {
    return;
  }

  if (strcmp (first_string, LIST_STR) == 0)
  {
    input->cmd = LIST_CMD;
  }

  else if (strcmp (first_string, SHOW_STR) == 0)
  {
    input->cmd = SHOW_CMD;

    second_string = strtok (NULL, INPUT_DELIMITERS);

    if (second_string == NULL)
    {
      return;
    }

    strcpy (input->key, second_string);
  }

  else
  {
    return;
  }

  input->valid = true;
}
