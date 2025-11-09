#ifndef ARVORE_H
#define ARVORE_H
#include <stdbool.h>

typedef struct stNode node_bst;
typedef struct stArvore arvore;

/// @brief Inicializa uma árvore e a retorna
/// @param cmp_data Função de comparação que a árvore usára em seus procedimentos internos
/// @param free_data Função de exclusão que a árvore usára em seus procedimentos internos
/// @param print_data Função de printar que a árvore usára em seus procedimentos internos
/// @return Retorna uma árvore criada
arvore *init_arvore(int (*cmp_data)(const void *a, const void *b), void (*free_data)(void *data), void (*print_data)(void *data));

/// @brief Verifica se uma árvore está vazia
/// @param bst Ponteiro para a árvore binária de busca
/// @return Retorna true caso a árvore esteja vazia e false caso o contrário
bool is_empty_bst(arvore *bst);

/// @brief Insere um novo nó na árvore
/// @param t Ponteiro para a árvore
/// @param data Dado a ser inserido
void arvore_insere(arvore *t, void *data);

#endif // ARVORE_H
