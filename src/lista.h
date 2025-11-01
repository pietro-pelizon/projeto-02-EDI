#ifndef LISTA_H
#define LISTA_H

#include "lista.h"

typedef struct stNode node;
typedef struct stLista lista;

/// @brief Cria uma lista duplamente ligada
/// @return Retorna uma lista duplamente ligada
lista *init_lista();

/// @brief Pega e retorna o tamanho de uma lista
/// @param l Ponteiro para a lista
/// @return Retorna o tamanho atual da lista
int get_tam_lista(lista *l);

/// @brief Insere um novo dado no início da lista
/// @param l Ponteiro para a lista
/// @param new_data Dado a ser inserido
void insert_head(lista *l, void *new_data);

/// @brief Insere um novo dado no fim da lista
/// @param l Ponteiro para a lista
/// @param new_data Dado a ser inserido
void insert_tail(lista *l, void *new_data);

/// @brief Insere um novo no na posição da lista determinada pelo index
/// @param l Ponteiro para a lista
/// @param new_data Dado a ser inserido
/// @param index Posição da lista onde o dado será inserido
void index_insert(lista *l, void *new_data, int index);

/// @brief Mostra a cada nó da lista com todos os seus dados na tela
/// @param l Ponteiro para a lista
/// @param print_data Callback para a função que sabe como imprimir determinado dado na lista
void print_lista(lista *l, void (*print_data)(void *data));

/// @brief Dado certo dado, procura na lista e retorna caso encontre
/// @param l Ponteiro para a lista
/// @param key Dado que será procurado na lista
/// @param compare Função que compara o dado atual com a key
/// @return Retorna o dado caso encontrado, caso o contrário, retorna NULL
void *search_lista(lista *l, void *key, int (*compare)(void *a, void *b));

/// @brief Remove e retorna o primeiro elemento da lista
/// @param l Ponteiro para a lista
/// @return Retorna o dado removido ou NULL se a lista estiver vazia
void *remove_head(lista *l);

/// @brief Remove e retorna o último elemento da lista
/// @param l Ponteiro para a lista
/// @return Retorna o dado removido ou NULL se a lista estiver vazia
void *remove_tail(lista *l);

/// @brief Remove o elemento na posição especificada
/// @param l Ponteiro para a lista
/// @param index Índice do elemento a ser removido
/// @return Retorna o dado removido ou NULL se o índice for inválido
void *remove_index(lista *l, int index);

/// @brief Remove a primeira ocorrência de um dado específico
/// @param l Ponteiro para a lista
/// @param key Dado a ser removido
/// @param compare Função de comparação
/// @return Retorna o dado removido ou NULL se não encontrado
void *remove_data(lista *l, void *key, int (*compare)(void *a, void *b));

/// @brief Libera o conteúdo da lista
/// @param l Ponteiro para a lista
/// @param free_data Callback da função que sabe liberar os dados da lista
void free_lista(lista *l, void (*free_data)(void *data));

#endif //LISTA_H
