#ifndef PROJETO_02_EDI_SORT_H
#define PROJETO_02_EDI_SORT_H
#include <stddef.h>

typedef struct stNode node;
typedef struct stLista lista;

/// @brief Ordena uma estrutura de dados contígua usando Insertion Sort
/// @param base O dado por onde o Insertion Sort começara a ordenação
/// @param num_elementos Número de elementos contidos na estrutura de dados
/// @param tamanho_elemento Sizeof dos elementos da estrutura
/// @param cmp_data Função de comparação que será criada se baseando no tipo contido na estrutura
/// @obs: Criei essa função acreditando que funcionaria para ordenar uma lista duplamente encadeada,
/// como descobri que não é caso, irei deixá-la aqui para possíveis utilizações
void insertion_sort_generico(void *base, size_t num_elementos, size_t tamanho_elemento, int (*cmp_data)(void *a, void *b));

/// @brief Usa insertion sort para ordenar uma lista
/// @param l Ponteiro para a lista a ser ordenada
/// @param cmp_data Função que sabe como comparar o dado contido na lista
void insertion_sort_lista(lista *l, int (*cmp_data)(void *a, void *b));

#endif //PROJETO_02_EDI_SORT_H