#include <stdio.h>
#include <stdlib.h>
#include <float.h>
#include "gap_structs.h"
#include "input_functions.h"

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

void print_with_filter(DblLinkedList *list);

DblLinkedList* create_dblLinkedList();

void delete_dblLinkedList(DblLinkedList **list);

Node* get_node(DblLinkedList *list, size_t index);

void push_front(DblLinkedList *list, gap data);

void push_back(DblLinkedList *list, gap value);

void insert_before_element(DblLinkedList *list, Node* elm, gap value);

void insertion_sort(DblLinkedList **list, int (*cmp)(void*, void*));

gap pop_front(DblLinkedList *list);

void swap(Node* node);

void reverse_DDL(DblLinkedList *list);

int save(DblLinkedList *list);

int load();

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
    DblLinkedList *list = create_dblLinkedList();

    int menu_input;
    do {
        print_menu();
        puts("Enter value:");
        menu_input = input_int();
        switch (menu_input) {
            case FIRST:
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

                insertion_sort(&list, comparator);

                clear_flags();

                break;
            case FIVE:
                field_input();

                print_with_filter(list);

                clear_flags();

                break;
            case SIX:
                reverse_DDL(list);
                puts("List reversed");

                break;
            case SEVEN:
                save(list);
                puts("List saved");

                break;
            case EIGHT:
                load();

                break;
            case NINE:
                delete_dblLinkedList(&list);

                break;
            default:
                puts("Incorrect value!");

                break;
        }

    } while (menu_input != NINE);

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

DblLinkedList* create_dblLinkedList() {
    DblLinkedList *tmp = (DblLinkedList*) malloc(sizeof(DblLinkedList));
    tmp->size = 0;
    tmp->head = tmp->tail = NULL;

    return tmp;
}

void delete_dblLinkedList(DblLinkedList **list) {
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

Node* get_node(DblLinkedList *list, size_t index) {
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
    elm = get_node(list, index);
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

void push_front(DblLinkedList *list, gap data) {
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

gap pop_front(DblLinkedList *list) {
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

void push_back(DblLinkedList *list, gap value) {
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

void insert_before_element(DblLinkedList *list, Node* elm, gap value) {
    Node *ins = NULL;
    if (elm == NULL) {
        exit(6);
    }

    if (!elm->prev) {
        push_front(list, value);
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

void insertion_sort(DblLinkedList **list, int (*cmp)(void*, void*)) {
    DblLinkedList *out = create_dblLinkedList();
    Node *sorted = NULL;
    Node *unsorted = NULL;

    push_front(out, pop_front(*list));

    unsorted = (*list)->head;
    while (unsorted) {
        sorted = out->head;
        while (sorted && !cmp(&(unsorted->value), &(sorted->value))) {
            sorted = sorted->next;
        }
        if (sorted) {
            insert_before_element(out, sorted, unsorted->value);
        } else {
            push_back(out, unsorted->value);
        }
        unsorted = unsorted->next;
    }

    free(*list);
    *list = out;
}

void swap(Node* node) {
    Node* prev = node->prev;
    node->prev = node->next;
    node->next = prev;
}

void reverse_DDL(DblLinkedList *list) {
    Node* prev = NULL;
    Node* curr = list->head;

    while (curr != NULL) {
        swap(curr);

        prev = curr;

        curr = curr->prev;
    }

    if (prev != NULL) {
        list->head = prev;
    }
}

int save(DblLinkedList *list) {
    size_t n = list->size;
    gap *st = (gap *) malloc(n * sizeof(gap));

    Node *tmp = list->head;
    int j = 0;
    while (tmp) {
        st[j] = tmp->value;

        ++j;
        tmp = tmp->next;
    }

    FILE *fp = fopen("gap_s.dat", "w");
    char *c;

    int size = n * sizeof(gap);

    c = (char *) &n;
    for (int i = 0; i < sizeof(int); ++i) {
        putc(*c++, fp);
    }

    c = (char *) st;
    for (int i = 0; i < size; ++i) {
        putc(*c, fp);
        c++;
    }
    fclose(fp);
    free(st);
    return 0;
}

int load() {
    FILE *fp = fopen("gap_s.dat", "r");
    char *c;
    int m = sizeof(int);
    int n, i;

    int *pti = (int *) malloc(m);

    c = (char *) pti;
    while (m > 0) {
        i = getc(fp);
        if (i == EOF) break;
        *c = i;
        c++;
        m--;
    }
    n = *pti;

    gap *ptr = (gap *) malloc(n * sizeof(gap));
    c = (char *) ptr;
    while ((i = getc(fp)) != EOF) {
        *c = i;
        c++;
    }

    for (int k = 0; k < n; ++k) {
        print_type_string((ptr + k)->gap_type);
        printf("Left dot: %.3f\nRight dot: %.3f\n", (ptr + k)->left_dot,
               (ptr + k)->right_dot);
        puts("");
    }

    free(pti);
    free(ptr);
    fclose(fp);
    return 0;
}

void print_with_filter(DblLinkedList *list) {
    Node *tmp = list->head;

    while (tmp) {
        if (Params.is_line) {
            printf("Type: ");
            print_type_string(tmp->value.gap_type);
        }
        if (Params.is_left_dot)
            printf("Left dot: %.3f\n", tmp->value.left_dot);
        if (Params.is_right_dot)
            printf("Right dot: %.3f\n", tmp->value.right_dot);
        if (Params.is_length && tmp->value.gap_type == LINE)
            printf("Length: %.3f\n", tmp->value.length);

        tmp = tmp->next;
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
    puts("6. Reversed list");
    puts("7. Save list");
    puts("8. Load list (print)");
    puts("9. Exit");
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
