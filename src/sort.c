#include "sort.h"

#include <stdlib.h>
#include <string.h>

#include "lista.h"


void insertion_sort_generico(void *base, size_t num_elementos, size_t tamanho_elemento, int (*cmp_data)(void *a, void *b)) {
    if (num_elementos <= 1) {
        return;
    }

    char *arr = (char*)base;

    char *temp_key = malloc(tamanho_elemento);
    if (temp_key == NULL) {
        return;
    }

    for (size_t i = 1; i < num_elementos; i++) {
        char *ptr_i= arr + i * tamanho_elemento;
        memcpy(temp_key, ptr_i, tamanho_elemento);

        char *ptr_j = ptr_i - tamanho_elemento;
        int j = (int)i - 1;

        while (j >= 0 && cmp_data(ptr_j, temp_key) > 0) {

            memmove(ptr_j + tamanho_elemento, ptr_j, tamanho_elemento);

            j--;
            ptr_j -= tamanho_elemento;
        }

        char *ptr_insertion = arr + (j + 1) * tamanho_elemento;
        memcpy(ptr_insertion, temp_key, tamanho_elemento);
    }

    free(temp_key);
}



void insertion_sort_lista(lista *l, int (*cmp_data)(void *a, void *b)) {
    if (is_empty_lista(l) || get_tam_lista(l) == 1) {
        return;
    }

    node *node_i = get_head_next(l);


    while (node_i != NULL) {
        node *node_j = node_i;
        node *j_prev = go_prev_node(node_j);

        node *next_i = go_next_node(node_i);


        while (j_prev != NULL && cmp_data(get_node_data(node_j), get_node_data(j_prev)) < 0) {
            swap_node_data(j_prev, node_j);

            node_j = j_prev;
            j_prev = go_prev_node(node_j);
        }

        node_i = next_i;
    }
}
