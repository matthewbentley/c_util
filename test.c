#include <stdio.h>

#include "list.c"

int main() {
    List *list = list_create(int);

    list_add(list, int, 1);
    list_add(list, int, 2);

    printf("Length: %d\n", list->length);

    int a = list_get(list, int, 1);

    printf("Second element: %d", a);
}
