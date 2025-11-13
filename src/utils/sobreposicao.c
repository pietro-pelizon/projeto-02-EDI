#include "sobreposicao.h"
#include "circulo.h"
#include "retangulo.h"
#include "linha.h"
#include "texto.h"
#include "formas.h"
#include "poligono.h"
#include "geometria.h"

#include <math.h>
#define EPSILON 1e-10

static bool sobrepoe_circulo_poligono(circulo *c, poligono *p);
static bool sobrepoe_retangulo_poligono(retangulo *r, poligono *p);
static bool sobrepoe_linha_poligono(linha *l, poligono *p);
static bool sobrepoe_texto_poligono(texto *t, poligono *p);

bool forma_sobrepoe_poligono(forma *f, poligono *p) {
    if (f == NULL || p == NULL) return false;

    tipo_forma tipo = get_tipo_forma(f);
    void *dados_forma = get_dados_forma(f);

    switch (tipo) {
        case CIRCULO: return sobrepoe_circulo_poligono((circulo*)dados_forma, p);
        case RETANGULO: return sobrepoe_retangulo_poligono((retangulo*)dados_forma, p);
        case LINHA: return sobrepoe_linha_poligono((linha*)dados_forma, p);
        case TEXTO: return sobrepoe_texto_poligono((texto*)dados_forma, p);
        case ANTEPARO: printf("Cálculo de sobreposição entre anteparos não faz sentido para a lógica do projeto!\n");
        default: return false;
    }
}

static bool ponto_dentro_retangulo(retangulo *r, ponto *pt) {
    double rx = getXretangulo(r);
    double ry = getYretangulo(r);
    double rw = getLarguraRetangulo(r);
    double rh = getAlturaRetangulo(r);

    double px = get_x_ponto(pt);
    double py = get_y_ponto(pt);

    return (px >= rx) && (px <= rx + rw) &&
           (py >= ry) && (py <= ry + rh);
}

static bool ponto_dentro_circulo(circulo *c, ponto *pt) {
    double cx = getXCirculo(c);
    double cy =getYCirculo(c);
    double r = getRaioCirculo(c);

    double px = get_x_ponto(pt);
    double py = get_y_ponto(pt);

    return distancia_quadrada(cx, cy, px, py) <= (r * r);
}

static bool sobrepoe_circulo_linha_texto(circulo *c, linha *l) {
    double cx = getXCirculo(c);
    double cy = getYCirculo(c);
    double cr = getRaioCirculo(c);

    double lx1 = getX1Linha(l);
    double lx2 = getX2Linha(l);
    double ly1 = getY1Linha(l);
    double ly2 = getY2Linha(l);

    double distP1Linha = distancia_quadrada(cx, cy, lx1, ly1);
    double distP2Linha = distancia_quadrada(cx, cy, lx2, ly2);
    double raioAoQuadrado = cr * cr;

    if (distP1Linha <= raioAoQuadrado || distP2Linha <= raioAoQuadrado) {
        return true;
    }

    double comprimentoAoQuadrado = distancia_quadrada(lx1, ly1, lx2, ly2);

    if (comprimentoAoQuadrado == 0.0) {
        return distP1Linha <= raioAoQuadrado;
    }

    double t = ((cx - lx1) * (lx2 - lx1) + (cy - ly1) * (ly2 - ly1)) / comprimentoAoQuadrado;

    t = fmax(0.0, fmin(1.0, t));

    double px = lx1 + t * (lx2 - lx1);
    double py = ly1 + t * (ly2 - ly1);
    double distanciaAoQuadradoDoCirculo = distancia_quadrada(cx, cy, px, py);

    return distanciaAoQuadradoDoCirculo <= raioAoQuadrado;

}

static bool sobrepoe_linha_linha(linha *l1, linha *l2) {
    double l1x1 = getX1Linha(l1);
    double l1y1 = getY1Linha(l1);
    double l1x2 = getX2Linha(l1);
    double l1y2 = getY2Linha(l1);

    double l2x1 = getX1Linha(l2);
    double l2y1 = getY1Linha(l2);
    double l2x2 = getX2Linha(l2);
    double l2y2 = getY2Linha(l2);

    int o1 = produto_vetorial(l1x1, l1y1, l1x2, l1y2, l2x1, l2y1);
    int o2 = produto_vetorial(l1x1, l1y1, l1x2, l1y2, l2x2, l2y2);
    int o3 = produto_vetorial(l2x1, l2y1, l2x2, l2y2, l1x1, l1y1);
    int o4 = produto_vetorial(l2x1, l2y1, l2x2, l2y2, l1x2, l1y2);

    if (o1 != o2 && o3 != o4) {
        return true;
    }

    if (o1 == 0 && is_ponto_no_segmento(l1x1, l1y1, l2x1, l2y1, l1x2, l1y2)) return true;
    if (o2 == 0 && is_ponto_no_segmento(l1x1, l1y1, l2x2, l2y2, l1x2, l1y2)) return true;
    if (o3 == 0 && is_ponto_no_segmento(l2x1, l2y1, l1x1, l1y1, l2x2, l2y2)) return true;
    if (o4 == 0 && is_ponto_no_segmento(l2x1, l2y1, l1x2, l1y2, l2x2, l2y2)) return true;

    return false;

}

