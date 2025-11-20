#include <stdio.h>

#include "lista.h"
#include "formas.h"
#include "circulo.h"
#include <stdlib.h>
#include <string.h>
#include "retangulo.h"
#include "linha.h"
#include "svg.h"
#include "texto.h"

typedef struct stForma {
	int id;
	tipo_forma tipo;
	void *dados;
}forma;

forma *cria_forma(int id, tipo_forma tipo, void *dados) {
	forma *f = malloc (sizeof(forma));
	if (f == NULL) {
		printf("Erro ao criar a forma!\n");
		return NULL;
	}

	f -> id = id;
	f -> tipo = tipo;
	f -> dados = dados;

	return f;
}

int get_id_forma(forma *f) {
	return f -> id;
}

tipo_forma get_tipo_forma(forma *f) {
	return f -> tipo;
}

void *get_dados_forma(forma *f) {
	return f -> dados;
}

void set_id_forma(forma *f, int novoID) {
	f -> id = novoID;
}

void set_tipo(forma *f, tipo_forma novo) {
	f -> tipo = novo;
}

char *get_corb_forma(forma *f) {
	if (f == NULL) return NULL;
	tipo_forma tipo = get_tipo_forma(f);
	void *dados = get_dados_forma(f);

	switch (tipo) {
		case CIRCULO: return getCorbCirculo(dados);
		case RETANGULO: return getCorbRetangulo(dados);
		case LINHA: return getCorLinha(dados);
		case ANTEPARO: return get_cor_anteparo(dados);
		case TEXTO: return getCorbTexto(dados);
		default: return NULL;
	}
}

char *get_corp_forma(forma *f) {
	if (f == NULL) return NULL;

	tipo_forma tipo = get_tipo_forma(f);
	void *dados = get_dados_forma(f);

	switch (tipo) {
		case CIRCULO: return getCorpCirculo(dados);
		case RETANGULO: return getCorpRetangulo(dados);
		case LINHA: printf("Linha não tem cor de preenchimento!\n"); break;
		case ANTEPARO: printf("Anteparo não tem cor de preenchimento!\n"); break;
		case TEXTO: return getCorpTexto(dados);
		default: return NULL;

	}

	return NULL;
}

void set_corp_formas(forma *f, char *nova_cor) {
	if (f == NULL || nova_cor == NULL) return;

	tipo_forma tipo = get_tipo_forma(f);
	void *dados = get_dados_forma(f);

	switch (tipo) {
		case CIRCULO: setCorpCirculo(dados, nova_cor); break;
		case RETANGULO: setCorpRetangulo(dados, nova_cor); break;
		case LINHA: printf("Linha não tem cor de preenchimento, nada a alterar!\n"); break;
		case ANTEPARO: printf("Anteparo não tem cor de preenchimento, nada a alterar!\n"); break;
		case TEXTO: setCorpTexto(dados, nova_cor); break;
		default: break;
	}
}

void set_corb_formas(forma *f, char *nova_cor) {
	if (f == NULL || nova_cor == NULL) return;

	tipo_forma tipo = get_tipo_forma(f);
	void *dados = get_dados_forma(f);

	switch (tipo) {
		case CIRCULO: setCorbCirculo(dados, nova_cor); break;
		case RETANGULO: setCorbRetangulo(dados, nova_cor); break;
		case LINHA: setCorLinha(dados, nova_cor); break;
		case ANTEPARO: set_cor_anteparo(dados, nova_cor); break;
		case TEXTO: setCorbTexto(dados, nova_cor); break;
		default: break;
	}
}

