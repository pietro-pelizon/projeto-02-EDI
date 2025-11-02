#ifndef LISTA_H
#define LISTA_H

#include <stdbool.h>

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

/// @brief Retorna o dado no índice especificado sem remover
/// @param l Ponteiro para a lista
/// @param index Índice do elemento desejado
/// @return Retorna o dado ou NULL se o índice for inválido
void *get_index(lista *l, int index);

/// @brief Retorna o primeiro elemento sem remover
/// @param l Ponteiro para a lista
/// @return Retorna o dado ou NULL se a lista estiver vazia
void *get_head(lista *l);

/// @brief Retorna o último elemento sem remover
/// @param l Ponteiro para a lista
/// @return Retorna o dado ou NULL se a lista estiver vazia
void *get_tail(lista *l);

/// @brief Inverte a ordem dos elementos na lista
/// @param l Ponteiro para a lista
void reverse_lista(lista *l);

/// @brief Libera os elementos de uma lista sem apagar a estatura
/// @param l Ponteiro para a lista
/// @param free_data Callback para função que sabe como liberar determinado dado
void clear_lista(lista *l, void (*free_data)(void *data));

/// @brief Aplica uma ação a cada elemento contido na lista
/// @param l Ponteiro para a lista
/// @param apply Callback para função que aplica a ação
void foreach_lista(lista *l, void (*apply)(void *data));

/// @brief Encontra o maior elemento da lista
/// @param l Ponteiro para a lista
/// @param compare Função de comparação (retorna positivo se a > b)
/// @return Ponteiro para o maior elemento ou NULL se vazia
void *find_max(lista *l, int (*compare)(void *a, void *b));

/// @brief Encontra o menor elemento da lista
/// @param l Ponteiro para a lista
/// @param compare Função de comparação (retorna negativo se a < b)
/// @return Ponteiro para o menor elemento ou NULL se vazia
void *find_min(lista *l, int (*compare)(void *a, void *b));

/// @brief Checa se um determinado dado está contido na lista
/// @param l Ponteiro para a lista
/// @param key Dado que será procurado na lista
/// @param compare Callback para a função que compara o tipo da lista
/// @return Retorna 1 se encontrar, 0 se não;
int contains(lista *l, void *key, int (*compare)(void *a, void *b));

/// @brief Filtra elementos que satisfazem uma condição
/// @return Nova lista com elementos filtrados
lista *filter_lista(lista *l, int (*predicate)(void *data));

/// @brief Aplica transformação em cada elemento
/// @param transform Função que modifica o elemento
void map_lista(lista *l, void (*transform)(void *data));

/// @brief Remove os elementos que satisfazem determinada condição
/// @param l Ponteiro para a lista
/// @param predicate Callback para a função que checa a condição
/// @param free_data Callback para a função que sabe apagar um dado
/// @return Quantidade de elementos removidos
int remove_all_if(lista *l, int (*predicate)(void *data), void (*free_data)(void *data));

/// @brief Verifica se uma lista está vazia
/// @param l Ponteiro para a lista
/// @return Retorna true caso esteja vazia e false caso o contrário
bool is_empty(lista *l);

#endif //LISTA_H
