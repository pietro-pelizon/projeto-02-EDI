#ifndef PROJETO_02_EDI_GEOMETRIA_H
#define PROJETO_02_EDI_GEOMETRIA_H
#include "ponto.h"
#include <math.h>

/// @brief Calcula o ângulo de determinado ponto em relação a outro
/// @param obs Ponto de observação usado como referência para calcular o ângulo
/// @param principal Ponto que está sendo observado para o cálculo do ângulo
/// @return O ângulo calculado
double calcula_angulo(ponto *obs, ponto *principal);

#endif //PROJETO_02_EDI_GEOMETRIA_H