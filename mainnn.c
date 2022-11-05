#include "notas.h"
#include "readmidi.h"
#include "tdasintetiza.h"
#include "tramo.h"
#include "writewave.h"

int main(){
  FILE *f = fopen("greensleeves.mid","rb");
  if(f == NULL){
    printf("FAIL OPEN MIDI\n");
    return 1;
  }

  notas_t *notas = notas_crear();
  if(!read_midi( f , 0, notas)){
    printf("FAIL MIDI\n");
    return 1;
  }
  fclose(f);

  if(!(f = fopen("sintetizador.txt", "rt"))){
    printf("FAIL SINTET TXT\n");
    return 1;
  }

  sintet_t *sintet = sintet_config(f);
  if(sintet == NULL){
    printf("FAIL SINTET CONFIG\n");
    return 1;
  }
  fclose(f);


  tramo_t *t;
  if(!sintet_notas(notas, &t, sintet, 44100, 400)){
    printf("FAIL SINTET NOTAS\n");
    return 1;
  }

 //imprimir_muestras(t);

  float *v = tramo_get_v(t);
  size_t n = tramo_get_n(t);

  escribir_wave("probando.wav", v, n, 44100);

  notas_destruir(notas);
  tramo_destruir(t);
  sintet_destruir(sintet);

  return 0;
}
