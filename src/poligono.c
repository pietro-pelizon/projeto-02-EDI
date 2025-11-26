#include "poligono.h"
#include "lista.h"
#include "linha.h"
#include "ponto.h"

#include <stdio.h>
#include <stdlib.h>
#include <math.h>


#define EPSILON 1e-10

typedef struct stPoligono {
    lista *vertices;
    int num_vertices;
    double xMin, xMax, yMin, yMax;
    bool bbox_valida;
    ponto *ultimo_ponto;

} poligono;

poligono *init_poligono() {
    poligono *p = malloc (sizeof(poligono));
    if (p == NULL) return NULL;

    p -> vertices = init_lista();
    if (p -> vertices == NULL) {
        free(p);
        return NULL;
    }

    p -> num_vertices = 0;
    p -> bbox_valida = false;
    p -> ultimo_ponto = NULL;

    return p;

}

static void adicionar_ponto_interno(poligono *p, ponto *v) {
    insert_tail(p -> vertices, v);
    p -> num_vertices++;
    p -> bbox_valida = false;
    p -> ultimo_ponto = v;
}


void insert_vertice(poligono *p, double x, double y) {
    if (p == NULL) return;

    if (p -> num_vertices > 0 && p -> ultimo_ponto != NULL) {
        double dx = x - get_x_ponto(p -> ultimo_ponto);
        double dy = y - get_y_ponto(p -> ultimo_ponto);
        double dist_sq = dx * dx + dy * dy;

        if (dist_sq < EPSILON * EPSILON) {
            return;
        }
    }

    ponto *v = init_ponto(x, y);
    adicionar_ponto_interno(p, v);
}

void insert_ponto_poligono(poligono *p, ponto *v) {
    if (p == NULL || v == NULL) return;

    double x = get_x_ponto(v);
    double y = get_y_ponto(v);

    if (p -> num_vertices > 0 && p->ultimo_ponto != NULL) {
        double dx = x - get_x_ponto(p -> ultimo_ponto);
        double dy = y - get_y_ponto(p -> ultimo_ponto);
        double dist_sq = dx * dx + dy * dy;

        if (dist_sq < EPSILON * EPSILON) {
            return;
        }
    }

    ponto *copia = copia_ponto(v);
    adicionar_ponto_interno(p, copia);
}

int get_num_vertices(poligono *p) {
    if (p == NULL) return 0;
    return p -> num_vertices;
}

ponto *get_ultimo_vertice(poligono *p) {
    if (p == NULL) return NULL;

    return get_tail_data(p -> vertices);
}


ponto *get_vertice(poligono* p, int i) {
    if (i < 0 || i >= p -> num_vertices) return NULL;

    ponto* v = get_data_index(p -> vertices, i);
    if (v == NULL) return NULL;

    return copia_ponto(v);
}

bool is_empty_poligono(poligono *p) {
    if (p == NULL) return true;
    return p -> num_vertices == 0;
}


lista* get_segmentos(poligono* p) {
    if (p == NULL || p->num_vertices < 2) return NULL;

    lista* segmentos = init_lista();
    node* atual = get_head_node(p->vertices);

    static int id_segmentos = 9000;

    for (int i = 0; i < p->num_vertices; i++) {
        ponto* p1 = get_node_data(atual);
        node* proximo = go_next_node(atual);

        if (proximo == NULL) proximo = get_head_node(p -> vertices);
        ponto* p2 = get_node_data(proximo);

        double x1 = get_x_ponto(p1);
        double y1 = get_y_ponto(p1);
        double x2 = get_x_ponto(p2);
        double y2 = get_y_ponto(p2);

        int novo_id = ++id_segmentos;

        linha* seg = criaLinha(novo_id, x1, y1, x2, y2, "#000000", false);
        insert_tail(segmentos, seg);

        atual = proximo;

        if (atual == get_head_node(p -> vertices)) break;
    }

    return segmentos;
}


lista *get_lista_vertices(poligono *p) {
    if (p -> vertices == NULL) return NULL;

   return p -> vertices;
}

void calcular_bounding_box(poligono* p) {
    if (p == NULL || p -> num_vertices == 0) return;

    node* no = get_head_node(p->vertices);
    ponto* primeiro = get_node_data(no);

    p -> xMin = p -> xMax = get_x_ponto(primeiro);
    p -> yMin = p -> yMax = get_y_ponto(primeiro);

    no = go_next_node(no);
    while (no != NULL) {
        ponto* pt = get_node_data(no);
        double x = get_x_ponto(pt);
        double y = get_y_ponto(pt);

        if (x < p -> xMin) p -> xMin = x;
        if (x > p -> xMax) p -> xMax = x;
        if (y < p -> yMin) p -> yMin = y;
        if (y > p -> yMax) p -> yMax = y;

        no = go_next_node(no);
    }

    p -> bbox_valida = true;
}

void get_bounding_box(poligono *p, double *xMin, double *xMax, double *yMin, double *yMax) {
    if (p == NULL || xMin == NULL || xMax == NULL || yMin == NULL || yMax == NULL) {
        return;
    }

    if (p->num_vertices == 0) {
        *xMin = *xMax = *yMin = *yMax = 0.0;
        return;
    }

    if (!p->bbox_valida) {
        calcular_bounding_box(p);
    }

    *xMin = p->xMin;
    *xMax = p->xMax;
    *yMin = p->yMin;
    *yMax = p->yMax;
}

bool is_inside(poligono* p, double px, double py) {
    if (p == NULL || p -> num_vertices < 3) return false;

    if (!p -> bbox_valida) {
        calcular_bounding_box(p);
    }

    if (px < p -> xMin || px > p -> xMax || py < p -> yMin || py > p -> yMax) {
        return false;
    }

    int intersecoes = 0;
    node *no1 = get_head_node(p -> vertices);
    ponto *p1 = get_node_data(no1);

    for (int i = 0; i < p -> num_vertices; i++) {
        node *no2 = go_next_node(no1);
        if (no2 == NULL) no2 = get_head_node(p -> vertices);

        ponto* p2 = get_node_data(no2);
        double x1 = get_x_ponto(p1);
        double y1 = get_y_ponto(p1);
        double x2 = get_x_ponto(p2);
        double y2 = get_y_ponto(p2);

        if ((y1 > py) != (y2 > py)) {
            double xIntersecao = (x2 - x1) * (py - y1) / (y2 - y1) + x1;
            if (px < xIntersecao) {
                intersecoes++;
            }
        }
        no1 = no2;
        p1 = p2;
    }

    return (intersecoes % 2) == 1;
}

void free_poligono(poligono* p) {
    if (p == NULL) return;

    free_lista(p -> vertices, (void (*)(void *))free_ponto);
    free(p);
}