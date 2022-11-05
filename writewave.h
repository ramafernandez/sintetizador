#ifndef WRITE_WAVE
#define WRITE_WAVE

#include <stdlib.h>

#define SUBCHUNCK1SIZE 16
#define AUDIOFORMAT 1
#define NUMCHANNELS 1
#define BLOCKALIGN 16
#define BITSPERSAMPLE 16
/*#define CHUNKLD 
#define FORMAT "EVAW"
#define SUBCHUNK1ID " tmf"
#define SUBCHUNK2ID "atad"*/
#define MASK_1BYTE 0xff

bool escribir_wave(char *r, float *v, size_t n, uint32_t fm);

#endif
