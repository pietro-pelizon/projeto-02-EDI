#ifndef PROJETO_02_EDI_SORT_H
#define PROJETO_02_EDI_SORT_H
#include <stddef.h>

/// @brief Função que compara doubles para os algoritmos de ordenação
/// @param a Ponteiro para o primeiro dado (double)
/// @param b Ponteiro para o segundo dado (double)
/// @return Retorna um número negativo se a for menor que b,
/// um número positivo se a for maior que b, e 0 se os dois forem iguais
int cmp_double(const void *a, const void *b);

/// @brief Insertion Sort genérico para um array de elementos genéricos
/// @param base Ponto de partida do Insertion Sort (segundo elemento do array)
/// @param num_elementos Número de elementos contidos no array
/// @param tamanho_elemento Sizeof do tipo dos elementos
/// @param cmp_data Função que compara o tipo contido no array
void insertion_sort_generico(void *base, size_t num_elementos, size_t tamanho_elemento, int (*cmp_data)(const void *a, const void *b));

/// @brief Merge Sort genérico introduzido para array com tipos genéricos, segue a mesma lógica de parâmetros do 'qsort' da <stdlib> de C
/// @param base Ponto de partida do Merge Sort
/// @param num_elementos Número de elementos contidos no array
/// @param tamanho_elemento Sizeof do tipo dos elementos
/// @param cmp_data Função que compara o tipo contido no array
/// @param threshold Indica o tamanho usado como referência para chamar o Insertion Sort
void merge_sort_generico(void *base, size_t num_elementos, size_t tamanho_elemento, int (*cmp_data)(const void *a, const void *b), int threshold);

#endif //PROJETO_02_EDI_SORT_H