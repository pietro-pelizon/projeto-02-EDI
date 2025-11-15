#include <stdio.h>
#include <math.h>
#include <assert.h>

#include "../geometria.h"
#include "../ponto.h"
#include "../anteparo.h"

#define EPSILON 1e-10
#define PI 3.14159265358

int main() {
    printf("Iniciando testes do módulo geometria...\n");

    // Teste 1: distancia_quadrada
    assert(fabs(distancia_quadrada(0, 0, 3, 4) - 25.0) < EPSILON);
    assert(fabs(distancia_quadrada(-1, -1, 2, 3) - 25.0) < EPSILON);
    assert(fabs(distancia_quadrada(5, 5, 5, 5) - 0.0) < EPSILON);

    // Teste 2: calcula_angulo
    ponto *obs = init_ponto(0, 0);
    ponto *p_dir = init_ponto(1, 0);
    ponto *p_cima = init_ponto(0, 1);
    ponto *p_esq = init_ponto(-1, 0);
    ponto *p_diag = init_ponto(1, 1);

    assert(fabs(calcula_angulo(obs, p_dir) - 0.0) < EPSILON);
    assert(fabs(calcula_angulo(obs, p_cima) - PI / 2.0) < EPSILON);
    assert(fabs(calcula_angulo(obs, p_esq) - PI) < EPSILON);
    assert(fabs(calcula_angulo(obs, p_diag) - PI / 4.0) < EPSILON);

    free_ponto(obs);
    free_ponto(p_dir);
    free_ponto(p_cima);
    free_ponto(p_esq);
    free_ponto(p_diag);

    // Teste 3: produto_vetorial
    assert(produto_vetorial(0, 0, 5, 0, 5, 5) == 1);
    assert(produto_vetorial(0, 0, 5, 0, 5, -5) == 2);
    assert(produto_vetorial(0, 0, 5, 0, 10, 0) == 0);

    // Teste 4: is_ponto_no_segmento
    assert(is_ponto_no_segmento(0, 0, 5, 5, 10, 10) == true);
    assert(is_ponto_no_segmento(0, 0, 11, 11, 10, 10) == false);
    assert(is_ponto_no_segmento(0, 0, -1, -1, 10, 10) == false);
    assert(is_ponto_no_segmento(0, 0, 5, 6, 10, 10) == true);

    // Teste 5: calcular_distancia_ponto_segmento
    ponto *p_meio = init_ponto(5, 3);
    ponto *p_antes = init_ponto(-2, 3);
    ponto *p_depois = init_ponto(12, 4);
    anteparo *a_horiz = init_anteparo(0, 0, 0, 10, 0, "red");

    assert(fabs(calcular_distancia_ponto_segmento(p_meio, a_horiz) - 3.0) < EPSILON);
    assert(fabs(calcular_distancia_ponto_segmento(p_antes, a_horiz) - sqrt(13.0)) < EPSILON);
    assert(fabs(calcular_distancia_ponto_segmento(p_depois, a_horiz) - sqrt(20.0)) < EPSILON);

    free_ponto(p_meio);
    free_ponto(p_antes);
    free_ponto(p_depois);
    free_anteparo(a_horiz);

    printf("... testes do módulo geometria concluídos (sem erros).\n");
    return 0;
}