#include "svg.h"

#include <stdio.h>
#include "formas.h"

FILE* inicializaSvg(const char* caminhoArquivo) {
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

void insereCirculo(FILE *svg, circulo *c) {
	fprintf(svg, " <circle id=\"%i\" cx=\"%lf\" cy=\"%lf\" r=\"%lf\" stroke=\"%s\" fill=\"%s\" opacity=\"%lf\" stroke-width=\"%lf\" />\n",
	getIDCirculo(c),
	getXCirculo(c),
	getYCirculo(c),
	getRaioCirculo(c),
	getCorbCirculo(c),
	getCorpCirculo(c), 0.5, 1.5);
}

void insereRetangulo(FILE *svg, retangulo *r) {
	fprintf(svg, "\t<rect id=\"%d\" x=\"%lf\" y=\"%lf\" width=\"%lf\" height=\"%lf\" stroke=\"%s\" fill=\"%s\" opacity=\"%lf\" stroke-width=\"%lf\" />\n",
		getIDretangulo(r),
		getXretangulo(r),
		getYretangulo(r),
		getLarguraRetangulo(r),
		getAlturaRetangulo(r),
		getCorbRetangulo(r),
		getCorpRetangulo(r), 0.5, 1.5);
}

void insereLinha(FILE *svg, linha *l) {
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

void insereTexto(FILE *svg, texto *t) {
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

void insere_bounding_box(FILE *svg, poligono *p) {
	if (p == NULL || svg == NULL) return;

	double xMin, xMax, yMin, yMax;

	get_bounding_box(p, &xMin, &xMax, &yMin, &yMax);

	double largura = xMax - xMin;
	double altura = yMax - yMin;

	fprintf(svg, "\t<rect x=\"%lf\" y=\"%lf\" width=\"%lf\" height=\"%lf\" "
				 "fill=\"none\" stroke=\"red\" stroke-dasharray=\"5,5\" stroke-width=\"1.5\" />\n", xMin, yMin, largura, altura);
}

void fechaSVG(FILE *svg) {
	if (svg == NULL) return;

	fprintf(svg, "</g>\n");

	fprintf(svg, "</svg>\n");

	fclose(svg);
}

void acao_desenhar(void* item, void* aux) {
	FILE* arquivo_svg = (FILE*)aux;
	desenhaFormaSvg((forma*)item, arquivo_svg);
}

void gerar_arquivo_svg(const char *nome_svg, lista *lista_formas) {
	if (lista_formas == NULL) {
		fprintf(stderr, "Aviso: filaDeFormas NULL em gerarArquivoSvg(%s)\n", nome_svg);
		return;
	}

	FILE *arquivo_svg = inicializaSvg(nome_svg);
	if (arquivo_svg == NULL) {
		printf("Erro ao abrir o arquivo svg!\n");
		return;
	}

	node *no_atual = get_head_node(lista_formas);

	while (no_atual != NULL) {
		forma *f = get_node_data(no_atual);

		desenhaFormaSvg(f, arquivo_svg);

		no_atual = go_next_node(no_atual);
	}

	fechaSVG(arquivo_svg);

	printf("Arquivo .svg gerado com sucesso!\n");

}
