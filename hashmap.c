#ifndef __C_HASHMAP_UTIL

#define __C_HASHMAP_UTIL

#include "hashmap.h"

// src: djb2 http://www.cse.yorku.ca/~oz/hash.html
inline int hash_string(char *key) {
    int hash = 5381;
    int c;

    while ((c = *key++)) {
        hash = ((hash << 5) + hash) + c;
    }

    return hash;
}

Hashmap *new_map() {
    Hashmap *map = malloc(sizeof(Hashmap));
    memset((void *)map, 0, sizeof(Hashmap));
    map->elements = malloc(STARTING_CAPACITY * sizeof(Element));
    memset((void *)map->elements, 0, STARTING_CAPACITY * sizeof(Element));
    map->used = 0;
    map->capacity = STARTING_CAPACITY;

    return map;
}

int _try_add(Hashmap *map, char *key, void *value, int offset) {
    int hash = hash_string(key) % map->capacity;
    if (hash+offset >= map->capacity) {
        return 1;
    } else if (map->elements[hash+offset].key == NULL) {
        char *new = malloc(strlen(key) + 1);
        strcpy(new, key);

        map->elements[hash+offset].key = new;
        map->elements[hash+offset].data = value;
        map->used += 1;
        return 0;
    } else if (!strcmp(key, map->elements[hash+offset].key)) {
        map->elements[hash+offset].data = value;
        return 0;
    } else {
        return 1;
    }
}

void _realloc_map(Hashmap *map) {
    int new_capacity = map->capacity * 2;
    Element *new_elements = malloc(new_capacity * sizeof(Element));
    memset(new_elements, 0, new_capacity * sizeof(Element));
    Hashmap tmp = { .elements = new_elements, .used = 0,
        .capacity = new_capacity};
    Element *elements = get_element_list(map);

    for(int i=0; i<map->used; i++) {
        map_add(&tmp, elements[i].key, elements[i].data);
    }
    map->capacity = new_capacity;
    free(map->elements);
    map->elements = new_elements;
}

Element *get_element_list(Hashmap *map) {
    Element *elements = malloc(sizeof(Element) * map->used);
    memset(elements, 0, sizeof(Element) * map->used);
    int found = 0;

    for(int i=0; i<map->capacity && found<map->used; i++) {
        if (map->elements[i].key != NULL) {
            elements[found].key = map->elements[i].key;
            elements[found].data = map->elements[i].data;
            found += 1;
        }
    }
    return elements;
}

void map_add(Hashmap *map, char *key, void *value) {
    int i = 0;

    while(_try_add(map, key, value, i++)) {
        if (i > MAX_OFFSET) {
            _realloc_map(map);
            i = 0;
        }
    }
}

void *_try_get(Hashmap *map, char *key, int offset) {
    int hash = hash_string(key) % map->capacity;

    if (hash+offset >= map->capacity) {
        return NULL;
    } else if (map->elements[hash+offset].key == NULL) {
        return NULL;
    } else if (strcmp(map->elements[hash+offset].key, key)) {
        return NULL;
    } else {
        return map->elements[hash+offset].data;
    }
}

void *map_get(Hashmap *map, char *key) {
    void *result = NULL;
    int i = 0;
    while((result = _try_get(map, key, i++)) == NULL) {
        if (i > MAX_OFFSET) {
            return NULL;
        }
    }
    return result;
}

#endif
