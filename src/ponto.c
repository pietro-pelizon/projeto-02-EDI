#include "ponto.h"

#include <stdio.h>
#include <stdlib.h>


typedef struct stPonto {
    double x, y;
} ponto;

ponto *init_ponto(double x, double y) {
    ponto *p = malloc (sizeof(ponto));
    if (p == NULL) {
        printf("PONTO: Erro ao alocar memória para o ponto!\n");
        return NULL;
    }

    p -> x = x;
    p -> y = y;

    return p;
}

double get_x_ponto(ponto *p) {
    return p -> x;
}

double get_y_ponto(ponto *p) {
    return p -> y;
}

void set_x_ponto(ponto *p, double x) {
    p -> x = x;
}

void set_y_ponto(ponto *p, double y) {
    p -> y = y;
}

void free_ponto(ponto *p) {
    if (p) free(p);
}

ponto *copia_ponto(ponto *p) {
    ponto *copia = malloc (sizeof(ponto));
    if (!copia) {
        return NULL;
    }


    copia -> x = p -> x;
    copia -> y = p -> y;

    return copia;
}