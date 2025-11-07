#include <stdio.h>
#include <stdlib.h>
#include <assert.h>
#include "../lista.h"
#include "../formas.h"
#include "../circulo.h"
#include "../retangulo.h"
#include "../linha.h"
#include "../texto.h"

void test_circulo_anteparo() {
    printf("=== Teste Círculo -> Anteparo ===\n");

    circulo *c_dados = criaCirculo(1, 100.0, 100.0, 50.0, "red", "blue");
    forma *circulo = criaForma(1, CIRCULO, c_dados);

    lista *anteparos_h = forma_anteparo(circulo, 'h');
    assert(anteparos_h != NULL);
    assert(get_tam_lista(anteparos_h) == 1);
    printf("✓ Círculo -> 1 anteparo horizontal OK\n");

    forma *anteparo = (forma*)get_head_data(anteparos_h);
    assert(anteparo != NULL);
    assert(getTipoForma(anteparo) == ANTEPARO);
    assert(getIDforma(anteparo) >= 10000);

    free_lista(anteparos_h, (void(*)(void*))destrutorForma);

    lista *anteparos_v = forma_anteparo(circulo, 'v');
    assert(anteparos_v != NULL);
    assert(get_tam_lista(anteparos_v) == 1);
    printf("✓ Círculo -> 1 anteparo vertical OK\n");

    free_lista(anteparos_v, (void(*)(void*))destrutorForma);
    destrutorForma(circulo);
}

void test_retangulo_anteparo() {
    printf("=== Teste Retângulo -> Anteparos ===\n");

    retangulo *r_dados = criaRetangulo(2, 50.0, 50.0, 100.0, 80.0, "green", "yellow");
    forma *retangulo = criaForma(2, RETANGULO, r_dados);

    lista *anteparos = forma_anteparo(retangulo, 'h');
    assert(anteparos != NULL);
    assert(get_tam_lista(anteparos) == 4); // 4 lados
    printf("✓ Retângulo -> 4 anteparos OK\n");

    int ids[4] = {0};
    printf("IDs dos anteparos do retângulo:\n");
    for (int i = 0; i < 4; i++) {
        forma *f = (forma*)get_data_index(anteparos, i);
        assert(f != NULL);
        ids[i] = getIDforma(f);
        printf("  Anteparo %d: ID = %d\n", i, ids[i]);
        assert(getTipoForma(f) == ANTEPARO);
    }

    for (int i = 0; i < 4; i++) {
        for (int j = i + 1; j < 4; j++) {
            if (ids[i] == ids[j]) {
                printf("✗ ERRO: IDs duplicados encontrados: %d (índices %d e %d)\n", ids[i], i, j);
                assert(ids[i] != ids[j]);
            }
        }
    }
    printf("✓ IDs únicos OK\n");

    free_lista(anteparos, (void(*)(void*))destrutorForma);
    destrutorForma(retangulo);
}

void test_linha_anteparo() {
    printf("=== Teste Linha -> Anteparo ===\n");

    linha *l_dados = criaLinha(3, 10.0, 10.0, 200.0, 150.0, "black", false);
    forma *linha_forma = criaForma(3, LINHA, l_dados);

    lista *anteparos = forma_anteparo(linha_forma, 'h');
    assert(anteparos != NULL);
    assert(get_tam_lista(anteparos) == 1);
    printf("✓ Linha -> 1 anteparo OK\n");

    forma *anteparo = (forma*)get_head_data(anteparos);
    assert(anteparo != NULL);
    assert(getTipoForma(anteparo) == ANTEPARO);

    assert(getIDforma(anteparo) != getIDforma(linha_forma));
    printf("✓ Cópia com ID diferente OK\n");

    free_lista(anteparos, (void(*)(void*))destrutorForma);
    destrutorForma(linha_forma);
}

void test_texto_anteparo() {
    printf("=== Teste Texto -> Anteparo ===\n");

    texto *t_dados = criaTexto(4, 100.0, 100.0, "white", "black", 'i', "Hello", NULL);
    forma *texto_forma = criaForma(4, TEXTO, t_dados);

    lista *anteparos = forma_anteparo(texto_forma, 'h');
    assert(anteparos != NULL);
    assert(get_tam_lista(anteparos) == 1);
    printf("✓ Texto -> 1 anteparo OK\n");

    forma *anteparo = (forma*)get_head_data(anteparos);
    assert(anteparo != NULL);
    assert(getTipoForma(anteparo) == ANTEPARO);

    free_lista(anteparos, (void(*)(void*))destrutorForma);
    destrutorForma(texto_forma);
}

void test_multiplas_formas_isoladas() {
    printf("=== Teste Múltiplas Formas (Isoladas) ===\n");


    forma *formas[4];
    int total_anteparos_esperado = 0;

    circulo *c_dados = criaCirculo(10, 50.0, 50.0, 20.0, "red", "blue");
    formas[0] = criaForma(10, CIRCULO, c_dados);
    total_anteparos_esperado += 1;

    retangulo *r_dados = criaRetangulo(20, 0.0, 0.0, 100.0, 50.0, "green", "yellow");
    formas[1] = criaForma(20, RETANGULO, r_dados);
    total_anteparos_esperado += 4;

    linha *l_dados = criaLinha(30, 0.0, 0.0, 100.0, 100.0, "black", false);
    formas[2] = criaForma(30, LINHA, l_dados);
    total_anteparos_esperado += 1;

    texto *t_dados = criaTexto(40, 50.0, 50.0, "white", "black", 'i', "Test", NULL);
    formas[3] = criaForma(40, TEXTO, t_dados);
    total_anteparos_esperado += 1;

    int total_anteparos_gerados = 0;

    for (int i = 0; i < 4; i++) {
        lista *anteparos = forma_anteparo(formas[i], 'h');
        assert(anteparos != NULL);

        int qtd_anteparos = get_tam_lista(anteparos);
        total_anteparos_gerados += qtd_anteparos;

        for (int j = 0; j < qtd_anteparos; j++) {
            forma *f = (forma*)get_data_index(anteparos, j);
            assert(getTipoForma(f) == ANTEPARO);
        }

        free_lista(anteparos, (void(*)(void*))destrutorForma);
        destrutorForma(formas[i]);
    }

    assert(total_anteparos_gerados == total_anteparos_esperado);
    printf("✓ Total de anteparos gerados: %d (esperado: %d)\n",
           total_anteparos_gerados, total_anteparos_esperado);
}

int main() {
    printf("INICIANDO TESTES DA FUNÇÃO FORMA_ANTEPARO\n\n");

    test_circulo_anteparo();
    test_retangulo_anteparo();
    test_linha_anteparo();
    test_texto_anteparo();
    test_multiplas_formas_isoladas();

    printf("\n========================================\n");
    printf("✓ TODOS OS TESTES PASSARAM!\n");
    printf("========================================\n");

    return 0;
}