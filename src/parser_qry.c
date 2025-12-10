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


static void remover_formas_da_lista(lista *lista_principal, lista *formas_para_remover) {
    node *rem = get_head_node(formas_para_remover);
    while (rem != NULL) {
        forma *f_alvo = get_node_data(rem);

        node *busca = get_head_node(lista_principal);
        int idx = 0;
        while (busca != NULL) {
            if (get_node_data(busca) == f_alvo) {
                remove_index(lista_principal, idx);
                destrutor_forma(f_alvo);
                break;
            }
            busca = go_next_node(busca);
            idx++;
        }
        rem = go_next_node(rem);
    }
}

static void gerar_saida_visual(const char *sfx, const char *path_saida, const char *path_base_svg,
                               double x, double y, poligono *vis, FILE *svg_final) {
    if (strcmp(sfx, "-") != 0) {
        char path_svg[512];
        sprintf(path_svg, "%s/%s-%s.svg", path_saida, path_base_svg, sfx);
        FILE *svg_vis = inicializa_svg(path_svg);
        if (svg_vis) {
            insere_poligono_visibilidade(svg_vis, vis);
            insere_bomba_svg(svg_vis, x, y);
            fecha_svg(svg_vis);
        }
    } else if (svg_final != NULL) {
        insere_poligono_visibilidade(svg_final, vis);
        insere_bomba_svg(svg_final, x, y);
    }
}

/* --- Comando A (Transformar em Anteparos) --- */

static void comando_a(char *buffer, lista *formas, lista *anteparos, FILE *arquivo_txt) {
    int id_i, id_j; char orientacao = 'h';
    sscanf(buffer, "a %i %i %c", &id_i, &id_j, &orientacao);
    fprintf(arquivo_txt, "\n[*] a %i %i %c\n", id_i, id_j, orientacao);

    lista *formas_a_transformar = init_lista();
    node *head = get_head_node(formas);

    while (head != NULL) {
        forma *f = get_node_data(head);
        int id_forma = get_id_forma(f);

        if (id_forma >= id_i && id_forma <= id_j) {
            const char* tipo_nome[] = {"CÍRCULO", "RETÂNGULO", "LINHA", "TEXTO", "ANTEPARO"};
            fprintf(arquivo_txt, "Forma original: ID -> %i | Tipo -> %s\n", get_id_forma(f), tipo_nome[get_tipo_forma(f)]);

            lista *novos_anteparos = forma_anteparo(f, orientacao);
            node *ant_node = get_head_node(novos_anteparos);

            while (ant_node != NULL) {
                forma *ant_forma = get_node_data(ant_node);
                anteparo *a = get_dados_forma(ant_forma);
                fprintf(arquivo_txt, "ANTEPARO - ID -> %d: (%.2f, %.2f) <-> (%.2f, %.2f)\n",
                       get_id_forma(ant_forma), get_x_p0(a), get_y_p0(a), get_x_p1(a), get_y_p1(a));

                insert_tail(anteparos, ant_forma);
                ant_node = go_next_node(ant_node);
            }
            insert_tail(formas_a_transformar, f);
            free_lista(novos_anteparos, NULL);
        }
        head = go_next_node(head);
    }
    remover_formas_da_lista(formas, formas_a_transformar);
    free_lista(formas_a_transformar, NULL);
}

/* --- Comando 'd' (Destruir) --- */

static void processar_destruicao(lista *lista_alvo, poligono *vis, lista *destruidos, FILE *txt, bool eh_anteparo) {
    node *head = get_head_node(lista_alvo);
    while (head != NULL) {
        forma *f = get_node_data(head);
        if (get_id_forma(f) >= 0 && forma_sobrepoe_poligono(f, vis)) {
            insert_tail(destruidos, f);
            if (eh_anteparo) {
                fprintf(txt, "Anteparo destruído - ID: %d\n", get_id_forma(f));
            } else {
                const char* tipo_nome[] = {"CÍRCULO", "RETÂNGULO", "LINHA", "TEXTO", "ANTEPARO"};
                fprintf(txt, "Forma destruída - ID -> %i | Tipo -> %s\n", get_id_forma(f), tipo_nome[get_tipo_forma(f)]);
            }
        }
        head = go_next_node(head);
    }
}

