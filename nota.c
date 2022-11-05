#include <math.h>
#include <stdlib.h>
#include "nota.h"

/*Tuve que añadir "octava" al TDA xq no me quedaba otra sino no tenia como
usar nota_cmp*/
struct nota {
  numnota_t nota;
  uint32_t in, d;
  uint8_t intensidad;
  signed char octava;
};

nota_t *nota_crear(uint8_t *mensaje, uint32_t ti){
  nota_t *n = malloc(sizeof(nota_t));
  if(n == NULL)
    return NULL;

  if(!decodificar_nota( mensaje[0], &(n->nota), &(n->octava) )){
    free(n);
    return NULL;
  }
  n->in = ti;
  n->intensidad = mensaje[1];
  n->d = 0;

  return n;
}

void nota_set_duracion(nota_t *n, uint32_t tf){
  n->d = tf - n->in ;
}

double nota_get_ti(nota_t *n, int pulsos_seg){
  return n->in / (double)pulsos_seg;
}

double nota_get_duracion(nota_t *n, int pulsos_seg){
  return n->d / (double)pulsos_seg;
}

float nota_get_intensidad(nota_t *n){
  return n->intensidad;
}

float nota_get_frecuencia(nota_t *n){
  int aux = (n->nota - 9);
  return 440.0*pow( 2.0, n->octava - 4 + (aux/12.0));;
}

bool nota_cmp(nota_t *n, numnota_t nota, signed char octava){
  return n->nota == nota && n->octava == octava && n->d == 0;
}

void print_nota(nota_t *n){
  printf("nota: %d\n", n->d);
}
