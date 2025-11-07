#include <stdio.h>
#include <stdlib.h>
#include "../lista.h"
#include "../sort.h"

int compare_ints(void* a, void* b) {
    int int_a = *(int*)a;
    int int_b = *(int*)b;

    return (int_a - int_b);
}

void print_array(int* arr, size_t num) {
    for (size_t i = 0; i < num; i++) {
        printf("%d ", arr[i]);
    }
    printf("\n");
}

void print_int_data(void *data) {
    printf("%d ", *(int*)data);
}

void free_int_ptr(void *data) {
    free(data);
}

int main() {

    printf("===========================================\n");
    printf("== Teste 1: insertion_sort_generico (Array)\n");
    printf("===========================================\n");

    int array[] = {8, 2, 5, 1, 10, 9, 3};
    size_t num_elementos = sizeof(array) / sizeof(array[0]);

    printf("Array ANTES:  ");
    print_array(array, num_elementos);

    insertion_sort_generico(array, num_elementos, sizeof(int), compare_ints);

    printf("Array DEPOIS: ");
    print_array(array, num_elementos);
    printf("\n");


    printf("===========================================\n");
    printf("== Teste 2: insertion_sort_lista (TAD Lista)\n");
    printf("===========================================\n");

    lista* minha_lista = init_lista();

    int valores[] = {8, 2, 5, 1, 10, 9, 3};
    size_t num_lista = sizeof(valores) / sizeof(valores[0]);

    for (size_t i = 0; i < num_lista; i++) {
        int* dado = (int*)malloc(sizeof(int));
        *dado = valores[i];
        insert_tail(minha_lista, dado);
    }

    printf("Lista ANTES:  ");
    print_lista(minha_lista, print_int_data);
    printf("\n");

    insertion_sort_lista(minha_lista, compare_ints);

    printf("Lista DEPOIS: ");
    print_lista(minha_lista, print_int_data);
    printf("\n");

    free_lista(minha_lista, free_int_ptr);

    printf("\nTestes concluidos.\n");

    return 0;
}