static bool sobrepoe_retangulo_linha(retangulo *r, linha *l) {
    double lx1 = getX1Linha(l);
    double ly1 = getY1Linha(l);
    double lx2 = getX2Linha(l);
    double ly2 = getY2Linha(l);

    double rx = getXretangulo(r);
    double ry = getYretangulo(r);
    double rw = getLarguraRetangulo(r);
    double rh = getAlturaRetangulo(r);

    double x_min = rx;
    double x_max = rx + rw;
    double y_min = ry;
    double y_max = ry + rh;

    bool p1_dentro = (lx1 >= x_min - EPSILON && lx1 <= x_max + EPSILON &&
                      ly1 >= y_min - EPSILON && ly1 <= y_max + EPSILON);
    bool p2_dentro = (lx2 >= x_min - EPSILON && lx2 <= x_max + EPSILON &&
                      ly2 >= y_min - EPSILON && ly2 <= y_max + EPSILON);


    if (p1_dentro || p2_dentro) {
        return true;
    }


    linha *borda_cima = criaLinha(-1, x_min, y_min, x_max, y_min, "temp", false);
    linha *borda_direita = criaLinha(-1, x_max, y_min, x_max, y_max, "temp", false);
    linha *borda_baixo = criaLinha(-1, x_max, y_max, x_min, y_max, "temp", false);
    linha *borda_esquerda = criaLinha(-1, x_min, y_max, x_min, y_min, "temp", false);


    bool resultado = (sobrepoe_linha_linha(l, borda_cima) ||
                     sobrepoe_linha_linha(l, borda_direita) ||
                     sobrepoe_linha_linha(l, borda_baixo) ||
                     sobrepoe_linha_linha(l, borda_esquerda));


    destrutorLinha(borda_cima);
    destrutorLinha(borda_direita);
    destrutorLinha(borda_baixo);
    destrutorLinha(borda_esquerda);

    return resultado;
}

static bool sobrepoe_retangulo_poligono(retangulo *r, poligono *p) {
    double x = getXretangulo(r);
    double y = getYretangulo(r);
    double w = getLarguraRetangulo(r);
    double h = getAlturaRetangulo(r);

    if (is_inside(p, x, y) || is_inside(p, x + w, y) || is_inside(p, x + w, y +h) || is_inside(p, x, y +h)) {
        return true;
    }

    int n_vertices = get_num_vertices(p);
    for (int i = 0; i < n_vertices; i++) {
        ponto *v = get_vertice(p, i);

        bool inside = ponto_dentro_retangulo(r, v);
        free_ponto(v);

        if (inside) {
            return true;
        }
    }

    lista *bordas_poligonos = get_segmentos(p);
    node *no_borda = get_head_node(bordas_poligonos);
    bool interseccao = false;

    while (no_borda != NULL) {
        linha *borda = get_node_data(no_borda);

        if (sobrepoe_retangulo_linha(r, borda)) {
            interseccao = true;
            break;
        }

        no_borda = go_next_node(no_borda);
    }

    free_lista(bordas_poligonos, (void (*)(void *))destrutorLinha);
    return interseccao;
}

static bool sobrepoe_circulo_poligono(circulo *c, poligono *p) {
    double cx = getXCirculo(c);
    double cy = getYCirculo(c);

    if (is_inside(p, cx, cy)) {
        return true;
    }

    int n_vertices = get_num_vertices(p);
    for (int i = 0; i < n_vertices; i++) {
        ponto *v = get_vertice(p, i);

        bool inside = ponto_dentro_circulo(c, v);
        free_ponto(v);

        if (inside) {
            return true;
        }
    }


    lista *bordas_poligono = get_segmentos(p);
    node *no_borda = get_head_node(bordas_poligono);
    bool intersecao = false;

    while (no_borda != NULL) {
        linha *borda = (linha*) get_node_data(no_borda);

        if (sobrepoe_circulo_linha_texto(c, borda)) {
            intersecao = true;
            break;
        }
        no_borda = go_next_node(no_borda);
    }

    free_lista(bordas_poligono, (void (*)(void *))destrutorLinha);

    return intersecao;
}

static bool sobrepoe_linha_poligono(linha *l, poligono *p) {
    if (is_inside(p, getX1Linha(l), getY1Linha(l))) {
        return true;
    }
    if (is_inside(p, getX2Linha(l), getY2Linha(l))) {
        return true;
    }

    lista *bordas_poligono = get_segmentos(p);
    node *no_borda = get_head_node(bordas_poligono);
    bool intersecao = false;

    while (no_borda != NULL) {
        linha *borda = (linha*) get_node_data(no_borda);

        if (sobrepoe_linha_linha(l, borda)) {
            intersecao = true;
            break;
        }
        no_borda = go_next_node(no_borda);
    }

    free_lista(bordas_poligono, (void (*)(void *))destrutorLinha);
    return intersecao;
}

static bool sobrepoe_texto_poligono(texto *t, poligono *p) {
    linha *temp_linha = converter_texto_para_anteparo(t);

    bool resultado = sobrepoe_linha_poligono(temp_linha, p);

    destrutorLinha(temp_linha);
    return resultado;
}