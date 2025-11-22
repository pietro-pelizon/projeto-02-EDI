#include <string.h>
#include <math.h>
#include <stdlib.h>
#include "visibilidade.h"
#include "sort.h"

#include "arvore.h"
#include "formas.h"
#include "geometria.h"
#include "poligono.h"
#include "retangulo.h"

#define PI 3.14159265358
#define EPSILON 1e-10

typedef enum enum_tipo_evento {
    INICIO,
    FIM
}tipo_evento;

typedef struct stEvento {
    int id;
    double angulo;
    double distancia_estatica;
    tipo_evento tipo;
    anteparo *segmento;
}evento;

typedef struct stSegmento_ativo {
    anteparo *seg;
    int id;
    double dist_bomba;
} segmento_ativo;

static int cmp_eventos(const void *a, const void *b) {
    evento *e1 = (evento*)a;
    evento *e2 = (evento*)b;

    if (e1 -> angulo < e2 -> angulo - EPSILON) return -1;
    if (e1 -> angulo > e2 -> angulo + EPSILON) return 1;

    if (e1 -> tipo < e2 -> tipo) return -1;
    if (e1 -> tipo > e2 -> tipo) return 1;

    if (e1 -> distancia_estatica < e2 -> distancia_estatica - EPSILON) return -1;
    if (e1->distancia_estatica > e2 -> distancia_estatica + EPSILON) return 1;

    return 0;
}

static double normalizar_angulo(double ang) {
    while (ang < 0) ang += 2.0 * PI;
    while (ang >= 2.0 * PI) ang -= 2.0 * PI;
    return ang;
}

static void free_segmento_ativo(void *data) {
    if (data) {
        free(data);
    }
}

