#ifndef PROJETO_02_EDI_VARREDURA_H
#define PROJETO_02_EDI_VARREDURA_H

#include "anteparo.h"
#include "lista.h"
#include "ponto.h"
#include "arvore.h"
#include "poligono.h"

/// @brief Estrutura interna com informações de cada segmento para a varredura
typedef struct stInfo_segmento info_segmento;

/// @brief Estrutura interna para segmentos ativos na AVL durante a varredura
typedef struct stSegmento_ativo segmento_ativo;

/// @brief Calcula a distância da bomba até onde o raio bate no anteparo
/// @param a Anteparo a ser testado
/// @param p_bomba Ponto de origem da bomba
/// @param angulo Direção do raio em radianos
/// @return Distância até a interseção, ou DBL_MAX se não bater
double calc_dist_anteparo_bomba(anteparo *a, ponto *p_bomba, double angulo);

/// @brief Coleta os ângulos das extremidades de todos os anteparos
/// @param bomba Ponto de origem
/// @param anteparos Lista de anteparos
/// @param angulos Array alocado dinamicamente com os ângulos (deve ser liberado pelo chamador)
/// @param num_angulos Quantidade de ângulos coletados
void get_angulos_criticos(ponto *bomba, lista *anteparos, double **angulos, int *num_angulos);

/// @brief Prepara informações dos segmentos para a varredura
/// @param bomba Ponto de origem da explosão
/// @param anteparos Lista de anteparos
/// @return Lista com informações calculadas (liberar com free_lista após uso)
lista *preparar_segmentos(ponto *bomba, lista *anteparos);

/// @brief Atualiza a AVL inserindo ou removendo segmentos ativos no ângulo atual
/// @param seg_ativo AVL de segmentos ativos
/// @param angulo Ângulo atual da varredura
/// @param info_seg Lista com informações dos segmentos
void update_AVL_angulo(arvore *seg_ativo, double angulo, lista *info_seg);

/// @brief Lança um raio e retorna onde ele para usando a AVL
/// @param seg_ativos AVL com segmentos visíveis no ângulo
/// @param bomba Origem do raio
/// @param angulo Direção do raio
/// @param raio_max Distância máxima se não bater em nada
/// @return Ponto onde o raio para (liberar com free_ponto após uso)
ponto *raio_ate_anteparo_avl(arvore *seg_ativos, ponto *bomba, double angulo, double raio_max);

/// @brief Calcula o polígono de visibilidade da bomba usando varredura angular com AVL
/// @param bomba Ponto de origem da explosão
/// @param anteparos Lista de anteparos que bloqueiam a visão
/// @param tipo_ord Tipo de ordenação: 'q' para qsort, 'm' para mergesort
/// @param raio_max Alcance máximo da explosão
/// @param threshold_i Limite para insertionsort no mergesort (ignorado se tipo_ord='q')
/// @return Polígono da região visível (liberar com free_poligono após uso)
poligono *calc_regiao_visibilidade(ponto *bomba, lista *anteparos, char tipo_ord, double raio_max, int threshold_i);

#endif //PROJETO_02_EDI_VARREDURA_H