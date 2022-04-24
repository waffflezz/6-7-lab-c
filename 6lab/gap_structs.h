//
// Created by dimas on 09.04.2022.
//

#ifndef UNTITLED1_GAP_STRUCTS_H
#define UNTITLED1_GAP_STRUCTS_H
#include <stdlib.h>

typedef enum {
    FALSE,
    TRUE
} bool;

typedef enum {
    LINE = 1,
    INTERVAL,
    HALF_INTERVAL,
    NOT_LIMIT_UP,
    NOT_LIMIT_DOWN
} gap_types;

typedef enum {
    FIRST = 1,
    SECOND,
    THIRD,
    FOUR,
    FIVE,
    SIX
} menu_items;

typedef struct {
    int gap_type;
    double left_dot;
    double right_dot;
    double length;
} gap;

typedef struct _Node {
    gap value;
    struct _Node *next;
    struct _Node *prev;
} Node;

typedef struct _DblLinkedList {
    size_t size;
    Node *head;
    Node *tail;
} DblLinkedList;

#endif //UNTITLED1_GAP_STRUCTS_H
