#include <stdio.h>
#include <math.h>
#include <string.h>

#include "tramo.h"

#define PI  3.1415926535897932
/*#define T01 0
#define T02 0.03
#define FM  44100
#define F1  110
#define F2  220
#define A1  1
#define A2  0.5
#define NFA 3*/


struct tramo{
	float *v;
	size_t n;
	double t0;
	int f_m;
};

static double onda(double t , float a, float f){
    return a * sin(2 * PI * f * t);
}

void inicializar_muestras(float v[], size_t n){
	for(size_t i = 0; i < n; i++)
		v[i] = 0;
}

void imprimir_muestras(const tramo_t *t){
	for(size_t i = 0; i < t->n; i++)
		printf("%f , %f\n", t->t0 + (double)i/44100, t->v[i]);
}

static void muestrear_senoidal(tramo_t *t, float f, float a){
		for(size_t i = 0; i < t->n; i++){
		  t->v[i] += onda( (t->t0 + (double)i/t->f_m), a, f);
	}
}

void muestrear_armonicos(tramo_t *t, float f, float a,
	                      float fa[20][2], size_t n_fa){
        inicializar_muestras( t->v, t->n);
        for(size_t j = 0; j < n_fa; j++){
        	 muestrear_senoidal( t, f*fa[j][0], a*fa[j][1]);
            }
}

tramo_t *_tramo_crear(double t0, double tf, int f_m){
	tramo_t *t_nuevo;
	if( (t_nuevo = malloc(sizeof(tramo_t))) == NULL)
		return NULL;

	size_t l_vector = (tf - t0) * f_m + 0.5;
	if((t_nuevo->v = malloc(l_vector *sizeof(float))) == NULL){
		free(t_nuevo);
		return NULL;
	}


	t_nuevo->n = l_vector;
	t_nuevo->t0 = t0;
	t_nuevo->f_m = f_m;
	return t_nuevo;
}

void tramo_destruir(tramo_t *t){
	if(t != NULL){
	    if(t->v != NULL){
	    	free(t->v);
	    }
		free(t);
	}

}

//Funcion auxiliar que devuelve el tiempo final del tramo t
static double _calcular_tf(const tramo_t *t){
	return (double)t->n/t->f_m + t->t0;
}

tramo_t *tramo_clonar(const tramo_t *t){
	double tf = _calcular_tf(t);

	tramo_t *t_copia = _tramo_crear( t->t0, tf, t->f_m);
	if(t_copia == NULL)
		return NULL;

	memcpy(t_copia->v, t->v, t_copia->n * sizeof(float));

	return t_copia;
}

tramo_t *tramo_crear_muestreo(double t0, double tf, int f_m, float f,
	                          float a, float fa[][2], size_t n_fa){
	tramo_t *t_muestreo = _tramo_crear( t0, tf, f_m);
	if(t_muestreo == NULL)
		return NULL;

	muestrear_armonicos( t_muestreo, f, a, fa, n_fa);
	return t_muestreo;
}

bool tramo_redimensionar(tramo_t *t, double tf){
	float *v_aux;
	size_t l_vector = (tf - t->t0) * t->f_m + 0.5;
	if((v_aux = realloc( t->v, l_vector * sizeof(float))) == NULL){
		return false;
	}

	t->v = v_aux;
	if( l_vector > t->n){
		inicializar_muestras( &(t->v[t->n]), l_vector - t->n);
	}

	t->n = l_vector;
	return true;
}

bool tramo_extender(tramo_t *destino, const tramo_t *extension){
	if((destino->t0 > extension->t0) || (destino->f_m != extension->f_m))
		return false;

	double tf_dest = _calcular_tf(destino);
	double tf_exten = _calcular_tf(extension);

	if( tf_dest < tf_exten){
		if(!tramo_redimensionar( destino, tf_exten))
			return false;
	}

	size_t pos_i_dest = (extension->t0 - destino->t0) * destino->f_m;
	for(size_t i = 0;  i < extension->n; i++)
		destino->v[i + pos_i_dest] += extension->v[i];

	return true;
}

static size_t calcular_pos(tramo_t *t, float tiempo){
  return (tiempo - t->t0) * t->f_m + 0.5;
}

//A esta la usamos para pasarle el "valor" que devuelven las funciones de modulacion
bool tramo_multiplicar(tramo_t *t, float ti, float tf, float *v, float f( double , float *)){
	//Podriamos ver de validar si desde > hasta y esas cosas
	size_t desde = calcular_pos( t, ti);
	size_t hasta = calcular_pos( t, tf);
	if(hasta > t->n || desde > t->n)
		return true;
	for(size_t i = desde; i < hasta; i++){
		t->v[i] *= f( t->t0 + (double)i/t->f_m , v);
		if(t->v[i] > 1000){
		}
	}
	return true;
}

float *tramo_get_v(tramo_t *t){
	return t->v;
}

size_t tramo_get_n(tramo_t *t){
	return t->n;
}

/*int main(){
	const float frec_amp[NFA][2] = {
		{1,0.5},
		{2,1.0},
		{3,1.5}
	};
	tramo_t *muestra1 = tramo_crear_muestreo( T01, 0.09, FM, F1, A1, frec_amp, NFA);
	tramo_t *muestra2 = tramo_crear_muestreo( T02, 0.05, FM, F2, A2, frec_amp, NFA);
	tramo_t *muestra_copia = tramo_clonar(muestra1);
	if(tramo_extender( muestra1, muestra2)){
		imprimir_muestras(muestra1);
	}
	tramo_destruir(muestra_copia);
	tramo_destruir(muestra1);
	tramo_destruir(muestra2);

	return 0;
}
*/
