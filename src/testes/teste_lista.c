#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <assert.h>

#include "arvore.h"
#include "../lista.h"

typedef struct {
    int id;
    char nome[20];
} TesteData;

void print_teste_data(void *data) {
    TesteData *td = (TesteData*)data;
    printf("TesteData{id=%d, nome=%s}", td->id, td->nome);
}

int compare_teste_data(void *a, void *b) {
    TesteData *td1 = (TesteData*)a;
    TesteData *td2 = (TesteData*)b;
    return td1->id - td2->id;
}

void free_teste_data(void *data) {
    free(data);
}

int predicado_id_par(void *data) {
    TesteData *td = (TesteData*)data;
    return td->id % 2 == 0;
}

int predicado_id_maior_que_2(void *data) {
    TesteData *td = (TesteData*)data;
    return td->id > 2;
}

void transforma_incrementa_id(void *data) {
    TesteData *td = (TesteData*)data;
    td->id += 100;
}

void test_lista_vazia() {
    printf("=== Teste Lista Vazia ===\n");

    lista *l = init_lista();
    assert(l != NULL);
    assert(get_tam_lista(l) == 0);
    assert(is_empty(l) == 1);

    free_lista(l, NULL);
    printf("✓ Lista vazia OK\n");
}

void test_insercao_remocao_basica() {
    printf("=== Teste Inserção e Remoção Básica ===\n");

    lista *l = init_lista();

    TesteData *td1 = malloc(sizeof(TesteData));
    td1->id = 1;
    snprintf(td1->nome, 20, "Primeiro");
    insert_head(l, td1);

    assert(get_tam_lista(l) == 1);
    assert(get_head(l) == td1);
    assert(get_tail(l) == td1);

    TesteData *td2 = malloc(sizeof(TesteData));
    td2->id = 2;
    snprintf(td2->nome, 20, "Segundo");
    insert_tail(l, td2);

    assert(get_tam_lista(l) == 2);
    assert(get_tail(l) == td2);

    TesteData *removido_head = remove_head(l);
    assert(removido_head == td1);
    assert(get_tam_lista(l) == 1);
    free(removido_head);

    TesteData *removido_tail = remove_tail(l);
    assert(removido_tail == td2);
    assert(get_tam_lista(l) == 0);
    free(removido_tail);

    free_lista(l, NULL);
    printf("✓ Inserção e remoção básica OK\n");
}

void test_insercao_por_indice() {
    printf("=== Teste Inserção por Índice ===\n");

    lista *l = init_lista();

    TesteData *td[3];
    for (int i = 0; i < 3; i++) {
        td[i] = malloc(sizeof(TesteData));
        td[i]->id = i;
        snprintf(td[i]->nome, 20, "Item%d", i);
        insert_tail(l, td[i]);
    }

    TesteData *td_meio = malloc(sizeof(TesteData));
    td_meio->id = 99;
    snprintf(td_meio->nome, 20, "Meio");
    index_insert(l, td_meio, 1);

    assert(get_tam_lista(l) == 4);
    assert(get_index(l, 1) == td_meio);
    assert(get_index(l, 2) == td[1]);

    free_lista(l, free_teste_data);
    printf("✓ Inserção por índice OK\n");
}

void test_busca_operacoes() {
    printf("=== Teste Busca e Operações ===\n");

    lista *l = init_lista();

    TesteData *td[5];
    for (int i = 0; i < 5; i++) {
        td[i] = malloc(sizeof(TesteData));
        td[i]->id = i;
        snprintf(td[i]->nome, 20, "Item%d", i);
        insert_tail(l, td[i]);
    }

    TesteData chave = {3, ""};
    TesteData *encontrado = search_lista(l, &chave, compare_teste_data);
    assert(encontrado == td[3]);

    assert(get_index(l, 0) == td[0]);
    assert(get_index(l, 4) == td[4]);

    assert(contains(l, &chave, compare_teste_data) == 1);

    TesteData nao_existe = {99, ""};
    assert(contains(l, &nao_existe, compare_teste_data) == 0);

    free_lista(l, free_teste_data);
    printf("✓ Busca e operações OK\n");
}

