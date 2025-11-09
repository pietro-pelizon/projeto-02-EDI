#include <stdio.h>
#include <stdlib.h>
#include <stdbool.h>
#include <math.h>

#include "poligono.h"
#include "ponto.h"

#include "lista.h"
#include "linha.h"

int g_falhas = 0;

void print_teste_status(bool sucesso, const char* nome_teste) {
    if (sucesso) {
        printf("[  OK  ] %s\n", nome_teste);
    } else {
        printf("[FALHA] %s\n", nome_teste);
        g_falhas++;
    }
}

bool double_equals(double a, double b) {
    return fabs(a - b) < 0.0001;
}


void testar_ponto() {
    printf("\n--- Testando TAD Ponto ---\n");


    ponto* p1 = init_ponto(10.5, 20.0);
    print_teste_status(p1 != NULL, "init_ponto (alocação)");
    print_teste_status(double_equals(get_x_ponto(p1), 10.5) && double_equals(get_y_ponto(p1), 20.0), "get_x_ponto / get_y_ponto");

    ponto* p2 = copia_ponto(p1);
    print_teste_status(p2 != NULL && p1 != p2, "copia_ponto (alocação distinta)");
    print_teste_status(double_equals(get_x_ponto(p2), 10.5) && double_equals(get_y_ponto(p2), 20.0), "copia_ponto (valores)");

    free_ponto(p1);
    free_ponto(p2);
    print_teste_status(true, "free_ponto (executado - checar com Valgrind)");
}



void testar_poligono_basico() {
    printf("\n--- Testando TAD Poligono (Básico) ---\n");
    poligono* p = init_poligono();

    print_teste_status(p != NULL, "init_poligono");
    print_teste_status(is_empty_poligono(p) == true, "is_empty_poligono (novo)");
    print_teste_status(get_num_vertices(p) == 0, "get_num_vertices (novo)");

    insert_vertice(p, 0.0, 0.0);
    insert_vertice(p, 10.0, 0.0);

    ponto* v3 = init_ponto(10.0, 10.0);
    insert_ponto_poligono(p, v3);

    print_teste_status(is_empty_poligono(p) == false, "is_empty_poligono (com dados)");
    print_teste_status(get_num_vertices(p) == 3, "get_num_vertices (3 vértices)");

    ponto* v_teste = get_vertice(p, 1);
    print_teste_status(v_teste != NULL && double_equals(get_x_ponto(v_teste), 10.0), "get_vertice (índice 1)");

    ponto* v3_interno = get_data_index(get_lista_vertices(p), 2);
    print_teste_status(v3 != v3_interno, "insert_ponto_poligono (insere cópia, não original)");

    ponto* v_invalido = get_vertice(p, 99);
    print_teste_status(v_invalido == NULL, "get_vertice (índice fora do limite)");

    free_ponto(v3);
    free_ponto(v_teste);
    free_poligono(p);
}

void testar_poligono_bbox() {
    printf("\n--- Testando Poligono (Bounding Box) ---\n");
    poligono* p = init_poligono();

    insert_vertice(p, 10.0, 20.0);
    insert_vertice(p, 100.0, 5.0);
    insert_vertice(p, 50.0, 80.0);
    insert_vertice(p, 0.0, 40.0);

    double xMin, xMax, yMin, yMax;
    get_bounding_box(p, &xMin, &xMax, &yMin, &yMax);

    bool calc1_ok = double_equals(xMin, 0.0) && double_equals(xMax, 100.0) &&
                    double_equals(yMin, 5.0) && double_equals(yMax, 80.0);
    print_teste_status(calc1_ok, "Bounding Box (cálculo inicial)");

    insert_vertice(p, 200.0, -10.0);

    get_bounding_box(p, &xMin, &xMax, &yMin, &yMax);

    bool calc2_ok = double_equals(xMin, 0.0) && double_equals(xMax, 200.0) &&
                    double_equals(yMin, -10.0) && double_equals(yMax, 80.0);
    print_teste_status(calc2_ok, "Bounding Box (re-cálculo após inserção)");

    free_poligono(p);
}

