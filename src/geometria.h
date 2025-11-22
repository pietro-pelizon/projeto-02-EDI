#ifndef PROJETO_02_EDI_GEOMETRIA_H
#define PROJETO_02_EDI_GEOMETRIA_H
#include "ponto.h"
#include <stdbool.h>

#include "anteparo.h"

/// @brief Calcula o ângulo de determinado ponto em relação a outro
/// @param obs Ponto de observação usado como referência para calcular o ângulo
/// @param principal Ponto que está sendo observado para o cálculo do ângulo
/// @return O ângulo calculado em radianos
double calcula_angulo(ponto *obs, ponto *principal);

/// @brief Calcula a distância ao quadrado de determinado ponto até outro,
/// usa a distância ao quadrado para economizar memória computacional, evitando sqrt()
/// @param x1 Coordenada X do primeiro ponto
/// @param y1 Coordenada Y do primeiro ponto
/// @param x2 Coordenada X do ponto de destino
/// @param y2 Coordenada Y do ponto de destino
/// @return Retorna a distância entre esses dois pontos
double distancia_quadrada(double x1, double y1, double x2, double y2);

/// @brief Determina a orientação geométrica de três pontos ordenados (p, q, r) no plano 2D.
///
/// Esta função usa o cálculo do produto vetorial dos vetores PQ e PR
/// para identificar se a sequência p -> q -> r forma uma virada à esquerda,
/// à direita, ou se os pontos são colineares
///
/// @param px Coordenada X do ponto p (origem)
/// @param py Coordenada Y do ponto p (origem)
/// @param qx Coordenada X do ponto q
/// @param qy Coordenada Y do ponto q
/// @param rx Coordenada X do ponto r
/// @param ry Coordenada Y do ponto r
/// @return 0 se os pontos são colineares
/// @return 1 se a sequência forma uma virada à esquerda (sentido anti-horário)
/// @return 2 se a sequência forma uma virada à direita (sentido horário)
int produto_vetorial(double px, double py, double qx, double qy, double rx, double ry);

/// @brief Verifica se um ponto (q) está dentro do retângulo (bounding box)
/// definido pelos pontos p e r.
/// @details Esta função é tipicamente usada como uma checagem auxiliar. Para
/// determinar se 'q' está *sobre o segmento* 'pr', você deve
/// primeiro verificar se os três pontos são colineares e,
/// em seguida, usar esta função.
/// @warning Esta função sozinha **NÃO** verifica se o ponto está no segmento.
/// Ela retornará 'true' para qualquer ponto dentro do retângulo
/// formado por p e r, mesmo que não seja colinear.
/// @param px Coordenada X do ponto p (um extremo do segmento).
/// @param py Coordenada Y do ponto p (um extremo do segmento).
/// @param qx Coordenada X do ponto q (o ponto a ser testado).
/// @param qy Coordenada Y do ponto q (o ponto a ser testado).
/// @param rx Coordenada X do ponto r (o outro extremo do segmento).
/// @param ry Coordenada Y do ponto r (o outro extremo do segmento).
/// @return true Se o ponto q está dentro do bounding box definido por p e r.
/// @return false Caso contrário.
bool is_ponto_no_segmento(double px, double py, double qx, double qy, double rx, double ry);

/// @brief Calcula a distância entre um ponto e um segmento de reta
/// @param p Ponteiro para o ponto
/// @param a Segmento para o segmento de reta (anteparo)
/// @return Retorna a distância
double calcular_distancia_ponto_segmento(ponto *p, anteparo *a);

/// @brief Calcula a distância da bomba até onde o raio bate no anteparo
/// @param a Anteparo a ser testado
/// @param p_bomba Ponto de origem da bomba
/// @param angulo Direção do raio em radianos
/// @return Distância até a interseção, ou DBL_MAX se não bater
double calc_dist_anteparo_bomba(anteparo *a, ponto *p_bomba, double angulo, double raio_max);

#endif //PROJETO_02_EDI_GEOMETRIA_H