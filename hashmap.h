#ifndef __C_HASHMAP_UTIL_H

#define __C_HASHMAP_UTIL_H

#include <stdlib.h>
#include <string.h>

#define STARTING_CAPACITY 128
#define MAX_OFFSET 8

typedef struct {
    char *key;
    void *data;
} Element;

typedef struct {
    Element *elements;
    int used;
    int capacity;
} Hashmap;

inline int hash_string(char *);

Hashmap *new_map();

Element *get_element_list(Hashmap *);

void map_add(Hashmap *, char *, void *);
void *map_get(Hashmap *, char *);

#endif
