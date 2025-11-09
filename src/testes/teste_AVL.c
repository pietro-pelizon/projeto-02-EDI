#define _POSIX_C_SOURCE 200809L
#include "arvore.h"
#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <assert.h>

int cmp_int(const void *a, const void *b) {
    int *ia = (int *)a;
    int *ib = (int *)b;
    return (*ia - *ib);
}

void free_int(void *data) {
    free(data);
}

void print_int(void *data) {
    printf("%d ", *(int *)data);
}

int *create_int(int value) {
    int *p = malloc(sizeof(int));
    if (p) *p = value;
    return p;
}

int cmp_string(const void *a, const void *b) {
    return strcmp((char *)a, (char *)b);
}

void free_string(void *data) {
    free(data);
}

void print_string(void *data) {
    printf("%s ", (char *)data);
}

char *create_string(const char *str) {
    if (str == NULL) return NULL;
    size_t len = strlen(str) + 1;
    char *copy = malloc(len);
    if (copy) {
        memcpy(copy, str, len);
    }
    return copy;
}


void test_init() {
    printf("\n--- teste 1: inicializacao ---\n");

    arvore *t = init_arvore(cmp_int, free_int, print_int);

    if (t == NULL) {
        printf("erro ao criar arvore\n");
        return;
    }

    if (is_empty_bst(t)) {
        printf("arvore criada vazia\n");
    }

    printf("tamanho: %d\n", get_tam_AVL(t));
    printf("altura: %d\n", get_altura_arvore(t));

    free_arvore(t);
}

void test_insert() {
    printf("\n--- teste 2: insercao ---\n");

    arvore *t = init_arvore(cmp_int, free_int, print_int);

    int valores[] = {10, 5, 15, 3, 7, 12, 17};
    int n = sizeof(valores) / sizeof(valores[0]);

    for (int i = 0; i < n; i++) {
        arvore_insere(t, create_int(valores[i]));
    }

    printf("inseridos %d elementos\n", n);
    printf("tamanho: %d\n", get_tam_AVL(t));
    printf("altura: %d\n", get_altura_arvore(t));

    printf("in-order: ");
    in_order(t);
    printf("\n");

    for (int i = 0; i < n; i++) {
        int key = valores[i];
        void *data = remove_node(t, &key);
        if (data) free_int(data);
    }
    free_arvore(t);
}

void test_duplicado() {
    printf("\n--- teste 3: insercao duplicada ---\n");

    arvore *t = init_arvore(cmp_int, free_int, print_int);

    arvore_insere(t, create_int(10));
    int tam_antes = get_tam_AVL(t);

    arvore_insere(t, create_int(10));
    int tam_depois = get_tam_AVL(t);

    printf("tamanho antes: %d\n", tam_antes);
    printf("tamanho depois: %d\n", tam_depois);

    if (tam_antes == tam_depois) {
        printf("duplicado nao inserido\n");
    }

    int key = 10;
    void *data = remove_node(t, &key);
    if (data) free_int(data);

    free_arvore(t);
}

void test_busca() {
    printf("\n--- teste 4: busca ---\n");

    arvore *t = init_arvore(cmp_int, free_int, print_int);

    int valores[] = {50, 30, 70, 20, 40, 60, 80};
    int n = sizeof(valores) / sizeof(valores[0]);

    for (int i = 0; i < n; i++) {
        arvore_insere(t, create_int(valores[i]));
    }

    for (int i = 0; i < n; i++) {
        int *result = (int *)binary_search(t, &valores[i]);
        if (result != NULL) {
            printf("encontrado: %d\n", valores[i]);
        }
    }


    int inexistente = 999;
    void *result = binary_search(t, &inexistente);
    if (result == NULL) {
        printf("999 nao encontrado\n");
    }

    for (int i = 0; i < n; i++) {
        void *data = remove_node(t, &valores[i]);
        if (data) free_int(data);
    }
    free_arvore(t);
}

