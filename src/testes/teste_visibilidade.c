#include <stdio.h>
#include <stdlib.h>
#include <math.h>
#include <float.h>

#include "visibilidade.h"
#include "lista.h"
#include "formas.h"
#include "poligono.h"
#include "arvore.h"
#include "sort.h"
#include "ponto.h"
#include "anteparo.h"

#define PI 3.14159265358
#define EPSILON 1e-10

typedef enum enum_tipo_evento {
    INICIO,
    FIM
} tipo_evento;

typedef struct stEvento {
    int id;
    double angulo;
    double distancia_estatica;
    tipo_evento tipo;
    anteparo *segmento;
} evento;

typedef struct stSegmento_ativo {
    anteparo *seg;
    int id;
    double dist_bomba;
} segmento_ativo;

static int comparar_segmentos_ativos_teste(const void *a, const void *b) {
    segmento_ativo *sa = (segmento_ativo*)a;
    segmento_ativo *sb = (segmento_ativo*)b;

    if (sa -> dist_bomba < sb -> dist_bomba - EPSILON) return -1;
    if (sa -> dist_bomba > sb -> dist_bomba + EPSILON) return 1;
    if (sa -> id < sb -> id) return -1;
    if (sa -> id > sb -> id) return 1;
    return 0;
}

static void free_segmento_ativo_teste(void *data) {
    if (data) free(data);
}

forma *criar_forma_anteparo_teste(int id, double x0, double y0, double x1, double y1) {
    ponto *p0 = init_ponto(x0, y0);
    ponto *p1 = init_ponto(x1, y1);
    anteparo *a = init_anteparo(id ,get_x_ponto(p0), get_y_ponto(p0), get_x_ponto(p1), get_y_ponto(p1), "black");
    forma *f = cria_forma(id, ANTEPARO, a);
    free_ponto(p0);
    free_ponto(p1);
    return f;
}

void free_forma_anteparo_teste(void *data) {
    if (!data) return;
    destrutor_forma(data);
}

void print_poligono_vertices(poligono *p) {
    if (!p) {
        printf("Poligono nulo.\n");
        return;
    }
    printf("  Vertices do poligono (%d):\n", get_num_vertices(p));

    lista *vertices = get_lista_vertices(p);
    if (!vertices) return;

    node *atual = get_head_node(vertices);
    while (atual != NULL) {
        ponto *v = (ponto*) get_node_data(atual);
        printf("    (%.2f, %.2f)\n", get_x_ponto(v), get_y_ponto(v));
        atual = go_next_node(atual);
    }
}

void print_segmento_ativo_data(void *data) {
    if (!data) return;
    segmento_ativo *sa = (segmento_ativo*)data;
    printf("(ID: %d, Dist: %.2f) ", sa->id, sa->dist_bomba);
}


void teste_calc_dist_anteparo_bomba() {
    printf("\n--- Teste: calc_dist_anteparo_bomba ---\n");

    ponto *bomba = init_ponto(0, 0);

    ponto *p0_v = init_ponto(5, -5);
    ponto *p1_v = init_ponto(5, 5);
    anteparo *a_v = init_anteparo(0 ,get_x_ponto(p0_v), get_y_ponto(p0_v), get_x_ponto(p1_v), get_y_ponto(p1_v), "black");

    double ang0 = 0.0;
    double dist0 = calc_dist_anteparo_bomba(a_v, bomba, ang0, 100.0);
    printf("Vertical, Ang 0: %.2f (Esperado: 5.00)\n", dist0);

    double ang90 = PI / 2.0;
    double dist90 = calc_dist_anteparo_bomba(a_v, bomba, ang90, 100.0);
    printf("Vertical, Ang 90: ");
    if (dist90 == DBL_MAX) printf("inf (Esperado: inf)\n");
    else printf("%.2f (Esperado: inf)\n", dist90);

    double ang45 = PI / 4.0;
    double dist45 = calc_dist_anteparo_bomba(a_v, bomba, ang45, 100.0);
    printf("Vertical, Ang 45: %.2f (Esperado: 7.07)\n", dist45);

    free_ponto(bomba); free_ponto(p0_v); free_ponto(p1_v); free_anteparo(a_v);
}

void teste_preparar_segmentos() {
    printf("\n--- Teste: preparar_segmentos ---\n");

    ponto *bomba = init_ponto(0, 0);
    lista *anteparos = init_lista();

    insert_tail(anteparos, criar_forma_anteparo_teste(1, 5, -5, 5, 5));

    int num_eventos = 0;
    evento *evts = preparar_segmentos(bomba, anteparos, &num_eventos);

    printf("Numero de eventos gerados: %d (Esperado: 2 ou 4 dependendo da normalizacao)\n", num_eventos);

    for(int i = 0; i < num_eventos; i++) {
        printf("  Evento %d: ID=%d, Ang=%.2f deg, Tipo=%s\n",
            i,
            evts[i].id,
            evts[i].angulo * 180.0/PI,
            evts[i].tipo == INICIO ? "INICIO" : "FIM");
    }

    free(evts);
    free_lista(anteparos, free_forma_anteparo_teste);
    free_ponto(bomba);
}