void test_find_max_min_simples() {
    printf("=== Teste Find Max/Min (Simples) ===\n");

    lista *l = init_lista();

    TesteData *test_data[3];

    test_data[0] = malloc(sizeof(TesteData));
    test_data[0]->id = 5;
    snprintf(test_data[0]->nome, 20, "Cinco");
    insert_tail(l, test_data[0]);

    test_data[1] = malloc(sizeof(TesteData));
    test_data[1]->id = 10;
    snprintf(test_data[1]->nome, 20, "Dez");
    insert_tail(l, test_data[1]);

    test_data[2] = malloc(sizeof(TesteData));
    test_data[2]->id = 3;
    snprintf(test_data[2]->nome, 20, "Tres");
    insert_tail(l, test_data[2]);

    TesteData *max = find_max(l, compare_teste_data);
    assert(max != NULL);
    assert(max->id == 10);
    printf("✓ Find_max OK: %d\n", max->id);

    TesteData *min = find_min(l, compare_teste_data);
    assert(min != NULL);
    assert(min->id == 3);
    printf("✓ Find_min OK: %d\n", min->id);

    free_lista(l, free_teste_data);
    printf("✓ Find max/min simples OK\n");
}

void test_operacoes_avancadas() {
    printf("=== Teste Operações Avançadas ===\n");

    lista *l = init_lista();

    TesteData *td = malloc(sizeof(TesteData));
    td->id = 10;
    snprintf(td->nome, 20, "Item10");
    insert_tail(l, td);

    map_lista(l, transforma_incrementa_id);
    assert(((TesteData*)get_index(l, 0))->id == 110);

    printf("✓ Map OK\n");

    free_lista(l, free_teste_data);
}

void test_limpeza_inversao() {
    printf("=== Teste Limpeza e Inversão ===\n");

    lista *l = init_lista();

    for (int i = 0; i < 3; i++) {
        TesteData *td = malloc(sizeof(TesteData));
        td->id = i;
        snprintf(td->nome, 20, "Item%d", i);
        insert_tail(l, td);
    }

    printf("Antes do reverse: ");
    for (int i = 0; i < 3; i++) {
        TesteData *td = (TesteData*)get_index(l, i);
        printf("%d ", td->id);
    }
    printf("\n");

    reverse_lista(l);

    printf("Depois do reverse: ");
    for (int i = 0; i < 3; i++) {
        TesteData *td = (TesteData*)get_index(l, i);
        printf("%d ", td->id);
    }
    printf("\n");

    TesteData *primeiro = (TesteData*)get_index(l, 0);
    TesteData *terceiro = (TesteData*)get_index(l, 2);
    assert(primeiro->id == 2);
    assert(terceiro->id == 0);

    clear_lista(l, free_teste_data);
     assert(get_tam_lista(l) == 0);
     assert(is_empty(l) == 1);

    free_lista(l, free_teste_data);

    printf("✓ Limpeza e inversão OK\n");
}

void test_find_max_min_completo() {
    printf("=== Teste Find Max/Min Completo ===\n");

    lista *l = init_lista();

    TesteData *test_data[5];
    int ids[5] = {5, 10, 3, 15, 7};

    for (int i = 0; i < 5; i++) {
        test_data[i] = malloc(sizeof(TesteData));
        test_data[i]->id = ids[i];
        snprintf(test_data[i]->nome, 20, "Item%d", ids[i]);
        insert_tail(l, test_data[i]);
    }

    TesteData *max = find_max(l, compare_teste_data);
    assert(max != NULL);
    assert(max->id == 15);
    printf("✓ Find_max OK: %d\n", max->id);

    TesteData *min = find_min(l, compare_teste_data);
    assert(min != NULL);
    assert(min->id == 3);
    printf("✓ Find_min OK: %d\n", min->id);

    free_lista(l, free_teste_data);
    printf("✓ Find max/min completo OK\n");
}

