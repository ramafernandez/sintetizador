#include <stdio.h>
#include <stdbool.h>
#include <stdint.h>
#include <math.h>

#include "writewave.h"

static bool wave_write_uint32(FILE *f , uint32_t a){
    uint8_t v[4];
    v[0] = a & MASK_1BYTE;
    v[1] = (a >> 8) & MASK_1BYTE;
    v[2] = (a >> 16) & MASK_1BYTE;
    v[3] = (a >> 24) & MASK_1BYTE;
    if(fwrite( v, sizeof(uint8_t), 4, f) != 4)
      return false;
    return true;
}

static bool wave_write_uint16(FILE *f, uint16_t a){
    uint8_t v[2];
    v[0] = a & MASK_1BYTE;
    v[1] = (a>>8) & MASK_1BYTE;
    if(fwrite( v, sizeof(uint8_t), 2, f) != 2)
      return false;
    return true;
}

static float max_search(float *v, size_t n){
    float max = v[0];
    for(size_t i = 1; i < n ; i++){
        if(max < v[i])
            max = v[i];
    }
    return max;
}

static bool wave_write_muestras(FILE *f , int16_t muestra){
    int8_t v[2];
    v[0] = muestra & MASK_1BYTE;
    v[1] = (muestra >> 8) & MASK_1BYTE;
    if(fwrite( v, sizeof(int8_t), 2, f) != 2)
      return false;
    return true;
}

bool escribir_wave(char *r, float *v, size_t n, uint32_t fm){
    FILE *f = fopen(r , "wb");
    if(f == NULL)
        return false;

    uint32_t chunkld = 0x46464952;
    if(fwrite( &chunkld, 4 , 1 , f) != 1){
        fclose(f);
        return false;
    }

    uint32_t ChunkSize = 36 + 2*n;
    if(!wave_write_uint32( f, ChunkSize)){
      fclose(f);
      return false;
    }

    uint32_t format = 0x45564157;
    if(fwrite( &format ,4 , 1,f) != 1){
        fclose(f);
        return false;
    }

    uint32_t subchunk1id = 0x20746d66;
    if(fwrite( &subchunk1id, 4 , 1 , f) != 1){
        fclose(f);
        return false;
    }

    if(!wave_write_uint16( f , SUBCHUNCK1SIZE)){
        fclose(f);
        return false;
    }

    if(!wave_write_uint16( f, AUDIOFORMAT)){
        fclose(f);
        return false;
    }

    if(!wave_write_uint16( f, NUMCHANNELS)){
        fclose(f);
        return false;
    }

    uint32_t SampleRate = fm;
    if(!wave_write_uint32( f, SampleRate)){
        fclose(f);
        return false;
    }

    uint32_t ByteRate = SampleRate * 2;
    if(!wave_write_uint32( f, ByteRate)){
        fclose(f);
        return false;
    }

    if(!wave_write_uint16( f, BLOCKALIGN)){
        fclose(f);
        return false;
    }

    if(!wave_write_uint16( f , BITSPERSAMPLE)){
        fclose(f);
        return false;
    }

    uint32_t data = 0x61746164;
    if(fwrite( &data , 4, 1 ,f) != 1){
        fclose(f);
        return false;
    }

    uint32_t SubChunk2Size = 2*n;
    if(!wave_write_uint32( f , SubChunk2Size)){
        fclose(f);
        return false;
    }

    float max_muestra = max_search( v, n);
    for(size_t i = 0; i < n ; i++){
      float aux = v[i]/ max_muestra;
      int16_t aux2 = aux * pow(2 , 15);
      wave_write_muestras( f , aux2);
    }
    fclose(f);
    return true;
}
