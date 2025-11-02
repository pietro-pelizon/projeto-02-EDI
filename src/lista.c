#include "lista.h"
#include <stdlib.h>
#include <stdio.h>
#include <stdbool.h>

typedef struct stNode {
    void *data;
    struct stNode *next;
    struct stNode *prev;
} node;

typedef struct stLista {
    int tam;
    node *head;
    node *tail;
} lista;

lista *init_lista() {
    lista *l = malloc (sizeof(lista));
    if (l == NULL) {
        printf("Erro ao alocar memoria para a lista!\n");
        return NULL;
    }

    l -> tam = 0;
    l -> head = NULL;
    l -> tail = NULL;

    return l;
}

int get_tam_lista(lista *l) {
    return l -> tam;
}

bool is_empty(lista *l) {
    if (l -> tam == 0 || l -> head == NULL) {
        return true;
    }

    return false;
}

void insert_head(lista *l, void *new_data) {
    node *new = malloc (sizeof(node));
    if (new == NULL) {
        printf("Erro ao alocar memoria para o novo node da lista!\n");
        return;
    }

    new -> data = new_data;
    new -> next = l -> head;
    new -> prev = NULL;

    if (l -> head != NULL) {
        l -> head -> prev = new;
    } else {
        l -> tail = new;
    }

    l -> head = new;
    l -> tam++;

}

void insert_tail(lista *l, void *new_data) {
    node *new = malloc (sizeof(node));
    if (new == NULL) {
        printf("DEBUG: Erro ao alocar memoria para o novo node\n");
        return;
    }

    new -> data = new_data;
    new -> next = NULL;

   if (is_empty(l)) {
        new -> prev = NULL;
        l -> head = new;
        l -> tail = new;
    }

    else {
        new -> prev = l -> tail;
        l -> tail -> next = new;
        l -> tail = new;
    }

    l -> tam ++;

}

void index_insert(lista *l, void *new_data, int index) {
    if (index > l -> tam || index < 0) {
        return;
    }

    if (index == 0) {
        insert_head(l, new_data);
        return;
    }

    if (index == l -> tam) {
        insert_tail(l, new_data);
        return;
    }

    node *new = malloc (sizeof(node));
    if (new == NULL) {
        printf("DEBUG: Erro ao alocar memoria o novo no da lista!\n");
        return;
    }
    new -> data = new_data;

    node *current = l -> head;
    for (int i = 0; i < index; i++) {
        current = current -> next;
    }


    new -> next = current;
    new -> prev = current -> prev;

    current -> prev -> next = new;
    current -> prev = new;

    l -> tam++;

}

void print_lista(lista *l, void (*print_data)(void *data)) {
    if (l == NULL) {
        printf("Ponteiro nulo passado para a funcao 'print_lista'\n");
        return;
    }

    node *current = l -> head;
    while (current != NULL) {
        if (current -> data != NULL && print_data != NULL) {
            print_data(current->data);
        }

        current = current -> next;
    }
}

void *search_lista(lista *l, void *key, int (*compare)(void *a, void *b)) {
    node *current = l -> head;
    while (current != NULL) {
        if (compare(key, current -> data) == 0) {
            return current -> data;
        }

        current = current -> next;
    }

    return NULL;
}

void *remove_head(lista *l) {
    if (l == NULL) {
        return NULL;
    }

    if (l -> head == NULL) {
        printf("DEBUG: Nada a remover, a lista ja esta vazia!\n");
        return NULL;
    }

    node *removed = l -> head;

    void *data = l -> head -> data;

    if (l -> head == l -> tail) {
        l -> head = NULL;
        l -> tail = NULL;
    }

    else {
        l->head = l -> head -> next;
        l->head->prev = NULL;
    }

    l -> tam--;

    free(removed);

    return data;
}

void *remove_tail(lista *l) {
    if (l == NULL) {
        return NULL;
    }

    if (is_empty(l)) {
        printf("DEBUG: Nada a remover, a lista ja esta vazia!\n");
        return NULL;
    }

    node *removed = l -> tail;
    void *data = l -> tail -> data;

    if (l -> head == l -> tail) {
        l -> head = NULL;
        l -> tail = NULL;
    }

    else {
        l -> tail = l -> tail -> prev;
        l -> tail -> next = NULL;
    }

    l -> tam--;

    free(removed);

    return data;

}

void *remove_index(lista *l, int index) {
    if (l == NULL || index < 0 || index > l -> tam ) {
        return NULL;
    }

    if (index == 0) {
        return remove_head(l);

    }

    if (index == l -> tam) {
        return remove_tail(l);
    }

    node *current = l -> head;
    for (int i = 0; i < index; i++) {
        current = current -> next;
    }

    void *data = current -> data;

    current -> prev -> next = current -> next;
    current -> next -> prev = current -> prev;

    l -> tam--;
    free(current);
    return data;

}

void *remove_data(lista *l, void *key, int (*compare)(void *a, void *b)) {
    if (l == NULL) {
        return NULL;
    }

    if (l -> tam == 0) {
        printf("A lista está vazia! Nada a remover!\n");
        return NULL;
    }

    node *current = l -> head;
    int index = 0;
      while (current != NULL) {
        if (compare(key, current -> data) == 0) {
            return remove_index(l, index);
        }

        current = current -> next;
        index++;
    }

    printf("DEBUG: Elemento não encontrado na lista\n");
    return NULL;
}

