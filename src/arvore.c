#include "arvore.h"
#include <stdio.h>
#include <stdlib.h>

typedef struct stNode {
    void *data;
    struct stNode *esquerda;
    struct stNode *direita;
} node_bst;

typedef struct stArvore {
    node_bst *root;
    int tamanho;
    int (*cmp_data)(const void *a, const void *b);
    void (*free_data)(void *data);
    void (*print_data)(void *data);
} arvore;

arvore *init_arvore(int (*cmp_data)(const void *a, const void *b), void (*free_data)(void *data), void (*print_data)(void *data)) {
    arvore *a = malloc (sizeof(arvore));
    if (a == NULL) {
        printf("DEBUG: Erro ao criar a estrutura árvore!\n");
        return NULL;
    }

    a -> root = NULL;
    a -> tamanho = 0;
    a -> cmp_data = cmp_data;
    a -> free_data = free_data;
    a -> print_data = print_data;

    return a;
}

bool is_empty_bst(arvore *bst) {
    if (bst -> tamanho == 0) {
        return true;
    }

    return false;
}

static node_bst *create_node(void *data) {
    node_bst *node = malloc (sizeof(node_bst));
    if (node == NULL) {
        printf("DEBUG: Erro ao criar nó para a BST!\n");
        return NULL;
    }

    node -> data = data;
    node -> esquerda = NULL;
    node -> direita = NULL;

    return node;
}

static node_bst *insert_bst(arvore *t, node_bst *node, void *data) {
    if (node == NULL) {
        node_bst *new_node = create_node(data);
        t -> tamanho++;
        return new_node;
    }

    int c = t -> cmp_data(data, node -> data);

    if (c < 0) {
        node -> esquerda = insert_bst(t, node -> esquerda, data);
    }

    else if (c > 0) {
        node -> direita = insert_bst(t, node -> direita, data);
    }

    else {
        printf("DEBUG: Dado duplicado não inserido.\n");
        if (t -> free_data != NULL) {
            t -> free_data(data);
        }
    }


    return node;

}

void arvore_insere(arvore *t, void *data) {
    t -> root = insert_bst(t, t -> root, data);
    t -> tamanho++;
}

