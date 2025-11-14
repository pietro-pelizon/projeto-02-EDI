#include "anteparo.h"
#include "geometria.h"
#include "ponto.h"
#include <float.h>
#include <math.h>
#include <stdio.h>
#include <stdlib.h>
#include <string.h>

#include "formas.h"
#include "linha.h"
#include "poligono.h"
#include "visibilidade.h"

#define EPSILON 1e-10

typedef struct stAnteparo {
    int id;
    ponto *p0;
    ponto *p1;
    char *cor;
} anteparo;

anteparo *init_anteparo(int id, double x0, double y0, double x1, double y1, char *cor) {
    anteparo *a = malloc (sizeof(anteparo));
    if (!a) {
        printf("DEBUG: Erro ao alocar memória para o anteparo!\n");
        return NULL;
    }

    a -> id = id;

    a -> p0 = init_ponto(x0, y0);
    a -> p1 = init_ponto(x1, y1);

    a -> cor = malloc (strlen(cor) + 1);
    if (a -> cor == NULL) {
        printf("DEBUG: Erro ao alocar memória para a cor do anteparo!\n");
        a -> cor = NULL;
        return a;
    }
    strcpy(a -> cor, cor);

    return a;
}

int get_id_anteparo(anteparo *a) {
    return a -> id;
}

ponto *get_p0_anteparo(anteparo *a) {
    return a -> p0;
}

double get_x_p0(anteparo *a) {
    return get_x_ponto(a -> p0);
}

double get_y_p0(anteparo *a) {
    return get_y_ponto(a -> p0);
}

double get_x_p1(anteparo *a) {
    return get_x_ponto(a -> p1);
}

double get_y_p1(anteparo *a) {
    return get_y_ponto(a -> p1);
}

ponto *get_p1_anteparo(anteparo *a) {
    return a -> p1;
}

char *get_cor_anteparo(anteparo *a) {
    return a -> cor;
}

void set_cor_anteparo(anteparo *a, char *new_cor) {
    if (a -> cor != NULL) {
        free(a -> cor);
    }

    a -> cor = (char*)malloc(strlen(new_cor) + 1);
    if (a -> cor == NULL) {
        printf("DEBUG: Erro ao alocar memória para a nova cor de anteparo!\n");
        return;
    }

    strcpy(a -> cor, new_cor);
}

void set_p0_anteparo(anteparo *a, ponto *new_p0) {
    a -> p0 = new_p0;
}

void set_p1_anteparo(anteparo *a, ponto *new_p1) {
    a -> p1 = new_p1;
}

void free_anteparo(anteparo *a) {
    if (a == NULL) return;

    if (a -> p0 != NULL) {
        free_ponto(a -> p0);
    }
    if (a -> p1 != NULL) {
        free_ponto(a -> p1);
    }
    if (a -> cor != NULL) {
        free(a -> cor);

    }

    free(a);
}