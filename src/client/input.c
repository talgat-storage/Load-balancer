#include "libraries.h"
#include "definitions.h"
#include "structures.h"

#include "input.h"

void input_parse (input_t *input)
{
  char input_string[INPUT_LENGTH];

  char *first_string = NULL;
  char *second_string = NULL;
  char *third_string = NULL;

  input->first_input = 0;
  input->second_string[0] = '\0';
  input->third_string[0] = '\0';
  input->valid = false;

  fgets (input_string, INPUT_LENGTH, stdin);

  first_string = strtok (input_string, INPUT_DELIMITERS);

  if (first_string != NULL)
    second_string = strtok (NULL, INPUT_DELIMITERS);

  if (first_string == NULL ||
      second_string == NULL || strlen (second_string) >= MAX_KEY_LENGTH)
  {
    return;
  }

  third_string = strtok (NULL, INPUT_DELIMITERS);

  if (strcmp (first_string, INPUT_SET_STR) == 0)
  {
    input->first_input = SET_CMD;
  }

  else if (strcmp (first_string, INPUT_CON_STR) == 0)
  {
    input->first_input = CON_CMD;
  }

  else if (strcmp (first_string, INPUT_PUT_STR) == 0)
  {
    input->first_input = PUT_CMD;

    if (third_string == NULL || strlen (third_string) >= MAX_VALUE_LENGTH)
    {
      return;
    }

    strcpy (input->third_string, third_string);
  }

  else if (strcmp (first_string, INPUT_GET_STR) == 0)
  {
    input->first_input = GET_CMD;
  }

  else if (strcmp (first_string, INPUT_DEL_STR) == 0)
  {
    input->first_input = DEL_CMD;
  }

  else
  {
    return;
  }

  strcpy (input->second_string, second_string);

  input->valid = true;
}
