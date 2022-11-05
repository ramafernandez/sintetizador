#ifndef TDA_SINTETIZA
#define TDA_SINTETIZA

#include <stdbool.h>
#include "readsintetiza.h"
#include "tramo.h"
#include "notas.h"

typedef struct modulacion modulacion_t;
typedef struct sintet sintet_t;

sintet_t *sintet_config(FILE *f);

bool sintet_modular(tramo_t *t, sintet_t *sintet, float t0, float d, float f_m, float f, float a);

bool sintet_notas(notas_t *notas, tramo_t **tt, sintet_t *sintet, int f_m,
              int pps);

void sintet_destruir(sintet_t *sintet);

#endif
