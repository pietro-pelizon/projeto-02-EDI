#ifndef ARVORE_H
#define ARVORE_H
#include <stdbool.h>

typedef struct stNode node_AVL;
typedef struct stArvore arvore;

/// @brief Inicializa uma árvore e a retorna
/// @param cmp_data Função de comparação que a árvore usára em seus procedimentos internos
/// @param free_data Função de exclusão que a árvore usára em seus procedimentos internos
/// @param print_data Função de printar que a árvore usára em seus procedimentos internos
/// @return Retorna uma árvore criada
arvore *init_arvore(int (*cmp_data)(const void *a, const void *b), void (*free_data)(void *data), void (*print_data)(void *data));

/// @brief Retorna o nó raiz da árvore
/// @param t Ponteiro para a árvore
/// @return Retorna um ponteiro para o nó raiz da árvore
node_AVL *get_root(arvore *t);

/// @brief Retorna a altura máxima de um nó da árvore
/// @param t Ponteiro para a árvore
/// @return Altura máxima da árvore
int get_altura_arvore(arvore *t);

/// @brief Retorna o tamanho da árvore
/// @param t Ponteiro para a árvore
/// @return Tamanho total da árvore
int get_tam_AVL(arvore *t);

/// @brief Recebe um nó árvore como parâmetro e retorna o nó a esquerda dele
/// @param n Ponteiro para o nó de referência
/// @return Retorna o nó a esquerda desse nó
node_AVL *get_esquerda_node(node_AVL *n);

/// @brief Recebe um nó árvore como parâmetro e retorna o nó a direita dele
/// @param n Ponteiro para o nó de referência
/// @return Retorna o nó a direita desse nó
node_AVL *get_direita_node(node_AVL *n);

/// @brief Recebe um nó de árvore como parâmetro e retorna seu conteúdo interno
/// @param n Ponteiro para o nó
/// @return O conteúdo do nó
void *get_node_dataAVL(node_AVL *n);

/// @brief Verifica se uma árvore está vazia
/// @param avl Ponteiro para a árvore binária de busca
/// @return Retorna true caso a árvore esteja vazia e false caso o contrário
bool is_empty_avl(arvore *avl);

/// @brief Insere um novo nó na árvore
/// @param t Ponteiro para a árvore
/// @param data Dado a ser inserido
/// @note A árvore não devolve o dado caso ele seja
/// duplicado, ela o destrói
void arvore_insere(arvore *t, void *data);

/// @brief Faz uma busca binária na árvore
/// @param t Ponteiro para a árvore
/// @param key Dado a ser encontrado
/// @return Retorna o dado encontrado, caso não encontrar, retorna NULL
void *binary_search(arvore *t, void *key);

/// @brief Remove um nó especificado
/// @param t Ponteiro para a árvore que contém o nó
/// @param key Nó a ser removido
/// @return Retorna os dados do nó, ou NULL caso não encontrar ele na árvore
void *remove_node(arvore *t, void *key);

/// @brief Printa uma árvore seguindo o padrão in_order(esquerda, raiz, direita)
/// @param t Ponteiro para a árvore
void in_order(arvore *t);

/// @brief Libera toda a estrutura da árvore
/// @param t Ponteiro para a árvore
void free_arvore(arvore *t);

#endif // ARVORE_H
