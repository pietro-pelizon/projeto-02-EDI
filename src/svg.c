#include "svg.h"

#include <stdio.h>
#include "formas.h"

FILE* inicializa_svg(const char* caminhoArquivo) {
	FILE* svg = fopen(caminhoArquivo, "w");
	if (svg == NULL) {
		perror("ERRO ao abrir o arquivo SVG");
		return NULL;
	}

	fprintf(svg, "<?xml version=\"1.0\" encoding=\"UTF-8\" standalone=\"no\"?>\n");
	fprintf(svg, "<svg xmlns:svg=\"http://www.w3.org/2000/svg\" xmlns=\"http://www.w3.org/2000/svg\" width=\"1000\" height=\"1000\">\n");

	fprintf(svg, "<g>\n");

	return svg;
}

void insere_circulo(FILE *svg, circulo *c) {
	fprintf(svg, " <circle id=\"%i\" cx=\"%lf\" cy=\"%lf\" r=\"%lf\" stroke=\"%s\" fill=\"%s\" opacity=\"%lf\" stroke-width=\"%lf\" />\n",
	getIDCirculo(c),
	getXCirculo(c),
	getYCirculo(c),
	getRaioCirculo(c),
	getCorbCirculo(c),
	getCorpCirculo(c), 0.5, 1.5);
}

void insere_retangulo(FILE *svg, retangulo *r) {
	fprintf(svg, "\t<rect id=\"%d\" x=\"%lf\" y=\"%lf\" width=\"%lf\" height=\"%lf\" stroke=\"%s\" fill=\"%s\" opacity=\"%lf\" stroke-width=\"%lf\" />\n",
		getIDretangulo(r),
		getXretangulo(r),
		getYretangulo(r),
		getLarguraRetangulo(r),
		getAlturaRetangulo(r),
		getCorbRetangulo(r),
		getCorpRetangulo(r), 0.5, 1.5);
}

void insere_linha(FILE *svg, linha *l) {
	if (getEh_pontilhada(l)) {
		fprintf(svg, "\t<line id=\"%d\" x1=\"%lf\" y1=\"%lf\" x2=\"%lf\" y2=\"%lf\" stroke=\"%s\" stroke-dasharray=\"1, 1\" stroke-width=\"%lf\" />\n",
			getIDLinha(l),
			getX1Linha(l),
			getY1Linha(l),
			getX2Linha(l),
			getY2Linha(l),
			getCorLinha(l), 1.5);
	}

	else {
		fprintf(svg, "\t<line id=\"%d\" x1=\"%lf\" y1=\"%lf\" x2=\"%lf\" y2=\"%lf\" stroke=\"%s\" stroke-width=\"%lf\" />\n",
			getIDLinha(l),
			getX1Linha(l),
			getY1Linha(l),
			getX2Linha(l),
			getY2Linha(l),
			getCorLinha(l), 1.5);
	}
}

void insere_texto(FILE *svg, texto *t) {
	if (t == NULL) return;

	estilo* e = getEstiloTexto(t);
	if (e == NULL) return;

	fprintf(svg, "\t<text id=\"%d\" x=\"%lf\" y=\"%lf\" stroke=\"%s\" fill=\"%s\" font-family=\"%s\" font-weight=\"%s\" font-size=\"%spt\" ",
		getIDTexto(t),
		getXTexto(t),
		getYTexto(t),
		getCorbTexto(t),
		getCorpTexto(t),
		getfFamily(e),
		getfWeight(e),
		getfSize(e));

	char ancora = getATexto(t);
	switch (ancora) {
		case 'm':
			fprintf(svg, "text-anchor=\"middle\"");
			break;
		case 'f':
			fprintf(svg, "text-anchor=\"end\"");
			break;
		case 'i':
		default:
			fprintf(svg, "text-anchor=\"start\"");
			break;
	}

	fprintf(svg, ">%s</text>\n", getTxtoTexto(t));
}