void test_remocao_folha() {
    printf("\n--- teste 5: remocao de folha ---\n");

    arvore *t = init_arvore(cmp_int, free_int, print_int);

    arvore_insere(t, create_int(10));
    arvore_insere(t, create_int(5));
    arvore_insere(t, create_int(15));

    printf("antes: ");
    in_order(t);
    printf("\n");

    int key = 5;
    void *data = remove_node(t, &key);
    if (data) {
        printf("removido: %d\n", *(int *)data);
        free_int(data);
    }

    printf("depois: ");
    in_order(t);
    printf("\n");
    printf("tamanho: %d\n", get_tam_AVL(t));


    key = 10;
    data = remove_node(t, &key);
    if (data) free_int(data);
    key = 15;
    data = remove_node(t, &key);
    if (data) free_int(data);

    free_arvore(t);
}

void test_remocao_um_filho() {
    printf("\n--- teste 6: remocao com 1 filho ---\n");

    arvore *t = init_arvore(cmp_int, free_int, print_int);

    arvore_insere(t, create_int(10));
    arvore_insere(t, create_int(5));
    arvore_insere(t, create_int(3));

    printf("antes: ");
    in_order(t);
    printf("\n");

    int key = 5;
    void *data = remove_node(t, &key);
    if (data) {
        printf("removido: %d\n", *(int *)data);
        free_int(data);
    }

    printf("depois: ");
    in_order(t);
    printf("\n");


    key = 10;
    data = remove_node(t, &key);
    if (data) free_int(data);
    key = 3;
    data = remove_node(t, &key);
    if (data) free_int(data);

    free_arvore(t);// Alterado de free(t)
}

void test_remocao_dois_filhos() {
    printf("\n--- teste 7: remocao com 2 filhos ---\n");

    arvore *t = init_arvore(cmp_int, free_int, print_int);

    int valores[] = {50, 30, 70, 20, 40, 60, 80};
    int n = sizeof(valores) / sizeof(valores[0]);

    for (int i = 0; i < n; i++) {
        arvore_insere(t, create_int(valores[i]));
    }

    printf("antes: ");
    in_order(t);
    printf("\n");

    int key = 50;
    void *data = remove_node(t, &key);
    if (data) {
        printf("removido: %d\n", *(int *)data);
        free_int(data);
    }

    printf("depois: ");
    in_order(t);
    printf("\n");


    for (int i = 0; i < n; i++) {
        if (valores[i] == 50) continue;
        void *d = remove_node(t, &valores[i]);
        if (d) free_int(d);
    }


    free_arvore(t);
}

void test_remocao_inexistente() {
    printf("\n--- teste 8: remocao de elemento inexistente ---\n");

    arvore *t = init_arvore(cmp_int, free_int, print_int);

    arvore_insere(t, create_int(10));
    arvore_insere(t, create_int(5));

    int tam_antes = get_tam_AVL(t);

    int key = 999;
    void *data = remove_node(t, &key);

    int tam_depois = get_tam_AVL(t);

    if (data == NULL) {
        printf("elemento nao encontrado\n");
    }

    printf("tamanho antes: %d, depois: %d\n", tam_antes, tam_depois);

    key = 10;
    data = remove_node(t, &key);
    if (data) free_int(data);
    key = 5;
    data = remove_node(t, &key);
    if (data) free_int(data);

    free_arvore(t);
}

void test_balanceamento() {
    printf("\n--- teste 9: balanceamento ---\n");

    arvore *t = init_arvore(cmp_int, free_int, print_int);

    printf("inserindo 1, 2, 3\n");
    arvore_insere(t, create_int(1));
    arvore_insere(t, create_int(2));
    arvore_insere(t, create_int(3));

    printf("in-order: ");
    in_order(t);
    printf("\n");
    printf("altura: %d\n", get_altura_arvore(t));

    for (int i = 1; i <= 3; i++) {
        void *data = remove_node(t, &i);
        if (data) free_int(data);
    }

    printf("\ninserindo 3, 2, 1\n");
    arvore_insere(t, create_int(3));
    arvore_insere(t, create_int(2));
    arvore_insere(t, create_int(1));

    printf("in-order: ");
    in_order(t);
    printf("\n");
    printf("altura: %d\n", get_altura_arvore(t));


    for (int i = 1; i <= 3; i++) {
        void *data = remove_node(t, &i);
        if (data) free_int(data);
    }

    printf("\ninserindo 3, 1, 2\n");
    arvore_insere(t, create_int(3));
    arvore_insere(t, create_int(1));
    arvore_insere(t, create_int(2));

    printf("in-order: ");
    in_order(t);
    printf("\n");
    printf("altura: %d\n", get_altura_arvore(t));


    for (int i = 1; i <= 3; i++) {
        void *data = remove_node(t, &i);
        if (data) free_int(data);
    }

    free_arvore(t);
}

