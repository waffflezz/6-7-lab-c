#include <stdio.h>
#include <stdlib.h>
#include <time.h>
#include <string.h>
#define MAX_INPUT_ARRAY_LEN 11

//15 OP
void sequence_border(const int array[], int array_len, int *start, int *end);
void print_sequence(int array[], int start, int end);
int input_int_check(char value_array[]);
int input_int();

int main(void) {
    int *array;
    int array_len;

    //update seed for random function
    srand(time(NULL));

    puts("Enter max array len:");
    array_len = input_int();
    array = malloc(array_len * sizeof(int));

    for (int i = 0; i < array_len; ++i) {
        array[i] = rand() % 20;
    }

    puts("Numbers in massive:");
    if (array_len < 100) {
        for (int i = 0; i < array_len; ++i) {
            printf("%d ", array[i]);
        }
    } else {
        for (int i = 0; i < 100; ++i) {
            printf("%d ", array[i]);
        }
        printf("...");
    }

    puts("\n");

    int start, end;
    sequence_border(array, array_len, &start, &end);

    printf("Maximum non-decreasing sequence\n");
    print_sequence(array, start, end);

    free(array);




    return 0;
}

int input_int() {
    char str_array_len[MAX_INPUT_ARRAY_LEN];
    fgets(str_array_len, MAX_INPUT_ARRAY_LEN, stdin);
    if (!(input_int_check(str_array_len)) || strlen(str_array_len) == 1) {
        fputs("You enter incorrect value. Please, enter integer!\n", stderr);
        return -1;
    }

    return strtol(str_array_len, NULL, 10);
}

void sequence_border(const int array[], int array_len, int *start, int *end) {
    int start_ptr = 0, end_ptr = 0;
    int next_start_ptr = 0, next_end_ptr, diff;

    for (int i = 1; i < array_len; ++i) {
        if (array[i] >= array[i - 1]) {
            next_end_ptr = i;
            diff = next_end_ptr - next_start_ptr + 1;
            if (end_ptr - start_ptr + 1 <= diff) {
                start_ptr = next_start_ptr;
                end_ptr = next_end_ptr;
            }
        } else {
            next_start_ptr = i;
        }
    }

    *start = start_ptr;
    *end = end_ptr;
}

void print_sequence(int array[], int start, int end) {
    for (int i = start; i <= end; ++i) {
        printf("%d ", array[i]);
    }
}

int input_int_check(char value_array[]) {
    for (int i = 0; i < strlen(value_array) - 1; ++i) {
        if ((value_array[i] < '0') || (value_array[i] > '9')) {
            return 0;
        }
    }
    return 1;
}
