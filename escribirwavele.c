#include <stdio.h>
#include <stdbool.h>
#include <stdint.h>
#include <stdlib.h>
#include <math.h>
#define MASK_1BYTE 0xff

static void _uint_32_to_str(uint8_t v[4] , uint32_t a){
    v[0] = a & MASK_1BYTE;
    v[1] = (a >> 8) & MASK_1BYTE;
    v[2] = (a >> 16) & MASK_1BYTE;
    v[3] = (a >> 24) & MASK_1BYTE;
}

static void _uint_16_to_str(uint8_t v[2] , uint16_t a){
    v[0] = a & MASK_1BYTE;
    v[1] = (a>>8) & MASK_1BYTE;
}

static float _max_search(float v[] , size_t n){
    size_t max = 0;
    for(size_t i = 0 ; i < n ; i++){
        if(v[max] < v[i]){
            max = i;
        }
    }
    return v[max];
}

void crear_vector_int(float *f, size_t n, uint16_t recip[]){
    float max_vector = max_search(f , n);
    for(size_t i = 0 ; i < n ; i++){
        recip[i] = f[1] * pow(2 , 15) / max_vector;
    }
}

bool escritor_muestras(uint16_t a, FILE *f){
    uint8_t vaux[2];
    uint_16_to_str(vaux, a);
    fwrite(vaux, 1, 2, f);
}

bool escribir_wave(char *r, float v[], size_t n, uint32_t fm){
    FILE *f = fopen(r , "wb");
    if(f == NULL)
        return false;
    char ChunkId[4] = "FFIR";//RIFF
    if(fwrite(ChunkId , 4 , 1 , f) != 1){
        fclose(f);
        return false;
    } 

    uint32_t ChunkSize = 36 + 2*n;
    uint8_t size[4];
    uint_32_to_str(size , ChunkSize);
    if(fwrite(size , 1 , 4 , f) != 4){
        fclose(f);
        return false;
    }
    char Format[4] = "EVAW";//WAVE
    if(fwrite(Format ,1 ,4 ,f) != 4){
        fclose(f);
        return false;
    }
    char SubChunk1ID[4] = " tmf";//fmt 
    if(fwrite(SubChunk1ID , 1 , 4 , f) != 4){
        fclose(f);
        return false;
    }
    uint16_t SubChunk1Size = 16;
    uint8_t subsize[2];
    uint_16_to_str(subsize , SubChunk1Size);
    if(fwrite(subsize , 1 , 2 , f) != 2){
        fclose(f);
        return false;
    }
    uint16_t AudioFormat = 1;
    uint8_t aformat[2];
    uint_16_to_str(aformat , AudioFormat);
    if(fwrite(aformat , 1 , 2 , f) != 2){
        fclose(f);
        return false;
    }
    uint16_t NumChannels = 1;
    uint8_t nc[2];
    uint_16_to_str(nc , NumChannels);
    if(fwrite(nc, 1,2 ,f) != 2){
        fclose(f); 
        return false;
    }
    uint32_t SampleRate = fm;
    uint8_t sr[4];
    uint_32_to_str(sr , SampleRate);
    if(fwrite(sr ,1 ,4 ,f) != 4){
        fclose(f);
        return false;
    }
    uint32_t ByteRate = SampleRate * 2;
    uint8_t br[4];
    uint_32_to_str(br , ByteRate);
    if(fwrite(br ,1 ,4 ,f) != 4){
        fclose(f);
        return false;
    }
    uint16_t BlockAlign = 2;
    uint8_t ba[2];
    uint_16_to_str(ba , BlockAlign);
    if(fwrite(ba, 1,2 ,f) != 2){
        fclose(f); 
        return false;
    }
    uint16_t BitsPerSample = 16;
    uint8_t bps[2];
    uint_16_to_str(bps , BitsPerSample);
    if(fwrite(bps, 1,2 ,f) != 2){
        fclose(f); 
        return false;
    }
    char SubChunk2ID[4] = "atad";//data
    if(fwrite(SubChunk2ID ,1 ,4 ,f) != 4){
        fclose(f);
        return false;
    }
    uint32_t SubChunk2Size = 2*n;
    uint8_t sc2s[4];
    uint_32_to_str(sc2s , SubChunk2Size);
    if(fwrite(sc2s ,1 ,4 ,f) != 4){
        fclose(f);
        return false;
    }

    uint16_t vi[n];
    crear_vector_int(v, n, vi);
    for(size_t i = 0; i<n ; i++){
        escritor_muestras(vi[i] , f);
    }
    return true;
}