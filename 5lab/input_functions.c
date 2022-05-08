//
// Created by dimas on 09.04.2022.
//
#include "input_functions.h"
#include "gap_structs.h"
#include <stdio.h>
#include <stdlib.h>
#include <ctype.h>
#include <string.h>

#define MAX_INPUT_ARRAY_LEN 10

int meetings_count(const char* str, const char* substr) {
    return ( str = strstr(str, substr) ) ? 1 + meetings_count(str + 1, substr) : 0;
}

double input_double() {
    char str_array_len[MAX_INPUT_ARRAY_LEN];
    fgets(str_array_len, MAX_INPUT_ARRAY_LEN, stdin);
    fflush(stdin);
    if (!input_double_check(str_array_len) || str_array_len[0] == '\n') {
        return FALSE;
    }

    return strtod(str_array_len, NULL);
}

int input_double_check(char value_array[]) {
    if (value_array[0] == '.' || meetings_count(value_array, "-") > 1
        || meetings_count(value_array, ".") > 1)
        return FALSE;

    if (value_array[0] != '-' && meetings_count(value_array, "-") > 1)
        return FALSE;

    for (int i = 0; value_array[i] != '\n'; ++i) {
        if (value_array[i] == '.' || value_array[i] == '-') continue;

        if (!isdigit(value_array[i])) {
            return FALSE;
        }
    }

    return TRUE;
}

int input_int() {
    char str_array_len[MAX_INPUT_ARRAY_LEN];
    fgets(str_array_len, MAX_INPUT_ARRAY_LEN, stdin);
    fflush(stdin);
    if (!(input_int_check(str_array_len))
        || str_array_len[0] == '\0') {
        return -1;
    }

    return strtol(str_array_len, NULL, 10);
}

int input_int_check(char value_array[]) {
    for (int i = 0; i < strlen(value_array) - 1; ++i) {
        if ((value_array[i] < '0') || (value_array[i] > '9')) {
            return FALSE;
        }
    }
    return TRUE;
}

void print_type_string(int gap_type) {
    switch (gap_type) {
        case LINE:
            puts("Line");
            break;
        case INTERVAL:
            puts("Interval");
            break;
        case HALF_INTERVAL:
            puts("Half interval");
            break;
        case NOT_LIMIT_UP:
            puts("Not limit up interval");
            break;
        case NOT_LIMIT_DOWN:
            puts("Not limit down interval");
            break;
        default:
            puts("Undefined");
            break;
    }
}
