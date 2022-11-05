#include <stdio.h>
#include <stdlib.h>
#include "tdasintetiza.h"
#define L_PARAMS 3
#define MAX_FUNCION 3
#define CANT_FUNC 14

//Esto es lo que creo que seria sintetizador TDA
struct modulacion {
  mod_t funcion;
  float params[L_PARAMS];
};

struct sintet {
  float armonics[20][2];
  size_t n_armonics;
  modulacion_t mod[MAX_FUNCION];
};

const pfmod_t fun_mod[CANT_FUNC] = {
    [CONSTANT] = constant,
    [LINEAR] = linear,
    [INVLINEAR] = invlinear,
    [SIN] = seno,
    [EXP] = exponente,
    [INVEXP] = invexp,
    [QUARTCOS] = quartcos,
    [QUARTSIN] = quartsin,
    [HALFCOS] = halfcos,
    [HALFSIN] = halfsin,
    [LOG] = logarithm,
    [INVLOG] = invlog,
    [TRI] = tri,
    [PULSES] = pulses
};

sintet_t *sintet_config(FILE *f){
  sintet_t *sintet = malloc(sizeof(sintet_t));
  if(sintet == NULL)
    return NULL;

  if(!sintet_read_armonicos( f, sintet->armonics, &(sintet->n_armonics))){
    free(sintet);
    return NULL;
  }

  for(size_t i = 0; i < MAX_FUNCION ; i++){
    if(!sintet_read_funcion( f, &(sintet->mod[i].funcion), sintet->mod[i].params)){
      free(sintet);
      return NULL;
    }
  }

  if(getc(f) != EOF){
    free(sintet);
    return NULL;
  }

  return sintet;
}

bool sintet_modular(tramo_t *t, sintet_t *sintet, float t0, float d, float f_m, float f, float a){
  //Ataque
  if(!tramo_multiplicar( t, t0, t0+sintet->mod[0].params[0], sintet->mod[0].params, fun_mod[sintet->mod[0].funcion]))
    return false;
  //Sostenido
  if(!tramo_multiplicar( t, t0+sintet->mod[0].params[0], t0+d, sintet->mod[1].params, fun_mod[sintet->mod[1].funcion]))
    return false;
  //Decaimiento
  tramo_t *t_d = tramo_crear_muestreo(t0+d, t0+d+sintet->mod[2].params[0], f_m, f, a, sintet->armonics, sintet->n_armonics);
  if(t_d == NULL)
    return false;

  if(!tramo_extender( t, t_d))
    return false;
  tramo_destruir(t_d);
  return true;
}

///SINTETIZAR LAS NOTAS...
bool sintet_notas(notas_t *notas, tramo_t **tt, sintet_t *sintet, int f_m,
              int pps){
  float f, a, t0, d;
  if(!notas_get( notas, 0, pps, &f, &a, &t0, &d))
    return false;

  //Muestreo la nota base
  *tt = tramo_crear_muestreo( t0, t0+d, f_m, f, a, sintet->armonics, sintet->n_armonics);
  if(*tt == NULL)
    return false;

  //Modulo la nota base
  if(!sintet_modular(*tt, sintet, t0, d, f_m, f, a))
    return false;
  size_t n_notas = notas_get_cant(notas);
  //Itero muestreando todas las notas y modulandolas
  for(size_t i = 1; i < n_notas; i++){
    if(!notas_get( notas, i, pps, &f, &a, &t0, &d))
      return false;

    //printf("%.2f:%.2f\n",f, a );
    //el td esta en la funcion 3, 1er parámetoro
    tramo_t *t = tramo_crear_muestreo( t0, t0+d, f_m, f, a, sintet->armonics, sintet->n_armonics);
    if( t == NULL)
      return false;

   if(!sintet_modular( t, sintet, t0, d, f_m, f, a))
      return false;

  if(!tramo_extender( *tt, t))
      return false;

  tramo_destruir(t);
  }

  return true;
}

void sintet_destruir(sintet_t *sintet){
  free(sintet);
}
