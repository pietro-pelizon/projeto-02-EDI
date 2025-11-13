#include <float.h>
#include "varredura.h"
#include "sort.h"
#include <stdlib.h>
#include <string.h>

#include "arvore.h"
#include "formas.h"
#include "geometria.h"
#include "poligono.h"

#define EPSILON 1e-10

typedef struct stInfo_segmento {
    int id;
    anteparo *seg;
    double angulo_inicial;
    double angulo_final;
    double distancia;
} info_segmento;

typedef struct stSegmento_ativo {
    anteparo *seg;
    int id;
    double dist_bomba;
} segmento_ativo;

static void free_segmento_ativo(void *data) {
    if (data) {
        free(data);
    }
}

static int comparar_segmentos_ativos(const void *a, const void *b) {
    segmento_ativo *sa = (segmento_ativo*)a;
    segmento_ativo *sb = (segmento_ativo*)b;


    if (sa -> dist_bomba < sb -> dist_bomba - EPSILON) return -1;
    if (sa -> dist_bomba > sb -> dist_bomba + EPSILON) return 1;

    if (sa -> id < sb -> id) return -1;
    if (sa -> id > sb -> id) return 1;

    return 0;
}


lista *preparar_segmentos(ponto *bomba, lista *anteparos) {
    if (bomba == NULL || anteparos == NULL) return NULL;

    lista *info = init_lista();
    node *atual = get_head_node(anteparos);

    while (atual != NULL) {
        forma *f = get_node_data(atual);
        anteparo *a = get_dados_forma(f);

        ponto *p0 = get_p0_anteparo(a);
        ponto *p1 = get_p1_anteparo(a);

        info_segmento *info_seg = malloc(sizeof(info_segmento));
        if (info_seg == NULL) {
            printf("DEBUG: Erro ao alocar memória para as informações dos segmentos!\n");
            return NULL;
        }

        info_seg -> seg = a;
        info_seg -> id = get_id_forma(f);

        double ang0 = calcula_angulo(bomba, p0);
        double ang1 = calcula_angulo(bomba, p1);

        if (ang0 > ang1) {
            double temp = ang0; ang0 = ang1; ang1 = temp;
        }
        if (ang1 - ang0 > M_PI) {
            double temp = ang0; ang0 = ang1; ang1 = temp;
        }

            info_seg -> angulo_inicial = ang0;
            info_seg -> angulo_final = ang1;

        info_seg -> distancia = calcular_distancia_ponto_segmento(bomba, a);

        insert_tail(info, info_seg);
        atual = go_next_node(atual);
    }

    return info;
}

void update_AVL_angulo(arvore *seg_ativo, double angulo, lista *info_seg) {
    node *atual = get_head_node(info_seg);

    while (atual != NULL) {
        info_segmento *info = get_node_data(atual);

        if (fabs(angulo - info -> angulo_inicial) < EPSILON) {
            segmento_ativo *sa = malloc(sizeof(segmento_ativo));
            sa -> seg = info -> seg;
            sa -> dist_bomba = info -> distancia;
            sa -> id = info -> id;

            arvore_insere(seg_ativo, sa);
        }

        if (fabs(angulo - info -> angulo_final) < EPSILON) {
            segmento_ativo chave;
            chave.dist_bomba = info -> distancia;
            chave.id = info -> id;

            void *removido = remove_node(seg_ativo, &chave);
            if (removido) free(removido);
        }

        atual = go_next_node(atual);
    }
}

static void buscar_intersecao_avl_rec(node_AVL *node, ponto *bomba, double angulo, double *dist_min, ponto **ponto_intersecao) {
    if (node == NULL || *ponto_intersecao != NULL) {
        return;
    }

    buscar_intersecao_avl_rec(get_esquerda_node(node), bomba, angulo, dist_min, ponto_intersecao);

    if (*ponto_intersecao != NULL) return;

    segmento_ativo *sa = (segmento_ativo*)get_node_dataAVL(node);
    double dist = calc_dist_anteparo_bomba(sa->seg, bomba, angulo);

    if (dist < *dist_min) {
        *dist_min = dist;
        double x = get_x_ponto(bomba) + dist * cos(angulo);
        double y = get_y_ponto(bomba) + dist * sin(angulo);
        *ponto_intersecao = init_ponto(x, y);
        return;
    }

    buscar_intersecao_avl_rec(get_direita_node(node), bomba, angulo, dist_min, ponto_intersecao);
}



double calc_dist_anteparo_bomba(anteparo *a, ponto *p_bomba, double angulo) {
    double p1_x = get_x_ponto(p_bomba);
    double p1_y = get_y_ponto(p_bomba);

    double p2_x = p1_x + cos(angulo);
    double p2_y = p1_y + sin(angulo);

    double p3_x = get_x_ponto(get_p0_anteparo(a));
    double p3_y = get_y_ponto(get_p0_anteparo(a));
    double p4_x = get_x_ponto(get_p1_anteparo(a));
    double p4_y = get_y_ponto(get_p1_anteparo(a));

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

void get_angulos_criticos(ponto *bomba, lista *anteparos, double **angulos, int *num_angulos) {
    *num_angulos = 2 * get_tam_lista(anteparos);

    *angulos = malloc (*num_angulos * sizeof(double));
    if (*angulos == NULL) {
        printf("DEBUG: Erro ao alocar memória para o array de ângulos!\n");
        *num_angulos = 0;
        return;
    }

    int index = 0;

    node *atual  = get_head_node(anteparos);

    while (atual != NULL) {
        forma *f = get_node_data(atual);

        anteparo *a = (anteparo*)get_dados_forma(f);

        ponto *p0 = get_p0_anteparo(a);
        ponto *p1 = get_p1_anteparo(a);

        (*angulos)[index++] = calcula_angulo(bomba, p0);
        (*angulos)[index++] = calcula_angulo(bomba, p1);

        atual = go_next_node(atual);
    }
}

