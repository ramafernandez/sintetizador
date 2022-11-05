#ifndef TRAMO_H
#define TRAMO_H

#include <stdbool.h>
#include <stdlib.h>

struct tramo;
typedef struct tramo tramo_t;

tramo_t *tramo_crear_muestreo(double t0, double tf, int f_m, float f,
	                          float a, float fa[][2], size_t n_fa);

bool tramo_redimensionar(tramo_t *t, double tf);

bool tramo_extender(tramo_t *destino, const tramo_t *extension);

bool tramo_modular(tramo_t *t, size_t desde, size_t hasta, float valor);

bool tramo_multiplicar(tramo_t *t, float ti, float tf, float *v, float f( double , float *));

float *tramo_get_v(tramo_t *t);

size_t tramo_get_n(tramo_t *t);

void tramo_destruir(tramo_t *t);

void imprimir_muestras(const tramo_t *t);

#endif