void insere_poligono_visibilidade(FILE *svg, poligono *p) {
	if (p == NULL || svg == NULL) return;

	int n = get_num_vertices(p);
	if (n < 3) return;

	fprintf(svg, "\t<polygon points=\"");

	for (int i = 0; i < n; i++) {
		ponto *v = get_vertice(p, i);
		fprintf(svg, "%.4lf,%.4lf ", get_x_ponto(v), get_y_ponto(v));
		free_ponto(v);
	}

	fprintf(svg, "\" fill=\"#FF0000\" fill-opacity=\"0.2\" stroke=\"#FF0000\" stroke-width=\"2\" />\n");
}

void insere_anteparo(FILE *svg, anteparo *a) {
	if (svg == NULL || a == NULL) return;

	double x0, y0, x1, y1;
	int id = get_id_anteparo(a);
	char *cor = get_cor_anteparo(a);
	x0 = get_x_p0(a); y0 = get_y_p0(a);
	x1 = get_x_p1(a); y1 = get_y_p1(a);

	fprintf(svg, "\t<line id=\"%d\" x1=\"%lf\" y1=\"%lf\" x2=\"%lf\" y2=\"%lf\" stroke=\"%s\" stroke-width=\"%lf\" />\n",
		id, x0, y0, x1, y1, cor, 1.5);
}

void insere_bounding_box(FILE *svg, poligono *p) {
	if (p == NULL || svg == NULL) return;

	double xMin, xMax, yMin, yMax;

	get_bounding_box(p, &xMin, &xMax, &yMin, &yMax);

	double largura = xMax - xMin;
	double altura = yMax - yMin;

	fprintf(svg, "\t<rect x=\"%lf\" y=\"%lf\" width=\"%lf\" height=\"%lf\" "
				 "fill=\"none\" stroke=\"black\" stroke-dasharray=\"3, 3\" stroke-width=\"2.0\" />\n", xMin, yMin, largura, altura);
}

void fecha_svg(FILE *svg) {
	if (svg == NULL) return;

	fprintf(svg, "</g>\n");

	fprintf(svg, "</svg>\n");

	fclose(svg);
}

void insere_bomba_svg(FILE *svg, double x, double y) {
	if (svg == NULL) return;

	fprintf(svg, "<circle cx=\"%lf\" cy=\"%lf\" r=\"1\" stroke-opacity=\"0.5\" fill=\"none\""
			  " stroke=\" #FF0000\" stroke-width=\"2\" stroke-dasharray=\"1, 1\" />\n", x, y);

	fprintf(svg, "<circle cx=\"%lf\" cy=\"%lf\" r=\"3\" stroke-opacity=\"0.5\" fill=\"none\""
			  " stroke=\" #FF0000\" stroke-width=\"2\" stroke-dasharray=\"1, 1\" />\n", x, y);

	fprintf(svg, "<circle cx=\"%lf\" cy=\"%lf\" r=\"2\" stroke-opacity=\"0.5\" fill=\"none\""
			  " stroke=\"#FF00FF\" stroke-width=\"2\" stroke-dasharray=\"1, 1\" />\n", x, y);
}

void acao_desenhar(void* item, void* aux) {
	FILE* arquivo_svg = (FILE*)aux;
	desenha_forma_svg((forma*)item, arquivo_svg);
}

void desenhar_formas_no_svg(FILE *svg, lista *formas) {
	if (svg == NULL || formas == NULL) return;

	node *atual = get_head_node(formas);

	while (atual != NULL) {
		forma *f = get_node_data(atual);
		desenha_forma_svg(f, svg);

		atual = go_next_node(atual);
	}
}

FILE *gerar_arquivo_svg(const char *nome_svg, lista *lista_formas) {
	if (lista_formas == NULL) {
		fprintf(stderr, "Aviso: lista de formas NULL em 'gerar_arquivo_svg'(%s)\n", nome_svg);
		return NULL;
	}

	FILE *arquivo_svg = inicializa_svg(nome_svg);
	if (arquivo_svg == NULL) {
		printf("Erro ao abrir o arquivo svg!\n");
		return NULL;
	}

	node *no_atual = get_head_node(lista_formas);

	while (no_atual != NULL) {
		forma *f = get_node_data(no_atual);

		desenha_forma_svg(f, arquivo_svg);

		no_atual = go_next_node(no_atual);
	}


	return arquivo_svg;

}
