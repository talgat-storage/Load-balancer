#ifndef LOGGING_H
#define LOGGING_H

#include "structures.h"

FILE *logging_create (char *, int);
void logging_packet (FILE *, packet_t *);

#endif // LOGGING_H