forma *clona_forma(forma *f_original) {
	if (f_original == NULL) return NULL;

	static int max_id = 10000;
	int novo_id = ++max_id;

	tipo_forma tipo = get_tipo_forma(f_original);
	void *dados_orig = get_dados_forma(f_original);
	void *novos_dados = NULL;

	switch (tipo) {
		case CIRCULO: {
			circulo *c = (circulo*)dados_orig;
			novos_dados = criaCirculo(novo_id,
				getXCirculo(c),
				getYCirculo(c),
				getRaioCirculo(c),
				getCorbCirculo(c),
				getCorpCirculo(c));
			break;
		}

		case RETANGULO: {
			retangulo *r = (retangulo*)dados_orig;
			novos_dados = criaRetangulo(novo_id,
				getXretangulo(r),
				getYretangulo(r),
				getLarguraRetangulo(r),
				getAlturaRetangulo(r),
				getCorbRetangulo(r),
				getCorpRetangulo(r));
			break;
		}

		case LINHA: {
			linha *l = (linha*)dados_orig;
			novos_dados = criaLinha(novo_id,
				getX1Linha(l),
				getY1Linha(l),
				getX2Linha(l),
				getY2Linha(l),
				getCorLinha(l),
				getEh_pontilhada(l));
			break;
		}
		case TEXTO: {
			texto *t = (texto*)dados_orig;
			estilo *e_orig = getEstiloTexto(t);
			estilo *novo = criaCopiaEstilo(e_orig);
			novos_dados = criaTexto(novo_id,
				getXTexto(t),
				getYTexto(t),
				getCorbTexto(t),
				getCorpTexto(t),
				getATexto(t),
				getTxtoTexto(t),
				novo);
			break;
		}
		case ANTEPARO: {
			anteparo *a = dados_orig;
			ponto *p0 = get_p0_anteparo(a);
			ponto *p1 = get_p1_anteparo(a);
			novos_dados = init_anteparo(novo_id,
				get_x_ponto(p0),
				get_y_ponto(p0),
				get_x_ponto(p1),
				get_y_ponto(p1),
				get_cor_anteparo(a));
			break;
		}

		default:
			return NULL;
	}

	if (novos_dados != NULL) {
		return cria_forma(novo_id, tipo, novos_dados);
	}
	return NULL;
}

void alterna_cores_forma(forma *f) {
	if (f == NULL) return;


	char *cor_borda = get_corb_forma(f);
	char *cor_preenchimento = get_corp_forma(f);

	if (cor_borda == NULL || cor_preenchimento == NULL) return;

	char *copia_preenchimento = malloc(strlen(cor_preenchimento) + 1);
	if (copia_preenchimento == NULL) {
		fprintf(stderr, "Erro de memoria na funcao alternaCores!\n");
		return;
	}
	strcpy(copia_preenchimento, cor_preenchimento);

	set_corp_formas(f, cor_borda);
	set_corb_formas(f, copia_preenchimento);

	free(copia_preenchimento);
}

void alterna_cores_entre_formas(forma *f1, forma *f2) {
	if (f1 == NULL || f2 == NULL) return;

	if (f1 == f2) {
		alterna_cores_forma(f1);
		return;
	}

	char *cor_preenchimento_f1 = get_corp_forma(f1);
	char *cor_borda_f2 = get_corb_forma(f2);

	if (cor_preenchimento_f1 == NULL || cor_borda_f2 == NULL) {
		return;
	}

	char *copia_preenchimento_f1 = malloc(strlen(cor_preenchimento_f1) + 1);
	if (copia_preenchimento_f1 == NULL) {
		fprintf(stderr, "Erro de memoria na funcao trocaCores!\n");
		return;
	}
	strcpy(copia_preenchimento_f1, cor_preenchimento_f1);

	set_corp_formas(f1, cor_borda_f2);
	set_corb_formas(f2, copia_preenchimento_f1);

	free(copia_preenchimento_f1);
}

double get_area_forma(forma *f) {
	if (f == NULL) {
		return 0.0;
	}

	switch (f -> tipo) {
		case CIRCULO: return calcAreaCirculo(f -> dados);
		case RETANGULO: return calcAreaRetangulo(f -> dados);
		case ANTEPARO: printf("Não há cálculo de área para anteparos!\n"); break;
		case LINHA: return calcAreaLinha(f -> dados);
		case TEXTO: return calcAreaTexto(f -> dados);
		default: break;
	}

	return 0.0;
}

