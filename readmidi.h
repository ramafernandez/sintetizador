#ifndef READMIDI_H
#define READMIDI_H

#include <stdio.h>
#include <stdbool.h>
#include "notas.h"

bool read_midi(FILE *f, char canal_leer, notas_t *notas);

#endif
