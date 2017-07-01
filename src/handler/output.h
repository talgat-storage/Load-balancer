#ifndef OUTPUT_H
#define OUTPUT_H

#include "structures.h"

void output_print_prompt (int);
void output_show_hash_table (hash_table_t *, int);
void output_show_worker_hash (hash_table_t *, int, int);
void output_list (list_t *list);

#endif // OUTPUT_H