void destrutor_forma(forma *f) {
	if (f == NULL) {
		return;
	}

	void* dados = f -> dados;

	switch (f -> tipo) {
		case CIRCULO: destrutorCirculo(dados); break;
		case RETANGULO: destrutorRetangulo(dados); break;
		case LINHA: destrutorLinha(dados); break;
		case ANTEPARO: free_anteparo(dados); break;
		case TEXTO: destrutorTexto(dados); break;
		default: break;
	}

	f -> dados = NULL;
	free(f);
}

void set_posicao_forma(forma *f, double x, double y) {
	if (f == NULL || f -> dados == NULL) {
		return;
	}

	switch (f -> tipo) {
		case CIRCULO: {
			circulo *c = (circulo*) f -> dados;
			setXCirculo(c, x);
			setYCirculo(c, y);
			break;
		}

		case RETANGULO: {
			retangulo *r = (retangulo*) f -> dados;
			setXretangulo(r, x);
			setYretangulo(r, y);
			break;
		}

		case LINHA:
		case ANTEPARO: {
			linha *l = (linha*) f -> dados;

			double x1Antigo = getX1Linha(l);
			double x2Antigo = getX2Linha(l);
			double y1Antigo = getY1Linha(l);
			double y2Antigo = getY2Linha(l);

			double dX = x - x1Antigo;
			double dY = y - y1Antigo;

			setX1Linha(l, x);
			setY1Linha(l, y);
			setX2Linha(l, x2Antigo + dX);
			setY2Linha(l, y2Antigo + dY);
			break;
		}

		case TEXTO: {
			texto *t = (texto*)f -> dados;
			setXTexto(t, x);
			setYTexto(t, y);
			break;
		}

		default: printf("Forma inexistente ou sem posicao!\n"); break;

	}
}

void desenha_forma_svg(forma *f, FILE *svg) {
	tipo_forma tipo = get_tipo_forma(f);
	void *dados = get_dados_forma(f);

	switch (tipo) {
		case CIRCULO: insere_circulo(svg, (circulo*)dados); break;
		case RETANGULO: insere_retangulo(svg, (retangulo*)dados); break;
		case LINHA: insere_linha(svg, (linha*)dados); break;
		case ANTEPARO: insere_anteparo(svg, (anteparo*)dados); break;
		case TEXTO:  insere_texto(svg, (texto*)dados); break;
		default: break;
	}
}

void escreveDadosFormaTxt(forma *f, FILE *txt, const char *reportDaFuncaoQRY) {
    tipo_forma tipo = get_tipo_forma(f);
    void* dados = get_dados_forma(f);

    const char* report_safe = (reportDaFuncaoQRY != NULL) ? reportDaFuncaoQRY : "";

    switch (tipo) {
        case CIRCULO: {
            fprintf(txt, "%s\n Círculo\n ID: %i\n Âncora em: (%.2lf, %.2lf)\n Raio: %lf\n Cor de borda: %s\n Cor de preenchimento: %s\n",
                report_safe,
                get_id_forma(f),
                getXCirculo(dados),
                getYCirculo(dados),
                getRaioCirculo(dados),
                getCorbCirculo(dados),
                getCorpCirculo(dados));
            break;
        }

        case RETANGULO: {
            fprintf(txt, "%s\n Retângulo\n ID: %i\n Âncora em: (%.2lf, %.2lf)\n Largura: %lf\n Altura %lf\n Cor de borda: %s\n Cor de preenchimento: %s\n",
                report_safe,
                get_id_forma(f),
                getXretangulo(dados),
                getYretangulo(dados),
                getLarguraRetangulo(dados),
                getAlturaRetangulo(dados),
                getCorbRetangulo(dados),
                getCorpRetangulo(dados));
            break;
        }

        case LINHA:{
            fprintf(txt, "%s\n Linha\n ID: %i\n Âncora de início em: (%.2lf, %.2lf)\n Âncora de fim em: (%.2lf, %.2lf)\n Cor: %s\n",
                report_safe,
                get_id_forma(f),
                getX1Linha(dados),
                getY1Linha(dados),
                getX2Linha(dados),
                getY2Linha(dados),
                getCorLinha(dados));
            break;
        }

        case TEXTO: {
            texto* t = (texto*)dados;
            estilo* e = getEstiloTexto(t);

            fprintf(txt, "%s\n Texto\n ID: %d\n Âncora em: (%.2f, %.2f)\n Posição da Âncora: %c\n Conteúdo: \"%s\"\n Cor de borda: %s\n Cor de preenchimento: %s\n",
                report_safe,
                get_id_forma(f),
                getXTexto(t),
                getYTexto(t),
                getATexto(t),
                getTxtoTexto(t),
                getCorbTexto(dados),
                getCorpTexto(dados));

            if (e != NULL) {
                fprintf(txt, " Família da fonte: %s\n Peso da fonte: %s\n Tamanho da fonte: %s\n\n",
                    getfFamily(e),
                    getfWeight(e),
                    getfSize(e));
            } else {
                fprintf(txt, "\n");
            }
            break;
        }

    	case ANTEPARO: {
	        fprintf(txt, "%s\n Anteparo\n ID: %i\n Ponto de ínício em: (%lf, %lf)\n Ponto de fim em: (%lf, %lf)\n Cor: %s\n",
	        	report_safe, get_id_anteparo(dados),
	        	get_x_p0(dados),
	        	get_y_p0(dados),
	        	get_x_p1(dados),
	        	get_y_p1(dados),
	        	get_cor_anteparo(dados));
        	break;
        }

        default:
            fprintf(txt, "Tipo de forma desconhecida!\n");
            break;
    }
}

