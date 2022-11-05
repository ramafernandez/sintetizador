#ifndef MOD1_H
#define MOD1_H

#include <stdio.h>
#include <stdint.h>
#include <stdbool.h>

#define SHIFT_LEER_BE 8
#define HEADER_ID   0x4D546864
#define HEADER_SIZE 6
#define TRACK_ID    0x4D54726B
#define MAX_V_TIEMPO 4
#define MASK_TIME   0x80
#define MASK_TIME2  0x7f
#define SHIFT_TIME  7

#define MASK_EVENTO  0x70
#define MASK_VALID   0x80
#define MASK_CANAL   0x0f
#define SHIFT_EVENTO 4


typedef enum{
	PISTA_UNICA,
	MULTIPISTA_SINCRONICAS,
	MULTIPISTA_ASINCRONICAS
} formato_t;

typedef enum{
	NOTA_APAGADA,
	NOTA_ENCENDIDA,
	NOTA_DURANTE,
	CAMBIO_CONTROL,
	CAMBIO_PROGRAMA,
	VARIAR_CANAL,
	CAMBIO_PITCH,
	METAEVENTO
} evento_t;

typedef enum{ C, C_S, D, D_S, E, F, F_S, G, G_S, A, A_S, B} numnota_t;

bool decodificar_formato(uint16_t , formato_t *);

bool decodificar_evento(uint8_t , evento_t *, char *, int *);

bool decodificar_nota(uint8_t , numnota_t *, signed char *);


bool leer_encabezado(FILE *f, formato_t *formato, uint16_t *numero_pistas,
					uint16_t *pulsos_negra);

bool leer_pista(FILE *f, uint32_t *tamagno);

bool leer_tiempo(FILE *f, uint32_t *tiempo);

bool leer_evento(FILE *f, evento_t *evento, char *canal, int *longitud,
				uint8_t mensaje[]);

void descartar_metaevento(FILE *f, uint8_t tamagno);


#endif
