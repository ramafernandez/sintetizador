#ifndef FMODULACION_H
#define FMODULACION_H

typedef float (*pfmod_t)(double t , float []);

typedef int prueba_t;

typedef enum{
    CONSTANT,
    LINEAR,
    INVLINEAR,
    SIN,
    EXP,
    INVEXP,
    QUARTCOS,
    QUARTSIN,
    HALFCOS,
    HALFSIN,
    LOG,
    INVLOG,
    TRI,
    PULSES
} mod_t;

float constant(double , float []);

float linear(double , float []);

float invlinear(double , float []);

float seno(double , float []);

float exponente(double , float []);

float invexp(double , float []);

float quartcos(double , float []);

float quartsin(double , float []);

float halfcos(double , float []);

float halfsin(double , float []);

float logarithm(double , float []);

float invlog(double , float []);

float tri(double , float []);

float pulses(double , float []);

#endif
