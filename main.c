#include <stdio.h>
#include <stdlib.h>
#include <time.h>

//15 OP
void sequence_border(const int array[], int array_len, int *start, int *end);
void print_sequence(int array[], int start, int end);

int main(void)
{
    int *array;
    int array_len;

    srand(time(NULL));

    puts("Enter max array len:");
    scanf("%d", &array_len);
    array = malloc(array_len * sizeof(int));

    for (int i = 0; i < array_len; ++i)
    {
        array[i] = rand() % 20;
    }

    puts("Numbers in massive:");
    for (int i = 0; i < array_len; ++i)
    {
        printf("%d ", array[i]);
    }

    puts("\n");

    int start, end;
    sequence_border(array, array_len, &start, &end);

    printf("Maximum non-decreasing sequence\n");
    print_sequence(array, start, end);

    free(array);

    return 0;
}

void sequence_border(const int array[], int array_len, int *start, int *end)
{
    int s1 = 0, f1 = 0;
    int s2 = 0, f2, diff;

    for (int i = 1; i < array_len; ++i)
    {
        if (array[i] >= array[i - 1])
        {
            f2 = i;
            diff = f2 - s2 + 1;
            if (f1 - s1 + 1 <= diff) {
                s1 = s2;
                f1 = f2;
            }
        } else
        {
            s2 = i;
        }
    }

    *start = s1;
    *end = f1;
}

void print_sequence(int array[], int start, int end)
{
    for (int i = start; i <= end; ++i)
    {
        printf("%d ", array[i]);
    }
}