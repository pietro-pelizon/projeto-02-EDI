#include <stdio.h>
#include <stdlib.h>
#include <stdbool.h>

#include "../sobreposicao.h"
#include "../formas.h"
#include "../poligono.h"
#include "../circulo.h"
#include "../retangulo.h"
#include "../linha.h"
#include "../texto.h"

int g_falhas_sobreposicao = 0;

void print_teste_status(bool sucesso, const char* nome_teste) {
    if (sucesso) {
        printf("[  OK  ] %s\n", nome_teste);
    } else {
        printf("[FALHA] %s\n", nome_teste);
        g_falhas_sobreposicao++;
    }
}

poligono* criar_poligono_alvo() {
    poligono* p = init_poligono();
    insert_vertice(p, 10.0, 10.0);
    insert_vertice(p, 20.0, 10.0);
    insert_vertice(p, 20.0, 20.0);
    insert_vertice(p, 10.0, 20.0);
    return p;
}

void testar_circulo_vs_poligono(poligono* p_alvo) {
    printf("--- Testando Círculo vs. Polígono ---\n");

    circulo* c_dentro = criaCirculo(1, 15, 15, 2, "blue", "blue");
    forma* f_c_dentro = criaForma(1, CIRCULO, c_dentro);
    print_teste_status(forma_sobrepoe_poligono(f_c_dentro, p_alvo) == true, "Círculo DENTRO");

    circulo* c_fora = criaCirculo(2, 100, 100, 5, "red", "red");
    forma* f_c_fora = criaForma(2, CIRCULO, c_fora);
    print_teste_status(forma_sobrepoe_poligono(f_c_fora, p_alvo) == false, "Círculo FORA");

    circulo* c_intersecta = criaCirculo(3, 21, 15, 2, "green", "green");
    forma* f_c_intersecta = criaForma(3, CIRCULO, c_intersecta);
    print_teste_status(forma_sobrepoe_poligono(f_c_intersecta, p_alvo) == true, "Círculo INTERSECTA");

    circulo* c_envolve = criaCirculo(4, 15, 15, 100, "yellow", "yellow");
    forma* f_c_envolve = criaForma(4, CIRCULO, c_envolve);
    print_teste_status(forma_sobrepoe_poligono(f_c_envolve, p_alvo) == true, "Círculo ENVOLVE");

    destrutorForma(f_c_dentro);
    destrutorForma(f_c_fora);
    destrutorForma(f_c_intersecta);
    destrutorForma(f_c_envolve);
}

void testar_retangulo_vs_poligono(poligono* p_alvo) {
    printf("--- Testando Retângulo vs. Polígono ---\n");

    retangulo* r_dentro = criaRetangulo(5, 12, 12, 2, 2, "blue", "blue");
    forma* f_r_dentro = criaForma(5, RETANGULO, r_dentro);
    print_teste_status(forma_sobrepoe_poligono(f_r_dentro, p_alvo) == true, "Retângulo DENTRO");

    retangulo* r_fora = criaRetangulo(6, 50, 50, 5, 5, "red", "red");
    forma* f_r_fora = criaForma(6, RETANGULO, r_fora);
    print_teste_status(forma_sobrepoe_poligono(f_r_fora, p_alvo) == false, "Retângulo FORA");

    retangulo* r_intersecta = criaRetangulo(7, 18, 15, 4, 2, "green", "green");
    forma* f_r_intersecta = criaForma(7, RETANGULO, r_intersecta);
    print_teste_status(forma_sobrepoe_poligono(f_r_intersecta, p_alvo) == true, "Retângulo INTERSECTA");

    destrutorForma(f_r_dentro);
    destrutorForma(f_r_fora);
    destrutorForma(f_r_intersecta);
}

void testar_linha_vs_poligono(poligono* p_alvo) {
    printf("--- Testando Linha vs. Polígono ---\n");

    linha* l_dentro = criaLinha(8, 12, 12, 18, 18, "blue", false);
    forma* f_l_dentro = criaForma(8, LINHA, l_dentro);
    print_teste_status(forma_sobrepoe_poligono(f_l_dentro, p_alvo) == true, "Linha DENTRO");

    linha* l_fora = criaLinha(9, 100, 100, 120, 120, "red", false);
    forma* f_l_fora = criaForma(9, LINHA, l_fora);
    print_teste_status(forma_sobrepoe_poligono(f_l_fora, p_alvo) == false, "Linha FORA");

    linha* l_intersecta = criaLinha(10, 5, 15, 25, 15, "green", false);
    forma* f_l_intersecta = criaForma(10, LINHA, l_intersecta);
    print_teste_status(forma_sobrepoe_poligono(f_l_intersecta, p_alvo) == true, "Linha INTERSECTA");

    destrutorForma(f_l_dentro);
    destrutorForma(f_l_fora);
    destrutorForma(f_l_intersecta);
}

void testar_texto_vs_poligono(poligono* p_alvo) {
    printf("--- Testando Texto vs. Polígono ---\n");
    estilo* est = criaEstilo("sans", "b", "10");

    texto* t_fora = criaTexto(11, 100, 100, "red", "red", 'i', "fora", est);
    forma* f_t_fora = criaForma(11, TEXTO, t_fora);
    print_teste_status(forma_sobrepoe_poligono(f_t_fora, p_alvo) == false, "Texto FORA");

    texto* t_intersecta = criaTexto(12, 5, 15, "green", "green", 'i', "abcde", est);
    forma* f_t_intersecta = criaForma(12, TEXTO, t_intersecta);
    print_teste_status(forma_sobrepoe_poligono(f_t_intersecta, p_alvo) == true, "Texto INTERSECTA");

    destrutorForma(f_t_fora);
    destrutorForma(f_t_intersecta);
    destroiEstilo(est);}


int main() {
    printf("--- Iniciando Testes do TAD Sobreposição ---\n");

    poligono* p_alvo = criar_poligono_alvo();

    testar_circulo_vs_poligono(p_alvo);
    testar_retangulo_vs_poligono(p_alvo);
    testar_linha_vs_poligono(p_alvo);
    testar_texto_vs_poligono(p_alvo);

    free_poligono(p_alvo);

    printf("\n--- Testes de Sobreposição Concluídos ---\n");
    if (g_falhas_sobreposicao == 0) {
        printf(">>> SUCESSO: Todos os testes de sobreposição passaram!\n");
    } else {
        printf(">>> FALHA: %d teste(s) de sobreposição falharam.\n", g_falhas_sobreposicao);
    }

    return g_falhas_sobreposicao;
}