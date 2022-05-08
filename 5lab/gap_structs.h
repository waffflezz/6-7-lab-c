//
// Created by dimas on 09.04.2022.
//

#ifndef UNTITLED1_GAP_STRUCTS_H
#define UNTITLED1_GAP_STRUCTS_H

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
    SIX,
    SEVEN,
    EIGHT
} menu_items;

typedef struct {
    int gap_type;
    double left_dot;
    double right_dot;
    double length;
} gap;

#endif //UNTITLED1_GAP_STRUCTS_H
