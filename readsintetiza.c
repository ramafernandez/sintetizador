#include <string.h>
#include <stdlib.h>
#include <stdbool.h>
#include <stdio.h>

#include "readsintetiza.h"

#define MAX 50
#define CANT_FUNC 14

const int params_fun[CANT_FUNC] = {
  [CONSTANT] = 0,
  [LINEAR] = 1,
  [INVLINEAR] = 1,
  [SIN] = 2,
  [EXP] = 1,
  [INVEXP] = 1,
  [QUARTCOS] = 1,
  [QUARTSIN] = 1,
  [HALFCOS] = 1,
  [HALFSIN] = 1,
  [LOG] = 1,
  [INVLOG] = 1,
  [TRI] = 3,
  [PULSES] = 3
};

const char *fnames[CANT_FUNC] = {
  [CONSTANT] = "CONSTANT",
  [LINEAR] = "LINEAR",
  [INVLINEAR] = "INVLINEAR",
  [SIN] = "SIN",
  [EXP] = "EXP",
  [INVEXP] = "INVEXP",
  [QUARTCOS] = "QUARTCOS",
  [QUARTSIN] = "QUARTSIN",
  [HALFCOS] = "HALFCOS",
  [HALFSIN] = "HALFSIN",
  [LOG] = "LOG",
  [INVLOG] = "INVLOG",
  [TRI] = "TRI",
  [PULSES] = "PULSES"
};

bool sintet_read_armonicos(FILE *f, float armonicos[][2], size_t *n){
  char *end;
  char buffer[MAX];

  fgets( buffer, MAX, f);
  *n = strtol( buffer, &end, 10);
  if(*end != '\n')
    return false;

  for(size_t i = 0; i < *n; i++){
    if(!fgets(buffer, MAX, f)){
      return false;
    }

    armonicos[i][0] = strtof( buffer, &end);
    if(*end != ' '){
      return false;
    }

    armonicos[i][1] = strtof( end , &end);
    if(*end != '\n'){
      return false;
    }
  }

  return true;
 }

 static bool search_cant_params(char *s, int *n_params, mod_t *funcion){
   for(size_t i = 0; i < CANT_FUNC; i++){
     if(!strcmp( s, fnames[i])){
       *n_params = params_fun[i];
       *funcion = i;
       return true;
     }
   }
   return false;
 }
 bool sintet_read_funcion(FILE *f, mod_t *funcion, float params[]){
   char buffer[MAX];
   char s[MAX];
   fgets( buffer, MAX, f);
   size_t i;
   for( i = 0; buffer[i] != ' ' && buffer[i] != '\n'; i++)
      s[i] = buffer[i];
   s[i] = '\0'; //Me guardo el nombre de la funcion con el \0

   int n_params;
   if(!search_cant_params( s, &n_params, funcion)) //busco su cantidad de params
    return false;

   char *end = buffer + i;
   for(size_t j = 0; j < n_params; j++){ //guardo y valido los params con strtof
     if( *end != ' ' || *end == '\n')
      return false;
    params[j] = strtof( end, &end);
   }
   if(*end != '\n')
    return false;

   return true;
 }

/* #ifdef DEBU
OJO SI PROBAS CON CONSTANT EN EL TXT SE ROMPE XQ HICE PRINT PARAMS[0]
#include <stdio.h>

int main(int argc, char *argv[]){
 FILE *f = fopen( argv[1], "rt");
 float armonicos[20][2];
 size_t n;
 if(!sintet_read_armonicos( f, armonicos, &n)){
   printf("FAIL\n");
   return 1;
 }
 for(size_t i = 0; i < n; i++){
   for(size_t j = 0; j < 2; j++)
      printf("%f ", armonicos[i][j]);
   printf("\n");
 }

 for(size_t i = 0; i < 3; i++){
   mod_t funcion;
   float params[10];
   if(!sintet_read_funcion( f, &funcion, params)){
     printf("FAIL FUNCIONES\n");
     return 1;
   }
   printf("%d\n", funcion);
   printf("%f\n", params[0]);
 }
 fclose(f);
 return 0;
}

#endif*/