double get_x_forma(forma *f) {
	if (f == NULL) {
		return 0.0;
	}

	tipo_forma tipo = get_tipo_forma(f);
	void* dados = get_dados_forma(f);

	switch (tipo) {
		case CIRCULO: return getXCirculo(dados);
		case RETANGULO: return getXretangulo(dados);
		case TEXTO: return getXTexto(dados);
		case LINHA: return getX1Linha(dados);
		case ANTEPARO: return get_x_p0(dados);
		default: return 0.0;
	}
}

double get_y_forma(forma *f) {
	if (f == NULL) {
		return 0.0;
	}

	tipo_forma tipo = get_tipo_forma(f);
	void* dados = get_dados_forma(f);

	switch (tipo) {
		case CIRCULO: return getYCirculo(dados);
		case RETANGULO: return getYretangulo(dados);
		case TEXTO: return getYTexto(dados);
		case LINHA: return getY1Linha(dados);
		case ANTEPARO: return get_y_p0(dados);
		default: return 0.0;
	}
}

lista *forma_anteparo(forma *f, char orientacao) {
	tipo_forma tipo = get_tipo_forma(f);
	void *dados = get_dados_forma(f);

	static int id_anteparo = 10000;
	int novo_id = ++id_anteparo;
	lista *anteparos = init_lista();
	if (anteparos == NULL) return NULL;

	switch (tipo) {
		case CIRCULO: {
			anteparo *anteparo_circulo = circulo_anteparo(dados, orientacao);
			forma *criada = cria_forma(++novo_id,  ANTEPARO, anteparo_circulo);
			insert_tail(anteparos, criada);
			break;
		}

		case RETANGULO: {
			lista *anteparos_ret = retangulo_anteparo(dados);
			while (get_tam_lista(anteparos_ret) != 0) {
				anteparo *retirado = remove_head(anteparos_ret);
				forma *criada = cria_forma(++novo_id, ANTEPARO, retirado);
				insert_tail(anteparos, criada);
			}
			free_lista(anteparos_ret, NULL);
			break;
		}

		case LINHA: {
			anteparo *copia = linha_anteparo(dados);
			forma *criada = cria_forma(++novo_id, ANTEPARO, copia);
			insert_tail(anteparos, criada);
			break;
		}

		case TEXTO: {
			anteparo *texto = converter_texto_para_anteparo(dados);
			forma *criada = cria_forma(++novo_id, ANTEPARO, texto);
			insert_tail(anteparos, criada);
			break;
		}

		default:
			return init_lista();
	}

	return anteparos;
}