#ifndef ANTEPARO_H
#define ANTEPARO_H

#include "anteparo.h"
#include "ponto.h"

/*
    O anteparo é, em essência, um 'objeto' do tipo
    linha, só que melhorado com a capacidade
    de aguentar o impacto da bomba e proteger as
    demais formas que não estão na região de visibilidade
*/

// O 'objeto' anteparo
typedef struct stAnteparo anteparo;

/// @brief Inicializa um anteparo com extremos com coordena (x0, y0) e (x1, y1)
/// @param id Identificador do anteparo
/// @param x0 Coordenada X do primeiro ponto
/// @param y0 Coordenada Y do primeiro ponto
/// @param x1 Coordenada X do segundo ponto
/// @param y1 Coordenada Y do segundo ponto
/// @param cor Cor do anteparo
/// @return Retorna um anteparo criado
anteparo *init_anteparo(int id, double x0, double y0, double x1, double y1, char *cor);

/// @brief Pega e retorna o identificador do anteparo
/// @param a Ponteiro para o anteparo
/// @return Identificador do anteparo
int get_id_anteparo(anteparo *a);

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

/// @brief Retorna a coordenada x do ponto inicial do anteparo
/// @param a Ponteiro para o anteparo
/// @return X do ponto inicial
double get_x_p0(anteparo *a);

/// @brief Retorna a coordenada y do ponto inicial do anteparo
/// @param a Ponteiro para o anteparo
/// @return Y do ponto inicial
double get_y_p0(anteparo *a);

/// @brief Retorna a coordenada x do ponto final do anteparo
/// @param a Ponteiro para o anteparo
/// @return X do ponto final
double get_x_p1(anteparo *a);

/// @brief Retorna a coordenada y do ponto final do anteparo
/// @param a Ponteiro para o anteparo
/// @return Y do ponto final
double get_y_p1(anteparo *a);

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

#endif