void test_string() {
    printf("\n--- teste 10: arvore de strings ---\n");

    arvore *t = init_arvore(cmp_string, free_string, print_string);

    char *palavras[] = {"dog", "cat", "elephant", "ant", "bear"};
    int n = sizeof(palavras) / sizeof(palavras[0]);

    for (int i = 0; i < n; i++) {
        arvore_insere(t, create_string(palavras[i]));
    }

    printf("palavras inseridas: %d\n", get_tam_AVL(t));
    printf("altura: %d\n", get_altura_arvore(t));
    printf("in-order: ");
    in_order(t);
    printf("\n");

    char *busca = "cat";
    char *result = (char *)binary_search(t, busca);
    if (result != NULL) {
        printf("encontrado: %s\n", result);
    }

    char *inexistente = "zebra";
    result = (char *)binary_search(t, inexistente);
    if (result == NULL) {
        printf("zebra nao encontrada\n");
    }


    for (int i = 0; i < n; i++) {
        void *data = remove_node(t, palavras[i]);
        if (data) free_string(data);
    }

    free_arvore(t);
}

void test_estresse() {
    printf("\n--- teste 11: estresse ---\n");

    arvore *t = init_arvore(cmp_int, free_int, print_int);

    int n = 100;
    printf("inserindo %d elementos\n", n);

    for (int i = 0; i < n; i++) {
        arvore_insere(t, create_int(i));
    }

    printf("tamanho: %d\n", get_tam_AVL(t));
    printf("altura: %d\n", get_altura_arvore(t));

    int encontrados = 0;
    for (int i = 0; i < n; i++) {
        int *result = (int *)binary_search(t, &i);
        if (result != NULL) {
            encontrados++;
        }
    }
    printf("buscas bem sucedidas: %d/%d\n", encontrados, n);


    for (int i = 0; i < n/2; i++) {
        void *data = remove_node(t, &i);
        if (data) free_int(data);
    }

    printf("tamanho apos remover metade: %d\n", get_tam_AVL(t));


    for (int i = n/2; i < n; i++) {
        void *data = remove_node(t, &i);
        if (data) free_int(data);
    }

    printf("tamanho final: %d\n", get_tam_AVL(t));
    printf("altura final: %d\n", get_altura_arvore(t));

    free_arvore(t);
}

void test_tamanho() {
    printf("\n--- teste 12: consistencia do tamanho ---\n");

    arvore *t = init_arvore(cmp_int, free_int, print_int);

    for (int i = 0; i < 5; i++) {
        arvore_insere(t, create_int(i * 10));
        printf("inseriu %d, tamanho = %d\n", i * 10, get_tam_AVL(t));
    }

    for (int i = 0; i < 5; i++) {
        int key = i * 10;
        void *data = remove_node(t, &key);
        if (data) free_int(data);
        printf("removeu %d, tamanho = %d\n", key, get_tam_AVL(t));
    }

    free_arvore(t);
}

int main() {
    printf("testando arvore AVL\n");

    test_init();
    test_insert();
    test_duplicado();
    test_busca();
    test_remocao_folha();
    test_remocao_um_filho();
    test_remocao_dois_filhos();
    test_remocao_inexistente();
    test_balanceamento();
    test_string();
    test_estresse();
    test_tamanho();

    printf("\ntestes concluidos\n");

    return 0;
}