static void comando_d(int threshold, char ordem, char *buffer, lista *formas, lista *anteparos, FILE *txt,
                      const char *base_svg, const char *path_out, FILE *svg_final) {
    double x, y; char sfx[64] = "-";
    sscanf(buffer, "d %lf %lf %s", &x, &y, sfx);
    fprintf(txt, "\n[*] d %.2lf %.2lf %s\n", x, y, sfx);

    ponto *bomba = init_ponto(x, y);
    poligono *vis = calc_regiao_visibilidade(bomba, anteparos, ordem, 10000.0, threshold);

    lista *formas_destruidas = init_lista();
    lista *anteparos_destruidos = init_lista();

    processar_destruicao(formas, vis, formas_destruidas, txt, false);
    processar_destruicao(anteparos, vis, anteparos_destruidos, txt, true);

    gerar_saida_visual(sfx, path_out, base_svg, x, y, vis, svg_final);

    remover_formas_da_lista(formas, formas_destruidas);
    remover_formas_da_lista(anteparos, anteparos_destruidos);

    free_lista(formas_destruidas, NULL);
    free_lista(anteparos_destruidos, NULL);
    free_ponto(bomba);
    free_poligono(vis);
}

/* --- Comando 'p' (Pintar) --- */

static void aplicar_pintura(lista *lista_alvo, poligono *vis, const char *cor, FILE *txt, bool eh_anteparo) {
    node *head = get_head_node(lista_alvo);

    while (head != NULL) {
        forma *f = get_node_data(head);

        if (forma_sobrepoe_poligono(f, vis)) {
            if (eh_anteparo) {
                fprintf(txt, "Forma pintada - ID -> %d | Tipo -> ANTEPARO\n", get_id_forma(f));
                set_corb_formas(f, (char*)cor);
            }

            else {
                const char* tipo_nome[] = {"CÍRCULO", "RETÂNGULO", "LINHA", "TEXTO", "ANTEPARO"};
                fprintf(txt, "Forma pintada - ID -> %i | Tipo -> %s\n", get_id_forma(f), tipo_nome[get_tipo_forma(f)]);
                set_corp_formas(f, (char*)cor);
                set_corb_formas(f, (char*)cor);
            }
        }

        head = go_next_node(head);
    }
}

static void comando_p(lista *formas, lista *anteparos, FILE *txt, char *buffer,
                      int threshold, char ordem, const char *path_out, const char *base_svg, FILE *svg_final) {
    double x, y; char sfx[64] = "-"; char cor[16] = "#FF0000";

    sscanf(buffer, "p %lf %lf %s %s", &x, &y, cor, sfx);
    fprintf(txt, "\n[*] p %.2lf %.2lf %s %s\n", x, y, cor, sfx);

    ponto *bomba = init_ponto(x, y);
    poligono *vis = calc_regiao_visibilidade(bomba, anteparos, ordem, 10000.0, threshold);

    aplicar_pintura(formas, vis, cor, txt, false);
    aplicar_pintura(anteparos, vis, cor, txt, true);

    gerar_saida_visual(sfx, path_out, base_svg, x, y, vis, svg_final);

    free_ponto(bomba);
    free_poligono(vis);
}

/* --- Comando 'cln' (Clonar) --- */

