#ifndef NOTA_H
#define NOTA_H

#include <stdbool.h>
#include <stdint.h>
#include "decodificarmidi.h"

struct nota;
typedef struct nota nota_t;

nota_t *nota_crear(uint8_t *mensaje, uint32_t ti);

void nota_set_duracion(nota_t *n, uint32_t tf);

double nota_get_ti(nota_t *n, int pulsos_seg);

double nota_get_duracion(nota_t *n, int pulsos_seg);

float nota_get_intensidad(nota_t *n);

float nota_get_frecuencia(nota_t *n);

bool nota_cmp(nota_t *n, numnota_t nota, signed char octava);

void print_nota(nota_t *n);

#endif
