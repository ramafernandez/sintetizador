#include <stdlib.h>
#include "readmidi.h"
#include "decodificarmidi.h"

#define METAEVENTO_FIN_DE_PISTA 0x2F
#define EVENTO_MAX_LONG 10

enum {NOTA, NOTA_VELOCIDAD};
enum {METAEVENTO_TIPO, METAEVENTO_LONGITUD};

bool read_midi(FILE *f, char canal_leer, notas_t *notas){
    formato_t formato;
    uint16_t numero_pistas;
    uint16_t pulsos_negra;

    if(! leer_encabezado(f, &formato, &numero_pistas, &pulsos_negra)) {
        return false;
    }

    for(uint16_t pista = 0; pista < numero_pistas; pista++) {
        // LECTURA ENCABEZADO DE LA PISTA:
        uint32_t tamagno_pista;
        if(! leer_pista(f, &tamagno_pista))
            return false;

        evento_t evento;
        char canal;
        int longitud;
        uint32_t tiempo = 0;
        
        // ITERAMOS LOS EVENTOS
        while(1) {
            uint32_t delta_tiempo;
            leer_tiempo(f, &delta_tiempo);
            tiempo += delta_tiempo;//Aca vamos sumando los delta tiempo entre cada evento

            // LECTURA DEL EVENTO:
            uint8_t buffer[EVENTO_MAX_LONG];
            if(! leer_evento(f, &evento, &canal, &longitud, buffer))
                return false;

            // PROCESAMOS EL EVENTO:
            if(evento == METAEVENTO && canal == 0xF) {
                // METAEVENTO:
                if(buffer[METAEVENTO_TIPO] == METAEVENTO_FIN_DE_PISTA) {
                    break;
                }
                descartar_metaevento(f, buffer[METAEVENTO_LONGITUD]);
            }

            else if(canal == canal_leer){
              //AÑADIMOS LA NOTA
              if(evento == NOTA_ENCENDIDA && buffer[NOTA_VELOCIDAD] != 0){
                if(!notas_agregar( notas, buffer, tiempo))//Aca cuando llegue el tiempo va a ser 0
                  return false;
              }
              //CONFIGURAMOS EL TIEMPO DE DURACION DE LA NOTA
              else if(evento == NOTA_APAGADA || (evento == NOTA_ENCENDIDA && buffer[NOTA_VELOCIDAD] == 0)){
                if(!notas_config( notas, tiempo, buffer[NOTA]))
                  return false;
              }
            }
        }
    }
    return true;
}
