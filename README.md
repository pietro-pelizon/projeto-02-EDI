<div align="center"> 

# Projeto 02 - Estrutura de Dados (EDI)

![Autor](https://img.shields.io/badge/Autor-Pietro%20Fernando%20Pelizon-maroon)
![C99](https://img.shields.io/badge/Language-C99-blue)
![Build](https://img.shields.io/badge/Makefile-GCC-green)
![Data Structures](https://img.shields.io/badge/ED-Doubly_Linked_List%20%7C%20AVL_Tree-orange)

</div>

## Estrutura do Repositório

```text
.
├── docs/              # PDFs com as especificações do trabalho
├── include/           # Arquivos de cabeçalho (.h)
├── src/               # Implementações (.c), Makefile e run.sh
├── testes/            # Arquivos de entrada (.geo e .qry)
└── README.md          # Documentação do repositório
```

## Sobre o Projeto

Este projeto, denominado **TED** (no executável), é um simulador de **Invasão Alienígena** que implementa e manipula estruturas de dados dinâmicas e complexas (Árvore AVL e Lista Duplamente Encadeada) para gerenciar objetos geométricos, anteparos e calcular polígonos de visibilidade.

O sistema lê arquivos de entrada (`.geo` e `.qry`) para configurar o ambiente e executar comandos de ataque com diferentes tipos de bombas (Destruição, Pintura e Clonagem), gerando como saída arquivos gráficos vetoriais (`.svg`) e relatórios de texto (`.txt`) detalhando as interações entre as formas e o polígono de visibilidade.

### Principais Funcionalidades

* **Manipulação de Formas Geométricas:** Suporte para Círculos, Retângulos, Linhas e Texto, com gestão de atributos como coordenadas, dimensões, cores e estilos de fonte.

* **TADs Genéricos:** Implementação de estruturas de dados genéricas (`void*`):
  * **Árvore AVL (AVL Tree):** Utilizada para armazenamento e busca eficiente dos anteparos ativos no cenário.
  * **Lista Duplamente Encadeada (Doubly Linked List):** Utilizada para o armazenamento e gerenciamento dinâmico das formas geométricas.
* **Transformação em Anteparos:** Conversão de formas geométricas em segmentos de reta que bloqueiam a explosão (Anteparos) através da aplicação de tinta especial.
* **Cálculo da Região de Visibilidade:** Determinação do polígono de visibilidade gerado a partir do ponto de explosão de uma bomba, considerando a oclusão causada pelos anteparos.
* **Mecânica de Bombas e Regras de Impacto:**
  * **Bomba de Destruição(`d`):** Elimina todas as formas contidas ou interceptadas pelo polígono de visibilidade.
  * **Bomba de Pintura(`p`):** Altera as cores de borda e preenchimento das formas atingidas pela região de visibilidade.
  * **Bomba de Clonagem(`cln`):** Duplica as formas atingidas e as translada pelas coordenadas especificadas.
* **Algoritmos de Ordenação:** Implementação flexível utilizando `qsort` ou `Merge Sort` customizado, com transição para `Insertion Sort` em subvetores pequenos.
* **Saída Gráfica e Geração de Relatórios:** Geração de arquivos (`.svg`) detalhando a posição final dos objetos e a área de impactos das bombas, além de relatórios (`.txt`) contendo os dados dos eventos.

## Pré-requisitos
Certifique-se de ter as seguintes ferramentas instaladas no seu ambiente:
* Compilador **GCC** (com suporte ao padrão C99)
* **Make** (para automação da build)
* Ambiente **Linux** ou **WSL** (Windows Subsystem for Linux)

## Compilação

O projeto inclui um `Makefile` para facilitar a compilação. Certifique-se de ter o `gcc` e o `make` instalados.

* Para compilar o projeto, execute:

```bash
cd src && make
```

Isto irá gerar o executável chamado **`ted`** dentro da pasta `src` e os arquivos `(*.o)` na pasta `build`.

* Para limpar os arquivos objeto (`*.o`) e o executável:

```bash
make clean
```

## Como Executar

A execução do programa requer parâmetros de linha de comando para indicar os diretórios, pastas de entrada/saída e as configurações do algoritmo de ordenação.

**Sintaxe:**

```bash
./ted -e [dir_entrada] -f [arquivo.geo] -o [dir_saida] -q [arquivo.qry] -i [threshold_insertion_sort] -to [q | m]
```

**Automação da execução:**

Pode se utilizar o script (`.sh`) provido no repositório para executar todos os testes de uma vez.

O seguinte comando deve ser executado a partir do diretório `src` após a compilação:

```bash
chmod +x run.sh && ./run.sh
```

Isso retira o processo de citar arquivo por arquivo corretamente. O único pré-requisito é que a pasta `testes` esteja presente na raiz do projeto, contendo todos os arquivos `(.geo)/(.qry)`.


**Parâmetros:**

| **Parâmetros**  | **Obrigatório?** | **Descrição**                                                                       |
|-----------------|------------------|-------------------------------------------------------------------------------------|
| `-e entrada `   | Não              | Diretório-base de entrada (`BED`). Caso omitido, usa o diretório corrente.            |
| `-f arq.geo`    | Sim              | Nome do arquivo contendo as formas presente no diretório de `entrada`.                |
| `-o saida`      | Sim              | Diretório-base de saída (`BSD`) onde os arquivos `.svg` e `.txt` serão gravados.          |
| `-q arq.qry `   | Não              | Arquivo de consultas da simulação.                                                  |
| `-to [q \|  m]` | Não              | Define qual algoritmo de ordenação (`q: qsort` padrão, `m: Merge Sort`). Default: `q`.    |
| `-i threshold`  | Não              | Define o limite (threshold) de elementos para acionar o `Insertion Sort`. Default: `10` |
| `[-h, --help]`  | Não              | Exibe a mensagem de ajuda com as instruções de execução.                            |

**Exemplo de uso:**

```bash
./ted -e entrada -f arq.geo -o saida -q arq.qry -i 15 -to m
```

## Estrutura dos arquivos de Entrada

### Arquivo `(.geo)`

Define as formas geométricas iniciais do cenário.

| Comando | Parâmetros                      | Descrição                             |
| :------ | :------------------------------ | :------------------------------------ |
| `c`     | `id x y r corb corp`            | Cria um Círculo.                      |
| `r`     | `id x y w h corb corp`          | Cria um Retângulo.                    |
| `l`     | `id x1 y1 x2 y2 cor`            | Cria uma Linha.                       |
| `t`     | `id x y corb corp ancora texto` | Cria um Texto.                        |
| `ts`    | `familia peso tamanho`          | Define o estilo do texto subsequente. |

### Arquivo `(.qry)`

Define os comandos operacionais da Invasão Alienígena.

| Comando | Parâmetros      | Descrição                                                                        |
|:--------|:----------------|:---------------------------------------------------------------------------------|
| `a`    | `i j [v \| h]`  | Converte as formas contidas no intervalo `[i, j]` em anteparos bloqueantes.        |
| `d`    | `x y sfx`       | Lança uma Bomba de Destruição nas coordenadas `(x, y).`                          |
| `p`  | `x y cor sfx`   | Lança uma Bomba de Pintura na coordenada `(x, y)`, aplicando a cor especificada. |
| `cln`  | `x y dx dy sfx` | Lança uma Bomba de Clonagem em `(x, y)` e desloca os clones por `(dx, dy)`.      |


## Estrutura do Código (Módulos)

- **`main.c`**: Ponto de entrada, manipulação de argumentos da CLI e fluxo geral.
- **`arvore.c/h`**: Implementação do TAD Árvore AVL para armazenamento e consulta dos anteparos.
- **`lista.c/h`**: Implementação do TAD Lista Duplamente Encadeada genérica para gerenciar as formas geométricas.
- **`visibilidade.c/h`**: Algoritmo responsável pelo cálculo da região/polígonos de visibilidade.
- **`poligono.c/h`**: Estrutura de dados e operações para representação de polígonos.
- **`anteparo.c/h`**: Representação de segmentos bloqueantes e funções de conversão de formas em anteparos.
- **`sort.c/h`**: Implementação das rotinas de ordenação (`Merge Sort`, `Insertion Sort` e wrapper para `qsort`).
- **`ponto.c/h`**: Estruturas e operações geométricas elementares com pontos 2D.
- **`geometria.c/h`**: Biblioteca com funções utilitárias de cálculos geométricos (distâncias, ângulos, produtos vetoriais).
- **`sobreposicao.c/h`**: Algoritmos para detecção de interseção e colisão entre polígonos, segmentos e formas geométricas.
- **`parser_geo.c/h` & `parser_qry.c/h`**: Interpretadores dos arquivos de entrada `.geo` e `.qry`.
- **`svg.c/h`**: Responsável pelo desenho e exportação do estado gráfico em formato `.svg`.
- **`formas.c/h`**: Invólucro (_wrapper_) genérico para suporte ao polimorfismo das formas geométricas.
