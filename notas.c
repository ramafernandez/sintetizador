#include <stdlib.h>
#include "nota.h"
#include "notas.h"
#include "decodificarmidi.h"

struct notas {
  nota_t **v; //vector de punteros a notas
  size_t n; //cantidad de notas
};
/*La idea es tener un vector de notas e ir guardando cada nota para tener x ejemplo
su ti y cuando sepamos que se termina, agarrar ese tiempo y restarle ti*/
notas_t *notas_crear(){
  notas_t *notas = malloc(sizeof(notas_t));
  if(notas == NULL)
    return NULL;

  notas->v = NULL;

  notas->n = 0;

  return notas;
}

/*Esta la llamamos cada vez que venga una nota nueva o sea un evento nota encendida
con v != 0*/
bool notas_agregar(notas_t *notas, uint8_t *mensaje, uint32_t ti){
  nota_t **aux = realloc( notas->v, (notas->n + 1)*sizeof(nota_t *));
  if(aux == NULL){
    return false;
    }

  if( !(aux[notas->n] = nota_crear( mensaje, ti)) ){
    notas_destruir(notas);
    return false;
  }

  notas->v = aux;
  notas->v[notas->n] = aux[notas->n];
  notas->n++;

  return true;
}

/*Esta la usamos para cuando venga un nota apagada o nota encendida con v = 0
saber que nota es,(compara la nota y la octava) para hacer nota_set_duracion
"valor" seria el buffer[EVNOTA_NOTA] del main de santisi*/
bool notas_config(notas_t *notas, uint32_t tf, uint8_t valor){
  numnota_t nota;
  signed char octava;
  if( !decodificar_nota( valor, &nota , &octava))
    return false;

  for(size_t i = 0; i < notas->n; i++){
    if(nota_cmp( notas->v[i], nota, octava)){
      nota_set_duracion( notas->v[i], tf);
      return true;
    }
  }

  notas_destruir(notas);
  return false;
}


/*La cosa es que llamemos esta funcion en el main (o puede que dentro de otra
fucnion de la parte de tramos) para sacar pasarle a
las funciones que hacen el tramo la frecuencia intesidad y los tiempos
de cada nota*/

bool notas_get(notas_t *notas, size_t posicion, int pulsos_seg,
                  float *f, float *a, float *t0, float *d){
  if(posicion > notas->n)
    return false;

  *f = nota_get_frecuencia(notas->v[posicion]);
  *a = nota_get_intensidad(notas->v[posicion]);
  *t0 = nota_get_ti(notas->v[posicion], pulsos_seg);
  *d = nota_get_duracion(notas->v[posicion], pulsos_seg);

  return true;
}

size_t notas_get_cant(notas_t *notas){
  return notas->n;
}


void notas_destruir(notas_t *notas){
  for(size_t i = 0; i < notas->n; i++){
    free(notas->v[i]);
  }
  free(notas->v);
  free(notas);
}

void print_notas(notas_t *notas){
  for(size_t i = 0; i < notas->n; i++)
    print_nota(notas->v[i]);
}