static bool validar_ponto(ponto *p, ponto *bomba, double raio_max) {
    if (p == NULL || bomba == NULL) return false;

    double dx = get_x_ponto(p) - get_x_ponto(bomba);
    double dy = get_y_ponto(p) - get_y_ponto(bomba);

    double dist_sq = dx * dx + dy * dy;
    double raio_max_sq = raio_max * raio_max;

    if (dist_sq < 0.0001) return false;

    if (dist_sq >= raio_max_sq * 0.999) {
        printf("DEBUG: Ponto descartado por estar no limite do raio (infinito)\n");
        return false;
    }

    double x = get_x_ponto(p);
    double y = get_y_ponto(p);
    if (!isfinite(x) || !isfinite(y)) return false;

    return true;
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

static void buscar_intersecao_avl_rec(node_AVL *node, ponto *bomba, double angulo, double *dist_min, ponto **ponto_intersecao, double raio_max) {
    if (node == NULL) return;

    buscar_intersecao_avl_rec(get_esquerda_node(node), bomba, angulo, dist_min, ponto_intersecao, raio_max);

    segmento_ativo *sa = (segmento_ativo*)get_node_dataAVL(node);

    double dist = calc_dist_anteparo_bomba(sa->seg, bomba, angulo, raio_max);

    if (dist < *dist_min) {
        *dist_min = dist;

        if (*ponto_intersecao != NULL) free_ponto(*ponto_intersecao);

        double x = get_x_ponto(bomba) + dist * cos(angulo);
        double y = get_y_ponto(bomba) + dist * sin(angulo);
        *ponto_intersecao = init_ponto(x, y);
    }

    buscar_intersecao_avl_rec(get_direita_node(node), bomba, angulo, dist_min, ponto_intersecao, raio_max);
}

static void update_evento_arvore(arvore *seg_ativo, evento *e) {
    if (e -> tipo == INICIO) {
        segmento_ativo *novo_ativo = malloc (sizeof(segmento_ativo));

        if (novo_ativo != NULL) {
            novo_ativo -> id = e -> id;
            novo_ativo -> seg = e -> segmento;
            novo_ativo -> dist_bomba = e -> distancia_estatica;

            arvore_insere(seg_ativo, novo_ativo);
            printf("DEBUG: Inserido ID %d\n", e -> id);
        }
    }

    else {
        segmento_ativo chave_busca;
        chave_busca.id = e -> id;
        chave_busca.dist_bomba = e -> distancia_estatica;

        void *seg_removido = remove_node(seg_ativo, &chave_busca);

        if (seg_removido) {
            free(seg_removido);
        }
    }
}

static void capturar_ponto_visibilidade(poligono *visibilidade, arvore *seg_ativos, ponto *bomba, double angulo, double raio_max) {
    ponto *p = raio_ate_anteparo_avl(seg_ativos, bomba, angulo, raio_max);

    if (p != NULL) {
        if (validar_ponto(p, bomba, raio_max)) {
            insert_ponto_poligono(visibilidade, p);
        }
        free_ponto(p);
    }
}

static bool deve_processar_raio(int index_atual, int num_total_eventos, evento *e) {
    if (index_atual == num_total_eventos - 1) return true;

    double angulo_atual = e[index_atual].angulo;
    double angulo_proximo = e[index_atual + 1].angulo;

    if (angulo_proximo - angulo_atual > EPSILON) {
        return true;
    }

    return false;
}

ponto *raio_ate_anteparo_avl(arvore *seg_ativos, ponto *bomba, double angulo, double raio_max) {
    if (get_tam_AVL(seg_ativos) == 0) {
        double x = get_x_ponto(bomba) + raio_max * cos(angulo);
        double y = get_y_ponto(bomba) + raio_max * sin(angulo);
        return init_ponto(x, y);
    }

    ponto *p_intersecao = NULL;
    double dist_min = raio_max;

    buscar_intersecao_avl_rec(get_root(seg_ativos), bomba, angulo, &dist_min, &p_intersecao, raio_max);

    if (p_intersecao != NULL) {
        return p_intersecao;
    }

    double x = get_x_ponto(bomba) + raio_max * cos(angulo);
    double y = get_y_ponto(bomba) + raio_max * sin(angulo);
    return init_ponto(x, y);
}

evento *preparar_segmentos(ponto *bomba, lista *anteparos, int *num_eventos) {
    if (bomba == NULL || anteparos == NULL) return NULL;

    int size_lista = get_tam_lista(anteparos);
    int max_eventos = 4 * size_lista;


    evento *vetor_eventos = malloc (max_eventos * sizeof(evento));
    if (vetor_eventos == NULL) {
        printf("DEBUG: Erro ao alocar vetor de eventos!\n");
        return NULL;
    }

    node *atual = get_head_node(anteparos);
    int k = 0;

    while (atual != NULL) {
        forma *f = get_node_data(atual);
        anteparo *a = get_dados_forma(f);
        ponto *p0 = get_p0_anteparo(a);
        ponto *p1 = get_p1_anteparo(a);


        vetor_eventos -> segmento = a;
        vetor_eventos -> id = get_id_anteparo(a);

        double angulo1 = normalizar_angulo(calcula_angulo(bomba, p0));
        double angulo2 = normalizar_angulo(calcula_angulo(bomba, p1));

        if (fabs(angulo1 - angulo2) < 0.0000001) {
            angulo2 += 0.000001;
        }

        if (angulo1 > angulo2) {
            double temp = angulo1;
            angulo1 = angulo2;
            angulo2 = temp;
            }

        double dist = calcular_distancia_ponto_segmento(bomba, a);

        if (angulo2 - angulo1 > PI) {

            vetor_eventos[k].angulo = angulo2;
            vetor_eventos[k].tipo = INICIO;
            vetor_eventos[k].segmento = a;
            vetor_eventos[k].distancia_estatica = dist;
            vetor_eventos[k].id = get_id_anteparo(a);
            k++;

            vetor_eventos[k].angulo = 2 * PI;
            vetor_eventos[k].tipo = FIM;
            vetor_eventos[k].segmento = a;
            vetor_eventos[k].distancia_estatica = dist;
            vetor_eventos[k].id = get_id_anteparo(a);
            k++;

            vetor_eventos[k].angulo = 0.0;
            vetor_eventos[k].tipo = INICIO;
            vetor_eventos[k].segmento = a;
            vetor_eventos[k].distancia_estatica = dist;
            vetor_eventos[k].id = get_id_anteparo(a);
            k++;

            vetor_eventos[k].angulo = angulo1;
            vetor_eventos[k].tipo = FIM;
            vetor_eventos[k].segmento = a;
            vetor_eventos[k].distancia_estatica = dist;
            vetor_eventos[k].id = get_id_anteparo(a);
            k++;
        }

        else {
            vetor_eventos[k].angulo = angulo1;
            vetor_eventos[k].tipo = INICIO;
            vetor_eventos[k].segmento = a;
            vetor_eventos[k].distancia_estatica = dist;
            vetor_eventos[k].id = get_id_anteparo(a);
            k++;

            vetor_eventos[k].angulo = angulo2;
            vetor_eventos[k].tipo = FIM;
            vetor_eventos[k].segmento = a;
            vetor_eventos[k].distancia_estatica = dist;
            vetor_eventos[k].id = get_id_anteparo(a);
            k++;
        }

        atual = go_next_node(atual);
    }

    *num_eventos = k;

    return vetor_eventos;
}

poligono *calc_regiao_visibilidade(ponto *bomba, lista *anteparos, char tipo_ord, double raio_max, int threshold_i) {
    poligono *visibilidade = init_poligono();

    retangulo *box_mundo = criaRetangulo(-1, -10, -10, 1010, 1010, "none", "none");
    forma *f_box = cria_forma(-1, RETANGULO, box_mundo);
    lista *paredes_mundo = forma_anteparo(f_box, 'h');

    int qtd_paredes = 0;
    node *aux = get_head_node(paredes_mundo);
    while(aux != NULL) {
        forma *seg_forma = get_node_data(aux);
        insert_tail(anteparos, seg_forma);
        qtd_paredes++;
        aux = go_next_node(aux);
    }


    int num_eventos = 0;
    evento *eventos = preparar_segmentos(bomba, anteparos, &num_eventos);

    if (num_eventos == 0) {
        // Caso sem anteparos nenhum,
        // cria um círculo grande o suficiente
        // para abranger todas as formas
        for (int i = 0; i < 32; i++) {
            double ang = (2.0 * PI * i) / 32.0;
            double x = get_x_ponto(bomba) + raio_max * cos(ang);
            double y = get_y_ponto(bomba) + raio_max * sin(ang);
            insert_vertice(visibilidade, x, y);
        }
        free(eventos);

        return visibilidade;
    }


    if (tipo_ord == 'm') {
        int threshold = (threshold_i > 0) ? threshold_i : 10;
        merge_sort_generico(eventos, num_eventos, sizeof(evento), cmp_eventos, threshold);
    }

    else {
        qsort(eventos, num_eventos, sizeof(evento), cmp_eventos);
    }

    arvore *seg_ativos = init_arvore(comparar_segmentos_ativos, free_segmento_ativo, NULL);

    // Algoritmo de sweep line
    for (int i = 0; i < num_eventos; i++) {
        evento *atual = &eventos[i];

        update_evento_arvore(seg_ativos, atual);

        if (deve_processar_raio(i, num_eventos, eventos)) {
            capturar_ponto_visibilidade(visibilidade, seg_ativos, bomba, atual -> angulo, raio_max);
        }
    }

    for(int i = 0; i < qtd_paredes; i++) {
        remove_tail(anteparos);
    }

    free_lista(paredes_mundo, (void(*)(void*))destrutor_forma);
    free(f_box);
    destrutorRetangulo(box_mundo);

    free(eventos);
    free_arvore(seg_ativos);

    if (get_num_vertices(visibilidade) > 0) {
        calcular_bounding_box(visibilidade);
    }

    return visibilidade;

}
