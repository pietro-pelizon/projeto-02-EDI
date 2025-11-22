#include <stdio.h>
#include <string.h>

#include "formas.h"
#include "lista.h"
#include "poligono.h"
#include "sobreposicao.h"
#include "svg.h"
#include "visibilidade.h"

#define BUFFER_SIZE 512
#define COMANDO_SIZE 16

/*
As formas cujos identificadores estão na
faixa [i, j] são pintadas com a tinta
bloqueante (viram anteparos)
Círculos são transformados em segmentos
horizontais (h) ou verticais (h).
Segmentos devem ter identificadores únicos.
Cor do segmento é o mesmo da cor da
borda da figura original.
TXT: reportar id e tipo da figura original,
id e extremos dos segmentos produzidos
*/

static void comando_a(char *buffer, lista *formas, lista *anteparos, FILE *arquivo_txt) {
    int id_i, id_j; char orientacao = 'h';

    sscanf(buffer, "a %i %i %c", &id_i, &id_j, &orientacao);

    fprintf(arquivo_txt, "[*] a %i %i %c\n", id_i, id_j, orientacao);

    lista *formas_a_remover = init_lista();
    node *head = get_head_node(formas);


    while (head != NULL) {
        forma *f = get_node_data(head);
        int id_forma = get_id_forma(f);

        if (id_forma >= id_i && id_forma <= id_j) {

            const char* tipo_nome[] = {"CÍRCULO", "RETÂNGULO", "LINHA", "TEXTO", "ANTEPARO"};
            fprintf(arquivo_txt, "Forma original: ID -> %i | Tipo -> %s\n", get_id_forma(f), tipo_nome[get_tipo_forma(f)]);

            lista *anteparos_da_transformacao = forma_anteparo(f, orientacao);

            node *anteparo_node = get_head_node(anteparos_da_transformacao);
            while (anteparo_node != NULL) {
                forma *anteparo_forma = get_node_data(anteparo_node);
                anteparo *a = get_dados_forma(anteparo_forma);
                fprintf(arquivo_txt, "Segmento(s) produzidos pela forma de ID -> %i:\n", get_id_forma(f));
                fprintf(arquivo_txt, "Segmento ID %d: (%.2f, %.2f) <-> (%.2f, %.2f)\n\n",
                       get_id_forma(anteparo_forma), get_x_p0(a), get_y_p0(a), get_x_p1(a), get_y_p1(a));


                insert_tail(anteparos, anteparo_forma);
                anteparo_node = go_next_node(anteparo_node);

            }

            insert_tail(formas_a_remover, f);
            free_lista(anteparos_da_transformacao, NULL);
        }

        head = go_next_node(head);
    }

    node *remover = get_head_node(formas_a_remover);
    while (remover != NULL) {
        forma *f = get_node_data(remover);

        node *busca = get_head_node(formas);
        int idx = 0;
        while (busca != NULL) {
            if (get_node_data(busca) == f) {
                remove_index(formas, idx);
                destrutor_forma(f);
                break;
            }
            busca = go_next_node(busca);
            idx++;
        }

        remover = go_next_node(remover);
    }

    free_lista(formas_a_remover, NULL);
}

/*
Bomba de destruição é lançada na
coordenada (x,y)
TXT: reportar id e tipo das formas
destruídas
SVG: desenhar a região de vis da
bomba. A região de vis deve ser
desenhada em um novo arquivo cujo nome
tem o sufixo sfx ou no próprio arquivo svg
final, caso sfx seja “-”.
*/

