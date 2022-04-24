#include <stdio.h>
#include <stdlib.h>
#include <float.h>
#include "gap_structs.h"
#include "input_functions.h"

#define MAX_GAP 1000

void print_menu();

void print_gap_submenu();

void print_structs(gap *gap_array, gap *end_ptr);

void add_gap(int gap_type, double left_dot, double right_dot, gap **end_ptr);

void delete_gap(int gap_number, gap *gap_array, gap **end_ptr);

int comparator(const void *tmp1, const void *tmp2);

void add_flag(unsigned char *flag, int gap_type);

void delete_flag(unsigned char *flag, int gap_type);

void enter_flags(int field_input);

void field_input();

void clear_flags();

void print_fields();

void print_with_filter(gap *gap_array, gap *end_ptr);

DblLinkedList* createDblLinkedList();

void deleteDblLinkedList(DblLinkedList **list);

union {
    struct {
        unsigned char is_line: 1;
        unsigned char is_left_dot: 1;
        unsigned char is_right_dot: 1;
        unsigned char is_length: 1;
    };
    unsigned char flags;
} Params;

int main(void) {
    size_t gap_size = sizeof(gap);
    gap *gap_array = malloc(gap_size * MAX_GAP);
    gap *end_ptr = gap_array;

    int menu_input;
    do {
        print_menu();
        puts("Enter value:");
        menu_input = input_int();
        switch (menu_input) {
            case FIRST:
                if ((end_ptr - gap_array) == MAX_GAP) {
                    printf("Array is full!");
                    break;
                }

                print_gap_submenu();
                puts("Enter type of gap:");
                int submenu_input = input_int();
                if (submenu_input <= 0 || submenu_input > 5) {
                    puts("Incorrect value!");
                    continue;
                }

                double left_dot, right_dot;
                if (submenu_input == LINE || submenu_input == INTERVAL ||
                    submenu_input == HALF_INTERVAL) {
                    puts("Enter left dot:");
                    left_dot = input_double();
                    puts("Enter right dot:");
                    right_dot = input_double();
                    if (left_dot > right_dot) {
                        puts("Left dot > right dot (incorrect!)");
                        continue;
                    }
                } else if (submenu_input == NOT_LIMIT_UP) {
                    puts("Enter left dot:");
                    left_dot = input_double();
                    right_dot = DBL_MAX;
                } else {
                    puts("Enter right dot:");
                    left_dot = DBL_MIN;
                    right_dot = input_double();
                }

                add_gap(submenu_input, left_dot, right_dot, &end_ptr);
                puts("");

                break;
            case SECOND:
                puts("Enter struct number:");
                int gap_number = input_int();

                delete_gap(gap_number, gap_array, &end_ptr);

                break;
            case THIRD:
                print_structs(gap_array, end_ptr);

                break;
            case FOUR:
                field_input();

                int num_of_elements = end_ptr - gap_array;

                qsort(gap_array, num_of_elements, sizeof(gap),
                      comparator);

                clear_flags();

                break;
            case FIVE:
                field_input();

                print_with_filter(gap_array, end_ptr);

                clear_flags();
                break;
            case SIX:
                free(gap_array);
                break;
        }

    } while (menu_input != SIX);

    return 0;
}

void add_flag(unsigned char *flag, int gap_type) {
    *flag |= (1 << (gap_type - 1));
}

void delete_flag(unsigned char *flag, int gap_type) {
    *flag ^= (1 << (gap_type - 1));
}

void enter_flags(int field_input) {
    switch (field_input) {
        case FIRST:
            (Params.is_line) ? delete_flag(&Params.flags,
                                           field_input)
                             : add_flag(&Params.flags,
                                        field_input);
            break;
        case SECOND:
            (Params.is_left_dot) ? delete_flag(&Params.flags,
                                               field_input)
                                 : add_flag(&Params.flags,
                                            field_input);
            break;
        case THIRD:
            (Params.is_right_dot) ? delete_flag(&Params.flags,
                                                field_input)
                                  : add_flag(&Params.flags,
                                             field_input);
            break;
        case FOUR:
            (Params.is_length) ? delete_flag(&Params.flags,
                                             field_input)
                               : add_flag(&Params.flags,
                                          field_input);
            break;
    }
}

