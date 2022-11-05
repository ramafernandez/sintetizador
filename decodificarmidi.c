#include <stdlib.h>
#include "decodificarmidi.h"

static const uint8_t v_longitud[] = {
	[NOTA_APAGADA] = 2,
	[NOTA_ENCENDIDA] = 2,
	[NOTA_DURANTE] = 2,
	[CAMBIO_CONTROL] = 2,
	[CAMBIO_PROGRAMA] = 1,
	[VARIAR_CANAL] = 1,
	[CAMBIO_PITCH] = 2,
	[METAEVENTO] = 2
};

bool decodificar_formato(uint16_t valor, formato_t *formato){
	if(valor > 2)
		return false;

	*formato = valor;

	return true;
}

bool decodificar_evento(uint8_t valor, evento_t *evento, char *canal, int *longitud){
	if( !(valor & MASK_VALID) )
	    return false;

	*evento = ((MASK_EVENTO & valor) >> SHIFT_EVENTO);
	*canal = (MASK_CANAL & valor);
	*longitud = v_longitud[(*evento)];

	return true;
}

bool decodificar_nota(uint8_t valor,  numnota_t *nota, signed char *octava){
	if(valor & MASK_VALID)
		return false;

	*nota = valor % 12;
	*octava = valor/12 - 1;

	return true;
}

static uint8_t leer_uint8_t(FILE *f){
	uint8_t dato;
	fread( &dato, 1, 1, f);
	return dato;
}

static uint16_t leer_uint16_big_endian(FILE *f){
	uint8_t dato[2];
	fread( dato, 1, 2, f);
	return dato[0] << SHIFT_LEER_BE | dato[1];
}

static uint32_t leer_uint32_big_endian(FILE *f){
	uint8_t dato[4];
	fread( dato, 1, 4, f);
	return dato[0] << SHIFT_LEER_BE*3 | dato[1] << SHIFT_LEER_BE*2 | dato[2] << SHIFT_LEER_BE | dato[3];
}

bool leer_encabezado(FILE *f, formato_t *formato, uint16_t *numero_pistas,
					uint16_t *pulsos_negra){
	if( leer_uint32_big_endian(f) != HEADER_ID)
		return false;

	if( leer_uint32_big_endian(f) != HEADER_SIZE)
		return false;

	if( !(decodificar_formato(leer_uint16_big_endian(f), formato)) )
		return false;

	*numero_pistas = leer_uint16_big_endian(f);
	*pulsos_negra = leer_uint16_big_endian(f);

	return true;
}

bool leer_pista(FILE *f, uint32_t *tamagno){
	if( leer_uint32_big_endian(f) != TRACK_ID )
		return false;

	*tamagno = leer_uint32_big_endian(f);

	return true;
}

bool leer_tiempo(FILE *f, uint32_t *tiempo){
	uint8_t v_tiempo[MAX_V_TIEMPO];
	uint8_t i = 0;
	*tiempo = 0;

	while( ((v_tiempo[i] = leer_uint8_t(f)) & MASK_TIME) ){
		*tiempo |= ( v_tiempo[i] & ~MASK_TIME);
		*tiempo = (*tiempo << SHIFT_TIME);
		if( (++i) == MAX_V_TIEMPO)
			return false;
	}

	*tiempo |= (v_tiempo[i] & ~MASK_TIME);

	return true;
}

bool leer_evento(FILE *f, evento_t *evento, char *canal, int *longitud,
				uint8_t mensaje[]){

	mensaje[0] = leer_uint8_t(f);
	uint8_t i = 1;

	if(decodificar_evento( mensaje[0], evento, canal,longitud)){
		i = 0;
	}

	for( ; i < *longitud; i++)
		mensaje[i] = leer_uint8_t(f);

	return true;

}

void descartar_metaevento(FILE *f, uint8_t tamagno){
	for( uint8_t i = 0; i < tamagno; i++)
		leer_uint8_t(f);
}