static lista* criar_lista_clones(lista *lista_alvo, poligono *vis, double dx, double dy, FILE *txt) {
    lista *clones = init_lista();
    node *head = get_head_node(lista_alvo);

    while (head != NULL) {
        forma *f = get_node_data(head);
        if (forma_sobrepoe_poligono(f, vis)) {
            forma *clone = clona_forma(f);

            const char* tipo_nome[] = {"CÍRCULO", "RETÂNGULO", "LINHA", "TEXTO", "ANTEPARO"};
            fprintf(txt, "Original - ID: %i | Tipo: %s\n", get_id_forma(f), tipo_nome[get_tipo_forma(f)]);
            fprintf(txt, "Clone - ID: %i | Tipo: %s\n", get_id_forma(clone), tipo_nome[get_tipo_forma(clone)]);

            set_posicao_forma(clone, get_x_forma(f) + dx, get_y_forma(f) + dy);
            insert_tail(clones, clone);
        }
        head = go_next_node(head);
    }
    return clones;
}

static void incorporar_clones(lista *lista_destino, lista *clones) {
    node *clone_node = get_head_node(clones);
    while (clone_node != NULL) {
        forma *clone = get_node_data(clone_node);
        insert_tail(lista_destino, clone);
        clone_node = go_next_node(clone_node);
    }
}

static void comando_cln(lista *formas, lista *anteparos, FILE *txt,
                        int threshold, char ordem, const char *path_out, const char *base_svg, char *buffer, FILE *svg_final) {
    double x, y, dx, dy; char sfx[64] = "-";

    sscanf(buffer, "cln %lf %lf %lf %lf %s", &x, &y, &dx, &dy, sfx);
    fprintf(txt, "\n[*] cln %.2lf %.2lf %.2lf %.2lf %s\n", x, y, dx, dy, sfx);

    ponto *bomba = init_ponto(x, y);
    poligono *vis = calc_regiao_visibilidade(bomba, anteparos, ordem, 10000.0, threshold);

    lista *clones_formas = criar_lista_clones(formas, vis, dx, dy, txt);
    lista *clones_anteparos = criar_lista_clones(anteparos, vis, dx, dy, txt);

    incorporar_clones(formas, clones_formas);
    incorporar_clones(anteparos, clones_anteparos);

    gerar_saida_visual(sfx, path_out, base_svg, x, y, vis, svg_final);

    free_lista(clones_formas, NULL);
    free_lista(clones_anteparos, NULL);
    free_ponto(bomba);
    free_poligono(vis);
}

/* --- Função Principal do Parser --- */

void parser_qry(lista *formas, lista *anteparos, char *nome_path_qry, char *nome_path_txt,
                int threshold_i, char tipo_ord, const char *path_saida, const char *path_base_svg, FILE *svg_final) {

    FILE *arquivo_qry = fopen(nome_path_qry, "r");
    if (!arquivo_qry) { printf("ERRO: Falha ao abrir .qry\n"); return; }

    FILE *arquivo_txt = fopen(nome_path_txt, "w");
    if (!arquivo_txt) { printf("ERRO: Falha ao criar .txt\n"); fclose(arquivo_qry); return; }

    char buffer[BUFFER_SIZE], comando[COMANDO_SIZE];

    while (fgets(buffer, sizeof(buffer), arquivo_qry)) {
        sscanf(buffer, "%s", comando);

        if (strcmp(comando, "a") == 0) {
            comando_a(buffer, formas, anteparos, arquivo_txt);
        } else if (strcmp(comando, "d") == 0) {
            comando_d(threshold_i, tipo_ord, buffer, formas, anteparos, arquivo_txt, path_base_svg, path_saida, svg_final);
        } else if (strcmp(comando, "p") == 0) {
            comando_p(formas, anteparos, arquivo_txt, buffer, threshold_i, tipo_ord, path_saida, path_base_svg, svg_final);
        } else if (strcmp(comando, "cln") == 0) {
            comando_cln(formas, anteparos, arquivo_txt, threshold_i, tipo_ord, path_saida, path_base_svg, buffer, svg_final);
        }
    }

    fclose(arquivo_qry);
    fclose(arquivo_txt);
}