#pragma once
#ifndef LIST_H
#define LIST_H

#include <stdlib.h>
#include <string.h>

/* ======================= Types ======================= */

typedef enum { STRING_NODE, LIST_NODE } Type;

typedef struct List List;

typedef struct {
    Type type;
    union {
        char *str;
        List *list;
    };
} Node;

struct List {
    Node    *data;
    size_t   size;
    size_t   capacity;
};

/* ======================= Init / Destroy ======================= */

/* returns 1 on success */
static int init(List *l) {
    if (!l) return 0;

    l->size     = 0;
    l->capacity = 4;
    l->data     = (Node *)calloc(l->capacity, sizeof(Node));

    if (!l->data) {
        l->capacity = 0;
        return 0;
    }
    return 1;
}
static int get_length(List *l){
    if (!l) return 0;
    return l->size;
}

/* frees contents */
static void free_list(List *l);

/* frees contents + struct */
static void destroy(List *l) {
    if (!l) return;
    free_list(l);
    free(l);
}

/* ======================= Internal Helpers ======================= */

static int reserve(List *l, size_t new_cap) {
    if (!l || new_cap <= l->capacity) return 1;

    Node *tmp = (Node *)realloc(l->data, new_cap * sizeof(Node));
    if (!tmp) return 0;

    l->data     = tmp;
    l->capacity = new_cap;
    return 1;
}

/* ======================= Node Creation ======================= */

static Node make_string(const char *s) {
    Node n;
    n.type = STRING_NODE;
    n.str  = NULL;

    if (!s) return n;

    size_t len = strlen(s) + 1;
    n.str = (char *)malloc(len);
    if (n.str) memcpy(n.str, s, len);

    return n;
}

/* takes ownership of child */
static Node make_list(List *child) {
    Node n;
    n.type = LIST_NODE;
    n.list = child;
    return n;
}

/* ======================= Append / Access ======================= */

static int append(List *l, Node n) {
    if (!l || (l->capacity == 0 && !reserve(l, 4))) return 0;

    if (l->size == l->capacity) {
        if (!reserve(l, l->capacity * 2)) return 0;
    }

    l->data[l->size++] = n;
    return 1;
}

static Node *get(List *l, size_t i) {
    if (!l || i >= l->size) return NULL;
    return &l->data[i];
}

/* ======================= Free ======================= */

static void free_list(List *l) {
    if (!l || !l->data) return;

    for (size_t i = 0; i < l->size; i++) {
        Node *n = &l->data[i];

        if (n->type == STRING_NODE) {
            free(n->str);
        } else if (n->type == LIST_NODE) {
            destroy(n->list);
        }
    }

    free(l->data);
    l->data     = NULL;
    l->size     = 0;
    l->capacity = 0;
}

/* ======================= Iteration ======================= */

typedef enum {
    VISIT_ALL,   /* visit list + string nodes */
    VISIT_LEAF   /* visit only strings */
} VisitMode;

typedef void (*visit_fn)(Node *);

/* preorder traversal */
static void list_foreach(List *l, visit_fn fn, VisitMode mode) {
    if (!l || !fn) return;

    for (size_t i = 0; i < l->size; i++) {
        Node *n = &l->data[i];

        if (mode == VISIT_ALL) {
            fn(n);
        }

        if (n->type == LIST_NODE) {
            list_foreach(n->list, fn, mode);
        } else if (mode == VISIT_LEAF) {
            fn(n);
        }
    }
}

static List* list_slice(List *l, int start, int end){
    if (!l) return NULL;
    size_t lo = (start <= 0) ? 0u : (size_t)start;
    size_t hi = (end   <= 0) ? 0u : (size_t)end;

    if (lo > l->size) return NULL;
    if (hi > l->size) return NULL;
    if (lo > hi) return NULL;

    size_t count = hi - lo;

    List *slice = (List *)malloc(sizeof(List));
    if (!slice) return NULL;

    slice->size = count;
    slice->capacity = count;
    slice->data = (count == 0) ? NULL : (Node *) malloc(count * sizeof(Node));
    if(count && !slice->data){
        free(slice); return NULL;
    }
    if (count) {
        memcpy(slice->data, &l->data[lo], count * sizeof(Node));
    }
    return slice;
}

#endif /* LIST_H */