#ifndef PROJETO_02_EDI_PONTO_H
#define PROJETO_02_EDI_PONTO_H

typedef struct stPonto ponto;

/// @brief Inicializa um ponto com coordenadas (x, y)
/// @param x Coordenada x do ponto
/// @param y Coordenada y do ponto
/// @return Retorna um ponto criado
ponto *init_ponto(double x, double y);

/// @brief Pega a coordenada x do ponto
/// @param p Ponteiro para o ponto
/// @return Coordenada x do ponto
double get_x_ponto(ponto *p);

/// @brief Pega a coordenada y do ponto
/// @param p Ponteiro para o ponto
/// @return Coordenada y do ponto
double get_y_ponto(ponto *p);

/// @brief Define uma nova coordenada x para o ponto
/// @param p Ponteiro para o ponto
/// @param x Nova coordenada x
void set_x_ponto(ponto *p, double x);

/// @brief Define uma nova coordenada y para o ponto
/// @param p Ponteiro para o ponto
/// @param y Nova coordenada y
void set_y_ponto(ponto *p, double y);

/// @brief Faz uma cópia do ponto
/// @param p Ponteiro para o ponto que será copiado
/// @return Cópia do ponto
ponto *copia_ponto(ponto *p);

/// @brief Libera a estrutura ponto
/// @param p Ponteiro para o ponto a ser liberado
void free_ponto(ponto *p);

#endif //PROJETO_02_EDI_PONTO_H