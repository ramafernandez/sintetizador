#ifndef NOTAS_H
#define NOTAS_H

#include <stdint.h>
#include <stdbool.h>
#include <stdlib.h>

struct notas;
typedef struct notas notas_t;

notas_t *notas_crear();

bool notas_agregar(notas_t *notas, uint8_t *mensaje, uint32_t ti);

bool notas_config(notas_t *notas, uint32_t tf, uint8_t valor);

bool notas_get(notas_t *notas, size_t posicion, int pulsos_seg,
                  float *f, float *a, float *t0, float *d);

size_t notas_get_cant(notas_t *notas);

void notas_destruir(notas_t *notas);

void print_notas(notas_t *notas);

#endif