static void comando_d(int threshold_i, char tipo_ord, char *buffer, lista *formas, lista *anteparos, FILE *arquivo_txt,
    const char *path_base_svg, const char *path_saida, FILE *svg_final) {
    double x_impacto, y_impacto;  char sfx[64] = "-";

    sscanf(buffer, "d %lf %lf %s", &x_impacto, &y_impacto, sfx);

    double raio_max = 10000.0;
    ponto *bomba = init_ponto(x_impacto, y_impacto);
    poligono *vis = calc_regiao_visibilidade(bomba, anteparos, tipo_ord, raio_max, threshold_i);

    fprintf(arquivo_txt, "[*] d %.2lf %.2lf %s\n", x_impacto, y_impacto, sfx);

    lista *formas_destruidas = init_lista();

    node *head = get_head_node(formas);
    while (head != NULL) {
        forma *f = get_node_data(head);

        if (forma_sobrepoe_poligono(f, vis)) {
            insert_tail(formas_destruidas, f);

            const char* tipo_nome[] = {"CÍRCULO", "RETÂNGULO", "LINHA", "TEXTO", "ANTEPARO"};
            fprintf(arquivo_txt, "Forma destruída - ID -> %i | Tipo -> %s\n", get_id_forma(f), tipo_nome[get_tipo_forma(f)]);

        }
        head = go_next_node(head);
    }

    if (strcmp(sfx, "-") != 0) {
        char path_svg[512];
        sprintf(path_svg, "%s/%s-%s.svg", path_saida, path_base_svg, sfx);

        FILE *svg_vis = inicializa_svg(path_svg);
        if (svg_vis) {
            insere_poligono_visibilidade(svg_vis, vis);
            fecha_svg(svg_vis);

        }
    }

    else {
            if (svg_final != NULL) {
                insere_poligono_visibilidade(svg_final, vis);
                insere_bomba_svg(svg_final, x_impacto, y_impacto);

            }
    }

    node *destruida = get_head_node(formas_destruidas);
    while (destruida != NULL) {
        forma *f = get_node_data(destruida);

        node *busca = get_head_node(formas);
        int idx = 0;
        while (busca != NULL) {
            if (get_node_data(busca) == f) {
                remove_index(formas, idx);
                destrutor_forma(f);
                break;
            }
            busca = go_next_node(busca);
            idx++;
        }

        destruida = go_next_node(destruida);
    }

    free_lista(formas_destruidas, NULL);
    free_ponto(bomba);
    free_poligono(vis);
}

/*
Bomba de pintura é lançada na coordenada
(x,y). Formas dentro da região de
vis tem suas cores de
preenchimento e de borda pintadas com a
cor cor.
TXT: reportar id e tipo das formas pintadas
SVG: semelhante ao comando d.
*/

static void comando_p(lista *formas, lista *anteparos, FILE *arquivo_txt, char *buffer,
    int threshold_i, char tipo_ord, const char *path_saida, const char *path_base_svg, FILE *svg_final) {
    double x_impacto, y_impacto; char sfx[64] = "-"; char cor[16] = "#FF0000";

    sscanf(buffer, "p %lf %lf %s %s", &x_impacto, &y_impacto, cor, sfx);
    ponto *bomba = init_ponto(x_impacto, y_impacto);

    double raio_max = 10000.0;
    poligono *vis = calc_regiao_visibilidade(bomba, anteparos, tipo_ord, raio_max, threshold_i);

    node *head = get_head_node(formas);

    fprintf(arquivo_txt, "[*] p %.2lf %.2lf %s %s\n", x_impacto, y_impacto, cor, sfx);
    while (head != NULL) {
        forma *f = get_node_data(head);
        if (forma_sobrepoe_poligono(f, vis)) {
            const char* tipo_nome[] = {"CÍRCULO", "RETÂNGULO", "LINHA", "TEXTO", "ANTEPARO"};
            fprintf(arquivo_txt, "Forma pintada - ID -> %i | Tipo -> %s\n", get_id_forma(f), tipo_nome[get_tipo_forma(f)]);

            set_corp_formas(f, cor);
            set_corb_formas(f, cor);
        }
        head = go_next_node(head);
    }

    if (strcmp(sfx, "-") != 0) {
        char path_svg[512];
        sprintf(path_svg, "%s/%s-%s.svg", path_saida, path_base_svg, sfx);

        FILE *svg_vis = inicializa_svg(path_svg);
        if (svg_vis) {
            insere_poligono_visibilidade(svg_vis, vis);
            fecha_svg(svg_vis);
        }
        else {
            if (svg_final != NULL) {
                insere_poligono_visibilidade(svg_final, vis);
                insere_bomba_svg(svg_final, x_impacto, y_impacto);
            }
        }
    }

    free_ponto(bomba);
    free_poligono(vis);
}

/*
Bomba de clonagem é lançada na
coordenada (x,y). Os clones são trasladados
em dx, dy (nos eixos x e y, respectivamente).
Note que clones devem ter identificadores
únicos.
TXT. id e tipo das figuras originais e dos
clones.
SVG: semelhante ao comando d.
*/