void teste_raio_ate_anteparo_avl() {
    printf("\n--- Teste: raio_ate_anteparo_avl ---\n");

    ponto *bomba = init_ponto(0, 0);
    arvore *seg_ativos = init_arvore(comparar_segmentos_ativos_teste, free_segmento_ativo_teste, print_segmento_ativo_data);
    double raio_max = 100.0;
    double ang0 = 0.0;

    ponto *p1 = raio_ate_anteparo_avl(seg_ativos, bomba, ang0, raio_max);
    printf("Caso 1 (AVL Vazia): Ponto (%.2f, %.2f) (Esperado: 100.00, 0.00)\n",
           get_x_ponto(p1), get_y_ponto(p1));
    free_ponto(p1);

    ponto *p0_s1 = init_ponto(10, -5); ponto *p1_s1 = init_ponto(10, 5);
    anteparo *a1 = init_anteparo(0 ,get_x_ponto(p0_s1), get_y_ponto(p0_s1), get_x_ponto(p1_s1), get_y_ponto(p1_s1), "black");

    segmento_ativo *s1 = malloc(sizeof(segmento_ativo));
    s1->id = 1; s1->dist_bomba = 10.0; s1->seg = a1;

    ponto *p0_s2 = init_ponto(5, -5); ponto *p1_s2 = init_ponto(5, 5);
    anteparo *a2 = init_anteparo(0 ,get_x_ponto(p0_s2), get_y_ponto(p0_s2), get_x_ponto(p1_s2), get_y_ponto(p1_s2), "black");

    segmento_ativo *s2 = malloc(sizeof(segmento_ativo));
    s2->id = 2; s2->dist_bomba = 5.0; s2->seg = a2;

    arvore_insere(seg_ativos, s1);
    arvore_insere(seg_ativos, s2);

    printf("AVL com 2 segs inseridos manualmente.\n");

    ponto *p2 = raio_ate_anteparo_avl(seg_ativos, bomba, ang0, raio_max);
    printf("Caso 2 (Parede perto ID=2): Ponto (%.2f, %.2f) (Esperado: 5.00, 0.00)\n",
           get_x_ponto(p2), get_y_ponto(p2));
    free_ponto(p2);

    free_arvore(seg_ativos);
    free_ponto(p0_s1); free_ponto(p0_s2);
    free_ponto(p1_s1); free_ponto(p1_s2);
    free_anteparo(a1); free_anteparo(a2);
    free_ponto(bomba);
}

void teste_calc_regiao_visibilidade() {
    printf("\n--- Teste: calc_regiao_visibilidade ---\n");

    ponto *bomba = init_ponto(0, 0);
    double raio_max = 100.0;
    int threshold = 10;

    printf("Caso 1: Uma parede vertical na frente (usando qsort)\n");
    lista *anteparos1 = init_lista();
    insert_tail(anteparos1, criar_forma_anteparo_teste(1, 10, -50, 10, 50));

    poligono *p1 = calc_regiao_visibilidade(bomba, anteparos1, 'q', raio_max, threshold);
    print_poligono_vertices(p1);

    free_poligono(p1);
    free_lista(anteparos1, free_forma_anteparo_teste);

    printf("\nCaso 2: Sem paredes (deve gerar circulo aprox)\n");
    lista *anteparos2 = init_lista();

    poligono *p2 = calc_regiao_visibilidade(bomba, anteparos2, 'm', raio_max, threshold);
    printf("Num vertices circulo: %d\n", get_num_vertices(p2));

    free_poligono(p2);
    free_lista(anteparos2, free_forma_anteparo_teste);

    printf("\nCaso 3: Caixa fechada (Bomba dentro)\n");
    lista *anteparos3 = init_lista();
    insert_tail(anteparos3, criar_forma_anteparo_teste(1, 10, -10, 10, 10));
    insert_tail(anteparos3, criar_forma_anteparo_teste(2, -10, -10, 10, -10));
    insert_tail(anteparos3, criar_forma_anteparo_teste(3, -10, -10, -10, 10));
    insert_tail(anteparos3, criar_forma_anteparo_teste(4, -10, 10, 10, 10));

    poligono *p3 = calc_regiao_visibilidade(bomba, anteparos3, 'q', raio_max, threshold);
    print_poligono_vertices(p3);

    free_poligono(p3);
    free_lista(anteparos3, free_forma_anteparo_teste);

    free_ponto(bomba);
}

int main() {
    teste_calc_dist_anteparo_bomba();
    teste_preparar_segmentos();
    teste_raio_ate_anteparo_avl();
    teste_calc_regiao_visibilidade();

    printf("\nTestes de varredura concluidos.\n");
    return 0;
}