#include <math.h>
#include "fmodulacion.h"

#include <stdio.h>

#define E 2.718281828459045
#define PI 3.14159265359
//la mayoria de las funciones solo reciben params[0] = t0
//seno recibe params[0] = a , params[1] = f
//las dos ultimas reciben params[0] = t0,
//params[1] = t1 y params[2] = a1.

float constant(double t, float params[3]){
    return 1;
}

float linear(double t, float params[3]){
    return t/params[0];
}

float invlinear(double t, float params[3]){
    float aux = 1 - t/params[0];
    if(aux < 0)
        return 0;
    return aux;
}

float seno(double t , float params[3]){
   return (1 + params[0] * sin(params[1] * t));
}

float exponente(double t, float params[3]){
    return pow(E , 5*(t - params[0])/params[0]);
}

float invexp(double t, float params[3]){
    return pow(E , (-5 * t)/params[0]);
}

float quartcos(double t, float params[3]){
    return cos((PI * t)/(2*params[0]));
}

float quartsin(double t, float params[3]){
    return sin((PI * t)/(2*params[0]));
}

float halfcos(double t, float params[3]){
    return (1 + quartcos(t, params))/2;
}

float halfsin(double t, float params[3]){
    return (1 + sin(PI*(t/params[0] - 1/2)))/2;
}

float logarithm(double t, float params[3]){
    return log10f((9*t/params[0]) + 1);
}

float invlog(double t, float params[3]){
    if(t < params[0])
        return log10((-9 * t / params[0]) + 10);
    return 0;
}

float tri(double t, float params[3]){
    if(t < params[1])
        return (t*params[2])/params[1];
    if(t > params[1])
        return ( ((t-params[1])/params[1]-params[0]) * (params[2] - 1) + params[2]);
    return params[2];
}

float pulses(double t, float params[3]){
    double tprim = (t/params[0]) - floor((t/ params[0]) * params[0]);
    double modulo = (((1 - params[2]) / params[1]) * (tprim - params[0] + params[1]));
    if(modulo < 0)
        modulo = -modulo;
    float result = modulo + params[2];
    if(result < 1)
        return result;
    return 1;
}
