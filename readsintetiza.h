#ifndef READ_SINTETIZADOR
#define READ_SINTETIZADOR

#include <stdbool.h>
#include <stdio.h>
#include "fmodulacion.h"

bool sintet_read_armonicos(FILE *f, float armonicos[][2], size_t *n);

bool sintet_read_funcion(FILE *f, mod_t *funcion, float params[]);

#endif
