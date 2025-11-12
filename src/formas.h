#ifndef FORMAS_H
#define FORMAS_H
#include <stdio.h>
#include "lista.h"

/*
 * ------- TAD FORMAS -------
 * O TAD Formas provê uma interface genérica para manipular diferentes
 * tipos de objetos geométricos (círculos, retângulos, etc.) de forma
 * uniforme. A 'forma' genérica funciona como um container com os
 * seguintes conceitos:
 *
 * Forma Genérica: Um 'invólucro' (wrapper) que representa qualquer objeto
 * geométrico do programa. Não armazena dados de geometria diretamente, mas
 * aponta para eles;
 *
 * Identificador de Tipo (enum): Uma 'etiqueta' (tag) que identifica qual o
 * tipo real da forma armazenada, permitindo que
 * operações específicas sejam aplicadas corretamente;
 *
 * Ponteiro de Dados (void*): Um ponteiro genérico que aponta para a estrutura
 * de dados real e específica da forma, onde os atributos são de fato
 * armazenados.
 */

/// Identificador de tipo para as formas.
typedef enum enum_tipo_forma {
    CIRCULO,
    RETANGULO,
    LINHA,
    TEXTO,
    ANTEPARO
} tipo_forma;


typedef struct stForma forma;

/// @brief: Cria uma fórmula genérica.
/// @param id: Identificador da forma.
/// @param tipo: Tipo da forma.
/// @param dados: Ponteiro para os dados da forma.
/// @return: Retorna um ponteiro para a forma criada.
forma *cria_forma(int id, tipo_forma tipo, void *dados);

/// @brief: Pega e retorna o ID da forma.
/// @param f: Ponteiro para a forma.
/// @return: ID da forma.
int get_id_forma(forma *f);

/// @brief: Pega o tipo da forma e o retorna.
/// @param f: Ponteiro para a forma.
/// @return: Tipo da forma.
tipo_forma get_tipo_forma(forma *f);

/// @brief Define um novo tipo para a forma
/// @param f Ponteiro para a forma
/// @param novo Novo tipo
void set_tipo(forma *f, tipo_forma novo);

/// @brief: Pega e retorna os dados da forma.
/// @param f: Ponteiro para a forma.
/// @return: Ponteiro para os dados da forma.
void *get_dados_forma(forma *f);

/// @brief: Define um novo identificador para a forma.
/// @param f: Ponteiro para a forma.
/// @param novoID: Novo identificador.
void set_id_forma(forma *f, int novoID);

/// @brief: Pega e retorna a cor de borda de uma forma genérica.
/// @param f: Ponteiro para a forma.
/// @return: Cor de borda da forma genérica.
char *getCorbForma(forma *f);

/// @brief: Pega e retorna a cor de preenchimento de uma forma genérica.
/// @param f: Ponteiro para a forma.
/// @return: Cor de preenchimento de uma forma genérica.
char *get_corp_forma(forma *f);

/// @brief: Define uma nova cor de preenchimento para uma forma genérica.
/// @param f: Ponteiro para a forma.
/// @param nova_cor: Nova cor de preenchimento para a forma genérica.
void set_corp_formas(forma *f, char *nova_cor);

/// @brief: Define uma nova cor de borda para uma forma genérica.
/// @param f: Ponteiro para a forma.
/// @param nova_cor: Nova cor de borda da forma genérica.
void set_corb_formas(forma *f, char *nova_cor);

/// @brief: Clona uma forma, acrescentando 1 ao seu ID.
/// @param f_original: Forma que será clonada;
/// @return: Ponteiro para a forma clonada.
forma *clona_forma(forma *f_original);

/// @brief: Calcula a área da forma.
/// @param f: Ponteiro a forma.
/// @return: Retorna a área já calculada da área.
double get_area_forma(forma *f);

/// @brief: Libera toda a forma e a destrói.
/// @param f: Ponteiro para a forma.
void destrutor_forma(forma *f);

/// @brief: Define a posição de uma forma genérica.
/// @param f: Ponteiro para a forma genérica.
/// @param x: Nova coordenada X.
/// @param y: Nova coordenada Y.
void set_posicao_forma(forma *f, double x, double y);

/// @brief: Faz o cálculo para obter a cor complementar de uma cor em hexadecimal.
/// @param cor_hexa_original: A cor que terá sua cor complementar calculada.
/// @return: Retorna um ponteiro para a cor complementar em hexadecimal.
char *getCorComplementar(char *cor_hexa_original);

/// @brief: Troca a cor de preenchimento de f1 com a cor de borda de f2.
/// @param f1: Ponteiro para a forma 1.
/// @param f2: Ponteiro para a forma 2;
void alterna_cores_entre_formas(forma *f1, forma *f2);

/// @brief: Troca a cor de preenchimento da forma com sua cor de borda e vice-versa.
/// @param f: Ponteiro para a forma.
void alterna_cores_forma(forma *f);

/// @brief: Dado o tipo da forma, pega seus dados e a desenha em um arquivo svg.
/// @param f: Ponteiro para a forma.
/// @param svg: Ponteiro para o arquivo svg.
void desenha_forma_svg(forma *f, FILE *svg);

/// @brief: Dado o tipo da forma, pega seus dados e gera um relatório no arquivo txt.
/// @param f: Ponteiro para a forma.
/// @param txt: Arquivo .txt aonde as informações serão escritas.
/// @param reportDaFuncaoQRY: Começo padrão que será adicionado conforme a função do .qry.
void escreveDadosFormaTxt(forma *f, FILE *txt, const char *reportDaFuncaoQRY);

/// @brief: Obtém a coordenada X principal (âncora) de uma forma genérica.
/// @param f: Ponteiro para a forma genérica.
/// @return: O valor da coordenada X. Retorna 0.0 se a forma for nula ou inválida.
double get_x_forma(forma *f);

/// @brief: Obtém a coordenada Y principal (âncora) de uma forma genérica.
/// @param f: Ponteiro para a forma genérica.
/// @return: O valor da coordenada Y. Retorna 0.0 se a forma for nula ou inválida.
double get_y_forma(forma *f);

/// @brief Transforma uma forma genérica já transformada em um anteparo
/// @param f Ponteiro para a forma
/// @param orientacao Orientação da linha do círculo
/// @return Retorna uma lista com o anteparo criado.
lista *forma_anteparo(forma *f, char orientacao);

#endif //FORMAS_H