void test_remove_all_if_completo() {
    printf("=== Teste Remove All If ===\n");

    lista *l = init_lista();

    TesteData *td[5];
    for (int i = 0; i < 5; i++) {
        td[i] = malloc(sizeof(TesteData));
        td[i]->id = i; // 0, 1, 2, 3, 4
        snprintf(td[i]->nome, 20, "Item%d", i);
        insert_tail(l, td[i]);
    }

    int removidos = remove_all_if(l, predicado_id_par, free_teste_data);
    assert(removidos == 3);
    assert(get_tam_lista(l) == 2);

    TesteData *first = (TesteData*)get_index(l, 0);
    assert(first->id == 1);
    TesteData *second = (TesteData*)get_index(l, 1);
    assert(second->id == 3);

    free_lista(l, free_teste_data);
    printf("✓ Remove_all_if OK\n");
}

void test_clear_lista() {
    printf("=== Teste Clear Lista ===\n");

    lista *l = init_lista();

    for (int i = 0; i < 3; i++) {
        TesteData *td = malloc(sizeof(TesteData));
        td->id = i;
        snprintf(td->nome, 20, "Item%d", i);
        insert_tail(l, td);
    }

    assert(get_tam_lista(l) == 3);

    clear_lista(l, free_teste_data);
    assert(get_tam_lista(l) == 0);
    assert(is_empty(l) == 1);

    TesteData *new_td = malloc(sizeof(TesteData));
    new_td->id = 100;
    snprintf(new_td->nome, 20, "NovoItem");
    insert_head(l, new_td);

    assert(get_tam_lista(l) == 1);
    assert(((TesteData*)get_head(l))->id == 100);

    free_lista(l, free_teste_data);
    printf("✓ Clear lista OK\n");
}

void test_remove_all_if() {
    printf("=== Teste Remove All If ===\n");

    lista *l = init_lista();

    TesteData *td[6];
    for (int i = 0; i < 6; i++) {
        td[i] = malloc(sizeof(TesteData));
        td[i]->id = i; // 0, 1, 2, 3, 4, 5
        snprintf(td[i]->nome, 20, "Item%d", i);
        insert_tail(l, td[i]);
    }

    printf("Lista original: ");
    print_lista(l, print_teste_data);
    printf("\n");

    int removidos = remove_all_if(l, predicado_id_par, free_teste_data);
    printf("Removidos %d elementos pares\n", removidos);

    assert(removidos == 3);
    assert(get_tam_lista(l) == 3);

    TesteData *first = (TesteData*)get_index(l, 0);
    assert(first->id == 1);
    TesteData *second = (TesteData*)get_index(l, 1);
    assert(second->id == 3);
    TesteData *third = (TesteData*)get_index(l, 2);
    assert(third->id == 5);

    printf("Lista após remoção dos pares: ");
    print_lista(l, print_teste_data);
    printf("\n");

    removidos = remove_all_if(l, predicado_id_maior_que_2, free_teste_data);
    printf("Removidos %d elementos com id > 2\n", removidos);

    assert(removidos == 2);
    assert(get_tam_lista(l) == 1);

    TesteData *last = (TesteData*)get_index(l, 0);
    assert(last->id == 1);

    printf("Lista final: ");
    print_lista(l, print_teste_data);
    printf("\n");

    free_lista(l, free_teste_data);
    printf("✓ Remove_all_if OK\n");
}

int main() {
    printf("INICIANDO TESTES DA LISTA\n\n");

    test_lista_vazia();
    test_insercao_remocao_basica();
    test_insercao_por_indice();
    test_busca_operacoes();
    test_find_max_min_simples();
    test_find_max_min_completo();
    test_operacoes_avancadas();
    test_remove_all_if_completo();
    test_clear_lista();
    test_limpeza_inversao();
    test_remove_all_if();

    printf("\n========================================\n");
    printf("✓ TODOS OS TESTES PASSARAM!\n");
    printf("========================================\n");

    return 0;
}