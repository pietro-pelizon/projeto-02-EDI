#ifndef PROJETO_02_EDI_POLIGONO_H
#define PROJETO_02_EDI_POLIGONO_H
#include <stdbool.h>

#include "lista.h"
#include "ponto.h"

typedef struct stPoligono poligono;

/// @brief Inicializa um polígono
/// @return Retorna ele já inicializado
poligono *init_poligono();

/// @brief Insere um novo vertíce no polígono
/// @param p O polígono onde o vértice será inserido.
/// @param x A coordenada X do novo vértice.
/// @param y A coordenada Y do novo vértice.
void insert_vertice(poligono *p, double x, double y);

/// @brief Insere um novo vértice no polígono a partir de um ponto (cria uma cópia).
/// @param p O polígono onde o vértice será inserido.
/// @param v O ponto a ser copiado e adicionado como vértice.
void insert_ponto_poligono(poligono* p, ponto* v);

/// @brief Obtém o número total de vértices no polígono
/// @param p O polígono
/// @return O número de vértices
int get_num_vertices(poligono *p);

/// @brief Obtém uma cópia de um vértice específico pelo seu índice
/// @param p O polígono
/// @param i O índice do vértice (base 0)
/// @return Uma cópia do ponto no índice, ou NULL se o índice for inválido
ponto *get_vertice(poligono* p, int i);

/// @brief Verifica se o polígono está vazio (não possui vértices)
/// @param p O polígono
/// @return true se estiver vazio, false caso contrário
bool is_empty_poligono(poligono *p);

/// @brief Gera e retorna uma nova lista contendo os segmentos (arestas) do polígono
/// @param p O polígono
/// @return Uma nova lista contendo os linha/segmento (o chamador deve liberar)
lista* get_segmentos(poligono* p);

/// @brief Obtém um ponteiro para a lista interna de vértices do polígono
/// @param p O polígono
/// @return Retorna uma cópia da lista de vértices do polígono
lista *get_lista_vertices(poligono *p);

/// @brief (Re)calcula a Bounding Box do polígono
/// @param p O polígono
void calcular_bounding_Box(poligono* p);

/// @brief Obtém os valores da Bounding Box do polígono
/// @param p O polígono
/// @param xMin Ponteiro para armazenar o X mínimo
/// @param xMax Ponteiro para armazenar o X máximo
/// @param yMin Ponteiro para armazenar o Y mínimo
/// @param yMax Ponteiro para armazenar o Y máximo
void get_bounding_box(poligono *p, double *xMin, double *xMax, double *yMin, double *yMax);

/// @brief Verifica se um ponto está dentro da área do polígono
/// @param p O polígono
/// @param px Coordenada X do ponto
/// @param py Coordenada Y  do ponto
/// @return true se o ponto estiver dentro, false caso contrário
bool is_inside(poligono* p, double px, double py);

/// @brief Libera toda a memória associada ao polígono e seus vértices internos
/// @param p O polígono a ser liberado
void free_poligono(poligono* p);

#endif //PROJETO_02_EDI_POLIGONO_H