void testar_poligono_is_inside() {
    printf("\n--- Testando Poligono (is_inside) ---\n");

    poligono* p = init_poligono();
    insert_vertice(p, 0.0, 0.0);
    insert_vertice(p, 10.0, 0.0);
    insert_vertice(p, 10.0, 10.0);
    insert_vertice(p, 0.0, 10.0);

    print_teste_status(is_inside_XY(p, 5.0, 5.0) == true, "is_inside (ponto central)");

    print_teste_status(is_inside_XY(p, 100.0, 100.0) == false, "is_inside (ponto fora - BBox)");

    print_teste_status(is_inside_XY(p, -5.0, 5.0) == false, "is_inside (ponto fora - adjacente)");

    print_teste_status(is_inside_XY(p, 5.0, 10.0) == false, "is_inside (ponto na borda horizontal)");
    print_teste_status(is_inside_XY(p, 10.0, 5.0) == false, "is_inside (ponto na borda vertical)");
    print_teste_status(is_inside_XY(p, 0.0, 0.0) == false, "is_inside (ponto no vértice)");

    free_poligono(p);
}

void testar_poligono_segmentos() {
    printf("\n--- Testando Poligono (get_segmentos) ---\n");

    poligono* p = init_poligono();
    insert_vertice(p, 0.0, 0.0);
    insert_vertice(p, 10.0, 0.0);
    insert_vertice(p, 5.0, 10.0);

    lista* segmentos = get_segmentos(p);
    print_teste_status(segmentos != NULL, "get_segmentos (alocação)");
    print_teste_status(get_tam_lista(segmentos) == 3, "get_segmentos (contagem = 3)");


    node* n_seg = get_head_node(segmentos);
    linha* s1 = (linha*) get_node_data(n_seg);
    n_seg = go_next_node(n_seg);
    linha* s2 = (linha*) get_node_data(n_seg);
    n_seg = go_next_node(n_seg);
    linha* s3 = (linha*) get_node_data(n_seg);

    bool seg1_ok = double_equals(getX1Linha(s1), 0.0) && double_equals(getY1Linha(s1), 0.0) &&
                     double_equals(getX2Linha(s1), 10.0) && double_equals(getY2Linha(s1), 0.0);
    bool seg2_ok = double_equals(getX1Linha(s2), 10.0) && double_equals(getY1Linha(s2), 0.0) &&
                     double_equals(getX2Linha(s2), 5.0) && double_equals(getY2Linha(s2), 10.0);
    bool seg3_ok = double_equals(getX1Linha(s3), 5.0) && double_equals(getY1Linha(s3), 10.0) &&
                     double_equals(getX2Linha(s3), 0.0) && double_equals(getY2Linha(s3), 0.0);

    print_teste_status(seg1_ok, "getSegmentos (Segmento 1: (0,0)->(10,0))");
    print_teste_status(seg2_ok, "getSegmentos (Segmento 2: (10,0)->(5,10))");
    print_teste_status(seg3_ok, "getSegmentos (Segmento 3: (5,10)->(0,0))");


    node *atual = get_head_node(segmentos);
    while (atual != NULL) {
        destrutorLinha(get_node_data(atual));
        atual = go_next_node(atual);
    }

    free_poligono(p);
}

int main() {

    testar_ponto();
    testar_poligono_basico();
    testar_poligono_bbox();
    testar_poligono_is_inside();
    testar_poligono_segmentos();

    printf("\n--- Testes Concluídos ---\n");
    if (g_falhas == 0) {
        printf(">>> SUCESSO: Todos os testes passaram!\n");
    } else {
        printf(">>> FALHA: %d teste(s) falharam.\n", g_falhas);
    }
    printf(">>> (Recomendado: executar com Valgrind para checar memory leaks)\n");

    return g_falhas;
}