static void comando_cln(lista *formas, lista *anteparos, FILE *arquivo_txt,
    int threshold_i, char tipo_ord, const char *path_saida, const char *path_base_svg, char *buffer, FILE *svg_final) {

    double x_impacto, y_impacto, dx_translacao, dy_translacao; char sfx[64] = "-";

    sscanf(buffer, "cln %lf %lf %lf %lf %s", &x_impacto, &y_impacto, &dx_translacao, &dy_translacao, sfx);

    ponto *bomba = init_ponto(x_impacto, y_impacto);
    double raio_max = 10000.0;;
    poligono *vis = calc_regiao_visibilidade(bomba, anteparos, tipo_ord, raio_max, threshold_i);

    node *head = get_head_node(formas);
    lista *clones = init_lista();
    fprintf(arquivo_txt, "[*] cln %.2lf %.2lf %.2lf %.2lf %s\n", x_impacto, y_impacto, dx_translacao, dy_translacao, sfx);
    while (head != NULL) {
        forma *f = get_node_data(head);

        if (forma_sobrepoe_poligono(f, vis)) {
            forma *clone = clona_forma(f);

            const char* tipo_nome[] = {"CÍRCULO", "RETÂNGULO", "LINHA", "TEXTO", "ANTEPARO"};
            fprintf(arquivo_txt, "Forma original - ID -> %i | Tipo -> %s\n ", get_id_forma(f), tipo_nome[get_tipo_forma(f)]);
            fprintf(arquivo_txt, "Forma clonada - ID -> %i | Tipo -> %s\n ", get_id_forma(clone), tipo_nome[get_tipo_forma(clone)]);

            set_posicao_forma(clone, get_x_forma(f) + dx_translacao, get_y_forma(f) + dy_translacao);

            insert_tail(clones, clone);
        }

        head = go_next_node(head);
    }

    node *clone_node = get_head_node(clones);
    while (clone_node != NULL) {
        forma *clone = get_node_data(clone_node);
        insert_tail(formas, clone);
        clone_node = go_next_node(clone_node);

    }

    if (strcmp(sfx, "-") != 0) {
        char path_svg[512];
        sprintf(path_svg, "%s/%s-%s.svg", path_saida, path_base_svg, sfx);

        FILE *svg_vis = inicializa_svg(path_svg);
        if (svg_vis) {
            insere_poligono_visibilidade(svg_vis, vis);
            fecha_svg(svg_vis);
        } else {
            if (svg_final != NULL) {
                insere_poligono_visibilidade(svg_final, vis);
                insere_bomba_svg(svg_final, x_impacto, y_impacto);

            }
        }
    }

    free_lista(clones, NULL);
    free_ponto(bomba);
    free_poligono(vis);

}

void parser_qry(lista *formas, lista *anteparos, char *nome_path_qry, char *nome_path_txt,
    int threshold_i, char tipo_ord, const char *path_saida, const char *path_base_svg, FILE *svg_final) {

    FILE *arquivo_qry = fopen(nome_path_qry, "r");
    if (arquivo_qry == NULL) {
        printf("DEBUG: Erro ao abrir o arquivo .qry!\n");
        return;
    }

    FILE *arquivo_txt = fopen(nome_path_txt, "w");
    if (arquivo_txt == NULL) {
        printf("DEBUG: Erro ao abrir o arquivo .txt!\n");
        return;
    }

    char comando[COMANDO_SIZE];
    char buffer[BUFFER_SIZE];

    while (fgets(buffer, sizeof(buffer), arquivo_qry)) {
        sscanf(buffer, "%s", comando);

        if (strcmp(comando, "a") == 0) {
            comando_a(buffer, formas, anteparos, arquivo_txt);
        }
        else if (strcmp(comando, "d") == 0) {
            comando_d(threshold_i, tipo_ord, buffer, formas, anteparos,
                     arquivo_txt, path_base_svg, path_saida, svg_final);

        }
        else if (strcmp(comando, "p") == 0) {
            comando_p(formas, anteparos, arquivo_txt, buffer, threshold_i,
                     tipo_ord, path_saida, path_base_svg, svg_final);
        }
        else if (strcmp(comando, "cln") == 0) {
            comando_cln(formas, anteparos, arquivo_txt, threshold_i, tipo_ord,
                       path_saida, path_base_svg, buffer, svg_final);

        }
    }

    fclose(arquivo_qry);
    fclose(arquivo_txt);

}