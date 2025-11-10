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

#define EPSILON 1e-10

typedef struct stAnteparo {
    ponto *p0;
    ponto *p1;
    char *cor;
} anteparo;

anteparo *init_anteparo(double x0, double y0, double x1, double y1, char *cor) {
    anteparo *a = malloc (sizeof(anteparo));
    if (!a) {
        printf("DEBUG: Erro ao alocar memória para o anteparo!\n");
        return NULL;
    }

    a -> p0 = init_ponto(x0, y0);
    a -> p1 =init_ponto(x1, y1);

    a -> cor = malloc (strlen(cor) + 1);
    if (a -> cor == NULL) {
        printf("DEBUG: Erro ao alocar memória para a cor do anteparo!\n");
        a -> cor = NULL;
        return a;
    }
    strcpy(a -> cor, cor);

    return a;
}

ponto *get_p0_anteparo(anteparo *a) {return a -> p0;}
ponto *get_p1_anteparo(anteparo *a) {return a -> p1;}
char *get_cor_anteparo(anteparo *a) {return a -> cor;}

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

void set_p0_anteparo(anteparo *a, ponto *new_p0) {a -> p0 = new_p0;}
void set_p1_anteparo(anteparo *a, ponto *new_p1) {a -> p1 = new_p1;}

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

double calc_dist_anteparo_bomba(anteparo *a, ponto *p_bomba, double angulo) {
    double p1_x = get_x_ponto(p_bomba);
    double p1_y = get_y_ponto(p_bomba);

    double p2_x = p1_x + cos(angulo);
    double p2_y = p1_y + sin(angulo);

    double p3_x = get_x_ponto(a -> p0);
    double p3_y = get_y_ponto(a -> p0);
    double p4_x = get_x_ponto(a -> p1);
    double p4_y = get_y_ponto(a -> p1);

    double D = (p1_x - p2_x) * (p3_y - p4_y) - (p1_y - p2_y) * (p3_x - p4_x);

    if (fabs(D) < EPSILON) {
        return DBL_MAX;
    }

    double t_num = (p1_x - p3_x) * (p3_y - p4_y) - (p1_y - p3_y) * (p3_x - p4_x);
    double u_num = -((p1_x - p2_x) * (p1_y - p3_y) - (p1_y - p2_y) * (p1_x - p3_x));

    double t = t_num / D;
    double u = u_num / D;

    if (t < -EPSILON) {
        return DBL_MAX;
    }

    if (u < -EPSILON || u > (1.0 + EPSILON)) {
        return DBL_MAX;
    }

    double ix = p1_x + t * (p2_x - p1_x);
    double iy = p1_y + t * (p2_y - p1_y);

    double dist = sqrt(distancia_quadrada(ix, iy, p1_x, p1_y));

    return dist;
}

