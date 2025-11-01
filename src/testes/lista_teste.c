#include <stdio.h>
#include <stdlib.h>
#include "../lista.h"

void print_int(void *data) {
    printf("%d ", *(int*)data);
}

void free_int(void *data) {
    free(data);
}

int compare_int(void *a, void *b) {
    int ia = *(int*)a;
    int ib = *(int*)b;
    return ia - ib;
}

void print_separator(const char *title) {
    printf("\n========== %s ==========\n", title);
}

void print_list_status(lista *l) {
    printf("Tamanho da lista: %d\n", get_tam_lista(l));
    printf("Elementos: ");
    print_lista(l, print_int);
    printf("\n");
}

int main() {
    lista *l = init_lista();

    print_separator("TESTE 1: insert_head");
    int *a = malloc(sizeof(int)); *a = 10;
    int *b = malloc(sizeof(int)); *b = 20;
    int *c = malloc(sizeof(int)); *c = 30;

    insert_head(l, a);
    insert_head(l, b);
    insert_head(l, c);
    printf("Inserindo 10, 20, 30 no head (ordem esperada: 30 20 10)\n");
    print_list_status(l);

    print_separator("TESTE 2: insert_tail");
    int *d = malloc(sizeof(int)); *d = 40;
    int *e = malloc(sizeof(int)); *e = 50;

    insert_tail(l, d);
    insert_tail(l, e);
    printf("Inserindo 40, 50 no tail (ordem esperada: 30 20 10 40 50)\n");
    print_list_status(l);

    print_separator("TESTE 3: index_insert");
    int *f = malloc(sizeof(int)); *f = 99;
    int *g = malloc(sizeof(int)); *g = 88;

    index_insert(l, f, 2);
    printf("Inserindo 99 no índice 2 (esperado: 30 20 99 10 40 50)\n");
    print_list_status(l);

    index_insert(l, g, 0);
    printf("Inserindo 88 no índice 0 (esperado: 88 30 20 99 10 40 50)\n");
    print_list_status(l);

    print_separator("TESTE 4: search_lista");
    int key = 99;
    void *found = search_lista(l, &key, compare_int);
    if (found != NULL) {
        printf("Buscando 99... Encontrado: %d\n", *(int*)found);
    } else {
        printf("Buscando 99... Não encontrado\n");
    }

    key = 777;
    found = search_lista(l, &key, compare_int);
    if (found != NULL) {
        printf("Buscando 777... Encontrado: %d\n", *(int*)found);
    } else {
        printf("Buscando 777... Não encontrado\n");
    }

    print_separator("TESTE 5: remove_head");
    void *removed = remove_head(l);
    if (removed != NULL) {
        printf("Removido do head: %d\n", *(int*)removed);
        free(removed);
    }
    printf("Lista após remoção (esperado: 30 20 99 10 40 50)\n");
    print_list_status(l);

    print_separator("TESTE 6: remove_tail");
    removed = remove_tail(l);
    if (removed != NULL) {
        printf("Removido do tail: %d\n", *(int*)removed);
        free(removed);
    }
    printf("Lista após remoção (esperado: 30 20 99 10 40)\n");
    print_list_status(l);

    print_separator("TESTE 7: remove_index");
    removed = remove_index(l, 2);
    if (removed != NULL) {
        printf("Removido do índice 2: %d\n", *(int*)removed);
        free(removed);
    }
    printf("Lista após remoção (esperado: 30 20 10 40)\n");
    print_list_status(l);

    print_separator("TESTE 8: remove_data");
    key = 20;
    removed = remove_data(l, &key, compare_int);
    if (removed != NULL) {
        printf("Removido elemento com valor 20: %d\n", *(int*)removed);
        free(removed);
    }
    printf("Lista após remoção (esperado: 30 10 40)\n");
    print_list_status(l);

    print_separator("TESTE 9: remove_data (não existe)");
    key = 999;
    removed = remove_data(l, &key, compare_int);
    if (removed == NULL) {
        printf("Tentou remover 999 (não existe) - comportamento correto\n");
    }
    print_list_status(l);

    print_separator("TESTE 10: Esvaziar lista");
    printf("Removendo todos os elementos...\n");
    while (get_tam_lista(l) > 0) {
        removed = remove_head(l);
        if (removed != NULL) {
            printf("Removido: %d\n", *(int*)removed);
            free(removed);
        }
    }
    print_list_status(l);

    print_separator("TESTE 11: Operações em lista vazia");
    removed = remove_head(l);
    printf("remove_head em lista vazia: %s\n", removed == NULL ? "NULL (correto)" : "erro!");

    removed = remove_tail(l);
    printf("remove_tail em lista vazia: %s\n", removed == NULL ? "NULL (correto)" : "erro!");

    print_separator("TESTE 12: free_lista");
    int *h = malloc(sizeof(int)); *h = 100;
    int *i = malloc(sizeof(int)); *i = 200;
    int *j = malloc(sizeof(int)); *j = 300;

    insert_tail(l, h);
    insert_tail(l, i);
    insert_tail(l, j);
    printf("Lista reconstruída: ");
    print_lista(l, print_int);
    printf("\n");

    printf("Liberando toda a lista...\n");
    free_lista(l, free_int);
    printf("Lista liberada com sucesso!\n");

    print_separator("TESTES CONCLUÍDOS");
    printf("Todos os testes foram executados!\n");

    return 0;
}

