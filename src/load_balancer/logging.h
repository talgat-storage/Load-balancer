#ifndef LOGGING_H
#define LOGGING_H

#include "structures.h"

pthread_mutex_t logging_file_lock;
FILE *logging_file;

void logging_create (void);
void logging_packet (packet_t *);

#endif // LOGGING_H
