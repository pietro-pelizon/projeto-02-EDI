#ifndef PARSER_QRY_H
#define PARSER_QRY_H

/// @brief Faz a leitura do arquivo .qry e executa as ações conforme os comandos
/// @param formas Lista de formas criadas no .geo
/// @param anteparos Lista contendo os anteparos (transformações das formas)
/// @param nome_path_qry Nome do arquivo .qry
/// @param nome_path_txt Nome do arquivo .txt que será produzido
/// @param threshold_i Define o valor aonde o Merge Sort irá usar Insertion Sort
/// @param tipo_ord Tipo de ordenação selecionada
/// @param path_saida Nome do diretório de saída
/// @param path_base_svg Nome inicial do arquivo .svg que será produzido caso 'sfx' seja diferente de '-'
void parser_qry(lista *formas, lista *anteparos, char *nome_path_qry, char *nome_path_txt,
                int threshold_i, char tipo_ord, const char *path_saida, const char *path_base_svg);

#endif