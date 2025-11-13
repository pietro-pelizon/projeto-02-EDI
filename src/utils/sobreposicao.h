#ifndef SOBREPOSICAO_H
#define SOBREPOSICAO_H
#include <stdbool.h>

#include "formas.h"
#include "poligono.h"

/// @brief Dado uma forma do tipo, círculo, retângulo, texto ou linha e um polígono com N vertíces e arestas,
/// determina se há ou não sobreposição entre os dois
/// @param f Ponteiro para a forma
/// @param p Ponteiro para o polígono
/// @return Retorna true se há sobreposição, false caso contrário
bool forma_sobrepoe_poligono(forma *f, poligono *p);

#endif //SOBREPOSICAO_H
