#ifndef PROJETO_02_EDI_ANTEPARO_H
#define PROJETO_02_EDI_ANTEPARO_H

#include "anteparo.h"
#include "ponto.h"

typedef struct stAnteparo anteparo;

/// @brief Inicializa um anteparo com extremos com coordena (x0, y0) e (x1, y1)
/// @param x0 Coordenada X do primeiro ponto
/// @param y0 Coordenada Y do primeiro ponto
/// @param x1 Coordenada X do segundo ponto
/// @param y1 Coordenada Y do segundo ponto
/// @param cor Cor do anteparo
/// @return Retorna um anteparo criado
anteparo *init_anteparo(double x0, double y0, double x1, double y1, char *cor);

/// @brief Pega o ponto da extremidade inicial do anteparo
/// @param a Ponteiro para o anteparo
/// @return Retorna o ponto p0 do anteparo
ponto *get_p0_anteparo(anteparo *a);

/// @brief Pega o ponto da extremidade final do anteparo
/// @param a Ponteiro para o anteparo
/// @return Retorna o ponto p1 do anteparo
ponto *get_p1_anteparo(anteparo *a);

/// @brief Pega e retorna a atribuida ao anteparo
/// @param a Ponteiro para o anteparo
/// @return Retorna a cor do anteparo
char *get_cor_anteparo(anteparo *a);

/// @brief Define uma nova cor para o anteparo
/// @param a Ponteiro para o anteparo
/// @param new_cor String com a nova cor
void set_cor_anteparo(anteparo *a, char *new_cor);

/// @brief Define um novo ponto p0 (primeira extremidade) para o anteparo
/// @param a Ponteiro para o anteparo
/// @param new_p0 Ponteiro para o novo ponto p0
void set_p0_anteparo(anteparo *a, ponto *new_p0);

/// @brief Define um novo ponto p1 (segunda extremidade) para o anteparo
/// @param a Ponteiro para o anteparo
/// @param new_p1 Ponteiro para o novo ponto p1
void set_p1_anteparo(anteparo *a, ponto *new_p1);

/// @brief Libera toda a memória alocada para o anteparo
/// @param a Ponteiro para o anteparo a ser liberado
void free_anteparo(anteparo *a);

/// @brief Calcula a distância entre a bomba e o ponto de interseção do raio com o anteparo
/// @details Lança um raio da bomba em um ângulo específico e verifica se intercepta o anteparo.
/// Utiliza interseção de linha-segmento para determinar o ponto de colisão.
/// @param a Ponteiro para o anteparo
/// @param p_bomba Ponto de origem da bomba
/// @param angulo Ângulo do raio em radianos (0 = direita, π/2 = cima)
/// @return Distância até o ponto de interseção, ou DBL_MAX se não houver interseção
double calc_dist_anteparo_bomba(anteparo *a, ponto *p_bomba, double angulo);

#endif