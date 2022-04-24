#include <stdio.h>
#include <stdlib.h>
#include <float.h>
#include "gap_structs.h"
#include "input_functions.h"

#define MAX_GAP 1000

void print_menu();

void print_gap_submenu();

void print_structs(DblLinkedList *list);

void add_gap(int gap_type, double left_dot, double right_dot, DblLinkedList *list);

void delete_gap(DblLinkedList *list, size_t index);

int comparator(void *tmp1, void *tmp2);

void add_flag(unsigned char *flag, int gap_type);

void delete_flag(unsigned char *flag, int gap_type);

void enter_flags(int field_input);

void field_input();

void clear_flags();

void print_fields();

void print_with_filter(gap *gap_array, gap *end_ptr);

DblLinkedList* createDblLinkedList();

void deleteDblLinkedList(DblLinkedList **list);

Node* getNth(DblLinkedList *list, size_t index);

void pushFront(DblLinkedList *list, gap data);

void pushBack(DblLinkedList *list, gap value);

void insertBeforeElement(DblLinkedList *list, Node* elm, gap value);

void insertionSort(DblLinkedList **list, int (*cmp)(void*, void*));

gap popFront(DblLinkedList *list);

void swap(Node *tmp1, Node *tmp2);

void ReverseList(DblLinkedList *list);

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
    DblLinkedList *list = createDblLinkedList();

    int menu_input;
    do {
        print_menu();
        puts("Enter value:");
        menu_input = input_int();
        switch (menu_input) {
            case FIRST:
                if (list->size == MAX_GAP) {
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

                add_gap(submenu_input, left_dot, right_dot, list);
                puts("");

                break;
            case SECOND:
                puts("Enter struct number:");
                int gap_number = input_int();

                delete_gap(list, gap_number);

                break;
            case THIRD:
                print_structs(list);

                break;
            case FOUR:
                field_input();

                insertionSort(&list, comparator);

                clear_flags();

                break;
            case FIVE:
                //field_input();
                ReverseList(list);
                // print_with_filter(gap_array, end_ptr);

                //clear_flags();
                break;
            case SIX:
                deleteDblLinkedList(&list);
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

DblLinkedList* createDblLinkedList() {
    DblLinkedList *tmp = (DblLinkedList*) malloc(sizeof(DblLinkedList));
    tmp->size = 0;
    tmp->head = tmp->tail = NULL;

    return tmp;
}

void deleteDblLinkedList(DblLinkedList **list) {
    Node *tmp = (*list)->head;
    Node *next = NULL;
    while (tmp) {
        next = tmp->next;
        free(tmp);
        tmp = next;
    }
    free(*list);
    (*list) = NULL;
}

void add_gap(int gap_type, double left_dot, double right_dot, DblLinkedList *list) {
    gap tmp_gap;
    tmp_gap.gap_type = gap_type;
    tmp_gap.left_dot = left_dot;
    tmp_gap.right_dot = right_dot;

    if (gap_type == LINE)
        tmp_gap.length = right_dot - left_dot;

    Node *tmp = (Node*) malloc(sizeof(Node));
    if (tmp == NULL) {
        exit(3);
    }
    tmp->value = tmp_gap;
    tmp->next = NULL;
    tmp->prev = list->tail;
    if (list->tail) {
        list->tail->next = tmp;
    }
    list->tail = tmp;

    if (list->head == NULL) {
        list->head = tmp;
    }
    list->size++;
}

Node* getNth(DblLinkedList *list, size_t index) {
    Node *tmp = list->head;
    size_t i = 0;

    while (tmp && i < index) {
        tmp = tmp->next;
        i++;
    }

    return tmp;
}

void delete_gap(DblLinkedList *list, size_t index) {
    Node *elm = NULL;
    gap tmp;
    elm = getNth(list, index);
    if (elm == NULL) {
        puts("Incorrect value!");
        return;
    }
    if (elm->prev) {
        elm->prev->next = elm->next;
    }
    if (elm->next) {
        elm->next->prev = elm->prev;
    }

    if (!elm->prev) {
        list->head = elm->next;
    }
    if (!elm->next) {
        list->tail = elm->prev;
    }

    free(elm);

    list->size--;
}

int comparator(void *tmp1, void *tmp2) {
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

void pushFront(DblLinkedList *list, gap data) {
    Node *tmp = (Node*) malloc(sizeof(Node));
    if (tmp == NULL) {
        exit(1);
    }
    tmp->value = data;
    tmp->next = list->head;
    tmp->prev = NULL;
    if (list->head) {
        list->head->prev = tmp;
    }
    list->head = tmp;

    if (list->tail == NULL) {
        list->tail = tmp;
    }
    list->size++;
}

gap popFront(DblLinkedList *list) {
    Node *prev;
    gap tmp;
    if (list->head == NULL) {
        exit(2);
    }

    prev = list->head;
    list->head = list->head->next;
    if (list->head) {
        list->head->prev = NULL;
    }
    if (prev == list->tail) {
        list->tail = NULL;
    }
    tmp = prev->value;
    free(prev);

    list->size--;
    return tmp;
}

void pushBack(DblLinkedList *list, gap value) {
    Node *tmp = (Node*) malloc(sizeof(Node));
    if (tmp == NULL) {
        exit(3);
    }
    tmp->value = value;
    tmp->next = NULL;
    tmp->prev = list->tail;
    if (list->tail) {
        list->tail->next = tmp;
    }
    list->tail = tmp;

    if (list->head == NULL) {
        list->head = tmp;
    }
    list->size++;
}

void insertBeforeElement(DblLinkedList *list, Node* elm, gap value) {
    Node *ins = NULL;
    if (elm == NULL) {
        exit(6);
    }

    if (!elm->prev) {
        pushFront(list, value);
        return;
    }
    ins = (Node*) malloc(sizeof(Node));
    ins->value = value;
    ins->prev = elm->prev;
    elm->prev->next = ins;
    ins->next = elm;
    elm->prev = ins;

    list->size++;
}

void insertionSort(DblLinkedList **list, int (*cmp)(void*, void*)) {
    DblLinkedList *out = createDblLinkedList();
    Node *sorted = NULL;
    Node *unsorted = NULL;

    pushFront(out, popFront(*list));

    unsorted = (*list)->head;
    while (unsorted) {
        sorted = out->head;
        while (sorted && !cmp(&(unsorted->value), &(sorted->value))) {
            sorted = sorted->next;
        }
        if (sorted) {
            insertBeforeElement(out, sorted, unsorted->value);
        } else {
            pushBack(out, unsorted->value);
        }
        unsorted = unsorted->next;
    }

    free(*list);
    *list = out;
}

void swap(Node **first, Node **second) {
    Node *tmp = *first;
    *first = *second;
    *second = tmp;
}

void swapNodes(DblLinkedList *list, int key1, int key2) {
    if ( key1 == key2 ) return;

    Node **first = &(list->head);

    while ( *first && ( *first )->value != (getNth(list, key1))->value ) first = &( *first )->next;

    if ( *first == NULL ) return;

    Node **second = headr;

    while ( *second && ( *second )->value != key2 ) second = &( *second )->next;

    if ( *second == NULL ) return;

    swap( first, second );
    swap( &( *first )->next, &( *second )->next );
}

void ReverseList(DblLinkedList *list) {
    Node *p = list->head;

    while (p != list->tail)
    {
        Node *next = p->next;

        swap(p->next, p->prev);

        p = next;
    }

    //swap(list->head->next, list->tail->prev);
    //swap(list->head->next->prev, list->tail->prev->next);
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

void print_structs(DblLinkedList *list) {
    if (list->size == 0) {
        puts("There are no structures :c");
        return;
    }

    Node *tmp = list->head;
    int i = 0;

    while (tmp) {
        printf("Struct index: %d\n", i);
        printf("Type: ");
        print_type_string(tmp->value.gap_type);

        if (tmp->value.gap_type == LINE)
            printf("Length: %.3f\n", tmp->value.length);

        if (tmp->value.left_dot == DBL_MIN) {
            printf("Left dot: infinity\n");
        } else {
            printf("Left dot: %.3f\n", tmp->value.left_dot);
        }

        if (tmp->value.right_dot == DBL_MAX) {
            printf("Right dot: infinity\n\n");
        } else {
            printf("Right dot: %.3f\n\n", tmp->value.right_dot);
        }

        tmp = tmp->next;
        ++i;
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