void free_lista(lista *l, void (*free_data)(void *data)) {
    if (l == NULL) {
        return;
    }

    node *current = l -> head;
    while (current != NULL) {
        node *next = current -> next;

        if (free_data != NULL && current -> data != NULL) {
            free_data(current -> data);
        }

        free(current);
        current = next;
    }

    free(l);
}

void *get_head(lista *l) {
    if (is_empty(l)) {
        printf("lista vazia!\n");
        return NULL;
    }

    return l -> head -> data;
}

void *get_tail(lista *l) {
    if (is_empty(l)) {
        printf("lista vazia!\n");
        return NULL;
    }

    return l -> tail -> data;
}

void *get_index(lista *l, int index) {
    if (is_empty(l)) {
        printf("lista vazia!\n");
        return NULL;
    }

    if (index < 0 || index > l -> tam) {
        printf("Index inválido!\n");
        return NULL;
    }

    if (index == 0) {
        return get_head(l);
    }

    if (index == l -> tam) {
        return get_tail(l);
    }

    node *current = l -> head;
    for (int i = 0; i < index; i++) {
        current = current -> next;
    }

    return current -> data;
}

void reverse_lista(lista *l) {
    if (l == NULL || l -> head == NULL) {
        return;
    }

    node *current = l -> head;
    node *temp = NULL;

    while (current != NULL) {
        temp = current -> prev;

        current -> prev = current -> next;
        current -> next = temp;

        current = current->prev;
    }

    temp = l->head;
    l->head = l->tail;
    l->tail = temp;
}


void clear_lista(lista *l, void (*free_data)(void *data)) {
    if (l == NULL) {
        return;
    }

    node *current = l->head;
    while (current != NULL) {
        node *next = current->next;

        if (free_data != NULL && current->data != NULL) {
            free_data(current->data);
        }

        free(current);
        current = next;
    }

    l->head = NULL;
    l->tail = NULL;
    l->tam = 0;
}

void foreach_lista(lista *l, void (*apply)(void *data)) {
    if (l == NULL) return;
    if (is_empty(l)) {
            return;
    }

    node *current = l -> head;
    while (current != NULL) {
        if (apply) {
            apply(current -> data);
        }
        current = current -> next;
    }
}

void *find_max(lista *l, int (*compare)(void *a, void *b)) {
    if (is_empty(l)) {
        return NULL;
    }

    if (l -> tam == 1) {
        return l -> head -> data;
    }

    void *max = l -> head -> data;
    node *current = l -> head -> next;
    while (current != NULL) {
        if (compare(current -> data, max) > 0) {
            max = current -> data;
        }
        current = current  -> next;
    }

    return max;
}

void *find_min(lista *l, int (*compare)(void *a, void *b)) {
     if (is_empty(l)) {
        return NULL;
    }

    if (l -> tam == 1) {
        return l -> head -> data;
    }

    void *min = l -> head -> data;
    node *current = l -> head -> next;
    while (current != NULL) {
        if (compare(current -> data, min) < 0) {
            min = current -> data;
        }
        current = current  -> next;
    }

    return min;
}

int contains(lista *l, void *key, int (*compare)(void *a, void *b)) {
    if (search_lista(l, key, compare) != NULL) {
        return true;
    }

    return false;
}

void swap(lista *l, int index1, int index2) {
    if (l == NULL) return;
    if (is_empty(l)) return;
    if (index1 == index2) return;
    if (index1 < 0|| index2 < 0 || index1 > l -> tam || index2 > l -> tam) return;

    node *node1 = l -> head;
    for (int i = 0; i < index1; i++) {
        node1 = node1 -> next;
    }

    node *node2 = l -> head;
    for (int i = 0; i < index2; i++) {
        node2 = node2 -> next;
    }

     void *temp = node1 -> data;
    node1 -> data = node2 -> data;
    node2 -> data = temp;
}

lista *filter_lista(lista *l, int (*predicate)(void *data)) {
    if (l == NULL) return NULL;
    if (is_empty(l)) return init_lista();

    lista *filtered_lista = init_lista();
    if (filtered_lista == NULL) {
        return NULL;
    }

    node *current = l -> head;
    while (current != NULL) {
        if (predicate && predicate(current -> data) == 1) {
            insert_tail(filtered_lista, current -> data);
        }
        current = current -> next;
    }

    return filtered_lista;
}


void map_lista(lista *l, void (*transform)(void *data)) {
    if (l == NULL || is_empty(l) || transform == NULL) return;

    node *current = l -> head;

    while (current != NULL) {
        transform(current -> data);
        current = current -> next;
    }
}

int remove_all_if(lista *l, int (*predicate)(void *data), void (*free_data)(void *data)) {
    if (l == NULL || predicate == NULL) return 0;

    int removidos = 0;
    node *current = l->head;
    node *next_node = NULL;

    while (current != NULL) {
        next_node = current->next;

        if (predicate(current->data)) {
            // Remove o nó atual
            if (current->prev != NULL) {
                current->prev->next = current->next;
            } else {
                l->head = current->next;
            }

            if (current->next != NULL) {
                current->next->prev = current->prev;
            } else {
                l->tail = current->prev;
            }

            if (free_data != NULL) {
                free_data(current->data);
            }
            free(current);
            removidos++;
            l->tam--;
        }
        current = next_node;
    }

    return removidos;
}