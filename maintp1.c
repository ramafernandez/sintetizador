#include <stdio.h>
#include <stdlib.h>
#include <stdint.h>
#include <stdlib.h>
#include <string.h>
#include <stdbool.h>

#include "notas.h"
#include "readmidi.h"
#include "tdasintetiza.h"
#include "tramo.h"
#include "escribirwave.h"

#define MAX_CADENA 100

enum{
    SINTENTIZADOR,
    ENTRADA,
    SALIDA,
    CANAL,
    FRECUENCIA,
    PULSO
};

const char* flags[] = {
    [SINTENTIZADOR] = "-s",
    [ENTRADA] = "-i",
    [SALIDA] = "-o",
    [CANAL] = "-c",
    [FRECUENCIA] = "-f",
    [PULSO] = "-r"

};

bool buscar_flags(char *v[] , int n , int *canal , int *frecuencia, int *pulsos , char *sintet , char *entrada , char *salida){
    char *aux = NULL;
    for(size_t i = 0 ; i < n ; i++){
        if(!strcmp(flags[SINTENTIZADOR], v[i])){
            strcpy(sintet , v[i+1]);
        }
        if(!strcmp(flags[ENTRADA], v[i])){
            strcpy(entrada , v[i+1]);
        }
        if(!strcmp(flags[SALIDA] , v[i])){
            strcpy(salida, v[i+1]);
        }
        if(!strcmp(flags[CANAL] , v[i])){
            *canal = strtol(v[i+1] , &aux , 10);
            if(*aux != '\0'){
                fprintf(stderr, "el valor ingresado luego de %s no es enteramente númerico" , v[i]);
                return false;
            }
        }
        if(!strcmp(flags[FRECUENCIA] , v[i])){
            *frecuencia = strtol(v[i+1] , &aux , 10);
            if(*aux != '\0'){
                fprintf(stderr, "el valor ingresado luego de %s no es enteramente númerico" , v[i]);
                return false;
            }
        }
        if(!strcmp(flags[PULSO] , v[i])){
            *pulsos = strtol(v[i+1] , &aux , 10);
            if(*aux != '\0'){
                fprintf(stderr, "el valor ingresado luego de %s no es enteramente númerico" , v[i]);
                return false;
            }
            return true;
        }
    }
}


int main(int argc, char *argv[]){
    if(!strcmp(argv[1] , "--help")){
        printf("El programa convierte un archivo .mid en un archivo .wav, por eso necesita que se ingrese de la manera:\n\t%s -s <archivo.txt> -i <archivo.mid> -o <salida.wav>.\n\t Opcionalmente, se pueden ingresar:\n\t[-c <canal>],\n\t[-f <frecuencia>],\n\t[-r <pulsosporsegundo>],\n\tcosas que serán predeterminadas en caso de omisión\n" , argv[0]);
        return 0;
    }
    
    if(argc < 7){
        fprintf(stderr, "Número insuficiente de parámetros.\nUso:\n\t %s -s <archivo.txt> -i <archivo.mid> -o <salida.wav>\nopcionalmente, se puede incluir:\n[-c <canal>],\n[-f <frecuencia>],\n[-r <pulsosporsegundo>],\ncosas que serán predeterminadas en caso de omisión\n", argv[0]);
        return 1;
    }

    int pulsos = 450;
    int frecuencia = 44100;
    int canal = 0;
    char sintetizador[MAX_CADENA];
    char entrada[MAX_CADENA];
    char salida[MAX_CADENA];

    buscar_flags(argv, argc, &canal, &frecuencia, &pulsos, sintetizador, entrada, salida);

    FILE *f = fopen(entrada, "rb");
  if(f == NULL){
    fprintf(stderr, "Fallo en lectura del archivo .mid\n");
    return 1;
  }

  notas_t *notas = notas_crear();
  if(!read_midi( f , 0, notas)){
    fprintf(stderr, "Fallo en lectura del archivo .mid\n");
    return 1;
  }
  fclose(f);

  if(!(f = fopen(sintetizador, "rt"))){
    fprintf(stderr, "fallo en la lectura del archivo .txt\n");
    return 1;
  }

  sintet_t *sintet = sintet_config(f);
  if(sintet == NULL){
    fprintf(stderr, "Fallo en la configuración del sintetizadr\n");
    return 1;
  }
  fclose(f);


  tramo_t *t;
  if(!sintet_notas(notas, &t, sintet, frecuencia, pulsos)){
    fprintf(stderr, "Fallo al sintetizar las notas\n");
    return 1;
  }

  float *v = tramo_get_v(t);
  size_t n = tramo_get_n(t);

  escribir_wave(salida, v, n, frecuencia);

  notas_destruir(notas);
  tramo_destruir(t);
  sintet_destruir(sintet);

    return 0;
}