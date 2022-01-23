#include <stdio.h>
#include <stdlib.h>

//15 вариант ОП

int main(void) {
    int *array;
    int array_len;
    int start, end;

    printf("Enter max array len:");
    scanf("%d", &array_len);
    array = malloc(array_len * sizeof(int));
    for (int i = 0; i < array_len; ++i) {
        array[i] = rand() % 20;
    }

    printf("Numbers in massive:\n");
    for (int i = 0; i < array_len; ++i) {
        printf("%d ", array[i]);
    }

    printf("Maximum non-decreasing sequence\n");
    for (int i = 0; i < array_len; ++i) {

    }


    return 0;
}