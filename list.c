#ifndef __C_LIST_UTIL

#define __C_LIST_UTIL

#include <stdarg.h>
#include <stdlib.h>
#include <string.h>

#define DEFAULT_CAPACITY 10

typedef struct {
    void *data;
    int element_size;
    int length;
    int capacity;
} List;

static void _realloc_data(List *list) {
    void *temp = realloc(list->data, list->element_size * list->capacity);

    if (temp != list->data) {
        free(list->data);
        list->data = temp;
    }
}

static void _ensure_capacity(List *list, int length, int buffer, float scale) {
    if (list->capacity < length) {
        list->capacity = (int) ((length + buffer) * scale);
        _realloc_data(list);
    }
}

static void _add(List *list, void *e) {
    memcpy(list->data + list->element_size * list->length, e, list->element_size);
    list->length++;
}

List* _list_create(int element_size) {
    List *list = (List*) malloc(sizeof(List));

    *list = (List) {
        NULL,
        element_size,
        0,
        DEFAULT_CAPACITY
    };
    _realloc_data(list);

    return list;
}

#define list_create(type_t) (_list_create(sizeof(type_t)))

void list_add_p(List *list, void *e) {
    _ensure_capacity(list, list->length + 1, DEFAULT_CAPACITY, 1.5f);
    _add(list, e);
}

#define list_add(l, type_t, e) do {type_t temp = e; list_add_p(l, &temp);} while (0)

void list_add_all(List *list, int count, ...) {
    _ensure_capacity(list, list->length + count, DEFAULT_CAPACITY, 1.5f);

    va_list arg;
    va_start(arg, count);

    while (count-->0) {
        _add(list, va_arg(arg, void*));
    }

    va_end(arg);
}

void* list_get_p(List *list, int i) {
    return list->data + list->element_size * i;
}

#define list_get(l, type_t, i) (*(type_t*)list_get_p(l, i))

#endif