void clear_flags() {
    Params.flags = 0b00000000;
}

void field_input() {
    do {
        print_fields();
        puts("Choose fields: (for exit enter 0)");
        int field_input = input_int();
        if (field_input == 0) break;
        if (field_input < FIRST || field_input > FOUR)
            continue;

        enter_flags(field_input);

    } while (TRUE);
}

void add_gap(int gap_type, double left_dot, double right_dot, gap **end_ptr) {


    gap *end_p = *end_ptr;

    end_p->gap_type = gap_type;
    end_p->left_dot = left_dot;
    end_p->right_dot = right_dot;

    if (end_p->gap_type == LINE)
        end_p->length = end_p->right_dot - end_p->left_dot;
    else
        end_p->length = 0;

    (*end_ptr)++;
}

void delete_gap(int gap_number, gap *gap_array, gap **end_ptr) {
    gap *end_p = *end_ptr;

    int gap_count = end_p - gap_array;

    if (gap_number > gap_count || gap_number == 0) {
        puts("Incorrect value!");
        return;
    }

    for (gap_array += gap_number - 1; gap_array != (end_p - 1); ++gap_array) {
        gap next_gap = *(++gap_array);
        --gap_array;
        *gap_array = next_gap;
    }

    (*end_ptr)--;
}

int comparator(const void *tmp1, const void *tmp2) {
    gap *tm1 = (gap *) tmp1;
    gap *tm2 = (gap *) tmp2;
    int cmp;

    if (Params.is_line) {
        cmp = tm1->gap_type - tm2->gap_type;
        if (cmp) return cmp;
    }

    if (Params.is_left_dot) {
        cmp = tm1->left_dot - tm2->left_dot;
        if (cmp) return cmp;
    }

    if (Params.is_length) {
        if (tm1->gap_type == LINE && tm2->gap_type == LINE)
            return tm1->length - tm2->length;
    }

    if (Params.is_right_dot) {
        cmp = tm1->right_dot - tm2->right_dot;
        return cmp;
    }

    return 0;
}

void print_with_filter(gap *gap_array, gap *end_ptr) {
    for (; gap_array != end_ptr; ++gap_array) {
        if (Params.is_line) {
            printf("Type: ");
            print_type_string(gap_array->gap_type);
        }
        if (Params.is_left_dot)
            printf("Left dot: %.3f\n", gap_array->left_dot);
        if (Params.is_right_dot)
            printf("Right dot: %.3f\n", gap_array->right_dot);
        if (Params.is_length && gap_array->gap_type == LINE)
            printf("Length: %.3f\n", gap_array->length);
    }
}

void print_structs(gap *gap_array, gap *end_ptr) {
    if (gap_array == end_ptr) {
        puts("There are no structures :c");
        return;
    }

    for (int i = 1; gap_array != end_ptr; ++gap_array, ++i) {
        printf("Struct number: %d\n", i);
        printf("Type: ");
        print_type_string(gap_array->gap_type);

        if (gap_array->gap_type == LINE)
            printf("Length: %.3f\n", gap_array->length);

        if (gap_array->left_dot == DBL_MIN) {
            printf("Left dot: infinity\n");
        } else {
            printf("Left dot: %.3f\n", gap_array->left_dot);
        }

        if (gap_array->right_dot == DBL_MAX) {
            printf("Right dot: infinity\n\n");
        } else {
            printf("Right dot: %.3f\n\n", gap_array->right_dot);
        }
    }
    puts("");
}

void print_menu() {
    puts("1. Add gap");
    puts("2. Delete gap");
    puts("3. Print gaps");
    puts("4. Sort gaps");
    puts("5. Filter");
    puts("6. Exit");
    puts("");
}

void print_gap_submenu() {
    puts("1. Line");
    puts("2. Interval");
    puts("3. Half interval");
    puts("4. Not limit up interval");
    puts("5. Not limit down interval");
    puts("");
}

void print_fields() {
    (Params.is_line) ? puts("1. Type +") : puts("1. Type");
    (Params.is_left_dot) ? puts("2. Left dot +") : puts("2. Left dot");
    (Params.is_right_dot) ? puts("3. Right dot +") : puts("3. Right dot");
    (Params.is_length) ? puts("4. Length +") : puts("4. Length");
    puts("");
}
