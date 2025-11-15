#include "sort.h"

#include <stddef.h>
#include <stdio.h>
#include <stdlib.h>
#include <string.h>


int cmp_double(const void *a, const void *b) {
    double ia = *(const double *)a;
    double ib = *(const double *)b;
    return (ia > ib) - (ia < ib);
}

void insertion_sort_generico(void *base, size_t num_elementos, size_t tamanho_elemento, int (*cmp_data)(const void *a, const void *b)) {
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

static void merge_halves_generico(char *base, char *temp_array, int inicio, int meio, int fim, size_t tam_elem, int (*cmp_data)(const void *a, const void *b)) {

    size_t slice_size = (fim - inicio + 1) * tam_elem;
    memcpy(temp_array + (inicio * tam_elem), base + (inicio * tam_elem), slice_size);

    int i = inicio;
    int j = meio + 1;
    int k = inicio;

    while (i <= meio && j <= fim) {
        char *elem_i = temp_array + (i * tam_elem);
        char *elem_j = temp_array + (j * tam_elem);

        if (cmp_data(elem_i, elem_j) <= 0) {
            memcpy(base + (k * tam_elem), elem_i, tam_elem);
            i++;
        } else {
            memcpy(base + (k * tam_elem), elem_j, tam_elem);
            j++;
        }
        k++;
    }

    while (i <= meio) {
        memcpy(base + (k * tam_elem), temp_array + (i * tam_elem), tam_elem);
        i++;
        k++;
    }

    while (j <= fim) {
        memcpy(base + (k * tam_elem), temp_array + (j * tam_elem), tam_elem);
        j++;
        k++;
    }
}

static void mergesort_recursivo(char *base, char *temp_array, int inicio, int fim, size_t tam_elem, int (*cmp_data)(const void *a, const void *b), int threshold) {

    int tamanho_slice = fim - inicio + 1;

    if (tamanho_slice <= threshold) {
        char *slice_base = base + (inicio * tam_elem);
        insertion_sort_generico(slice_base, tamanho_slice, tam_elem, cmp_data);
        return;
    }

    int meio = inicio + (fim - inicio) / 2;

    mergesort_recursivo(base, temp_array, inicio, meio, tam_elem, cmp_data, threshold);
    mergesort_recursivo(base, temp_array, meio + 1, fim, tam_elem, cmp_data, threshold);

    merge_halves_generico(base, temp_array, inicio, meio, fim, tam_elem, cmp_data);
}

void merge_sort_generico(void *base, size_t num_elementos, size_t tamanho_elemento, int (*cmp_data)(const void *a, const void *b), int threshold) {
    if (base == NULL || num_elementos <= 1) {
        return;
    }

    char *temp_array = malloc(num_elementos * tamanho_elemento);
    if (temp_array == NULL) {
        fprintf(stderr, "ERRO: Falha ao alocar memória para o mergesort!\n");
        return;
    }

    mergesort_recursivo((char*)base, temp_array, 0, (int)num_elementos - 1, tamanho_elemento, cmp_data, threshold);

    free(temp_array);
}