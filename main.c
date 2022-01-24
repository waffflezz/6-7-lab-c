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

    printf("\n\n");

    int s1 = 0, f1 = 0;
    int s2 = 0, f2 = 0, diff = 0;

    for (int i = 1; i < array_len; ++i) {
        if (array[i] >= array[i - 1]) {
            f2 = i;
            diff = f2 - s2 + 1;
            if (f1 - s1 + 1 <= diff) {
                s1 = s2;
                f1 = f2;
            }
        } else {
            s2 = i;
        }
    }

    printf("Maximum non-decreasing sequence\n");
    for (int i = s1; i <= f1; ++i) {
        printf("%d ", array[i]);
    }


    return 0;
}