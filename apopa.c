#include <stdio.h>
#include <stdlib.h>
#include <math.h>
#include <string.h>
#include <time.h>
#define MAX_INPUT_ARRAY_LEN 11
#define RGB_COUNT 3

unsigned char *** create_rgb_array(int row, int column);
unsigned char ** create_grayscale_array(unsigned char ***array, int arr_row,
                                        int arr_column);
unsigned char made_grayscale_color(unsigned char red, unsigned char green,
                                   unsigned char blue);

void delete_rgb_array(unsigned char ***array, int row, int column);
void print_rgb_array(unsigned char ***array, int row, int column);
void delete_grayscale_array(unsigned char **array, int row);
void print_grayscale_array(unsigned char **array, int row, int column);

int input_int_check(char value_array[]);
int input_int();

int main(void) {
    unsigned char ***rgb_array = NULL;
    unsigned char **grayscale_array = NULL;
    int rgb_row, rgb_column, grs_row, grs_column;
    int menu_choice;

    srand(time(NULL));

    do {
        puts("1. Enter array len and fill it with random numbers");
        puts("2. Create Grayscale array");
        puts("3. Print RGB array");
        puts("4. Print Grayscale array");
        puts("5. Exit");
        
        puts("Enter value:");
        menu_choice = input_int();
        switch (menu_choice) {
            case 1:
                puts("Enter row len:");
                rgb_row = input_int();
                puts("Enter column len:");
                rgb_column = input_int();
                rgb_array = create_rgb_array(rgb_row, rgb_column);
                puts("RGB array created\n");

                break;
            case 2:
                if (rgb_array != NULL) {
                    grs_row = rgb_row;
                    grs_column = rgb_column;

                    grayscale_array = create_grayscale_array(rgb_array,
                                                             grs_row,
                                                             grs_column);
                    puts("Grayscale array created\n");
                } else {
                    fputs("Grayscale array don't exist!\n", stderr);
                }

                break;
            case 3:
                if (rgb_array != NULL)
                    print_rgb_array(rgb_array, rgb_row,
                                    rgb_column);
                else
                    fputs("RGB array don't exist!\n", stderr);

                break;
            case 4:
                if (grayscale_array != NULL)
                    print_grayscale_array(grayscale_array, grs_row,
                                          grs_column);
                else
                    fputs("Grayscale array don't exist!\n", stderr);

                break;
            case 5:
                if (rgb_array != NULL)
                    delete_rgb_array(rgb_array, rgb_row,rgb_column);
                if (grayscale_array != NULL)
                    delete_grayscale_array(grayscale_array, grs_row);

                break;
            default:
                puts("Not in menu!\n");
        }

    } while (menu_choice != 5);

    return 0;
}

unsigned char *** create_rgb_array(int row, int column) {
    unsigned char ***array = malloc(row * sizeof(int **));
    for (int i = 0; i < row; ++i) {
        array[i] = malloc(column * sizeof(int *));
        for (int j = 0; j < column; ++j) {
            array[i][j] = malloc(RGB_COUNT * sizeof(char));
            for (int k = 0; k < RGB_COUNT; ++k) {
                array[i][j][k] = (unsigned char) (rand() % 256);
            }
        }
    }

    return array;
}

void delete_rgb_array(unsigned char ***array, int row, int column) {
    for (int i = 0; i < row; ++i) {
        for (int j = 0; j < column; ++j) {
            free(array[i][j]);
        }
        free(array[i]);
    }

    free(array);
}

void print_rgb_array(unsigned char ***array, int row, int column) {
    for (int i = 0; i < row; ++i) {
        for (int j = 0; j < column; ++j) {
            for (int k = 0; k < 3; ++k) {
                printf("|%3.3d", array[i][j][k]);
            }
            printf("|\t");
        }
        printf("\n\n");
    }
}

unsigned char ** create_grayscale_array(unsigned char ***array,
                                        int arr_row, int arr_column) {
    unsigned char **grayscale_array = malloc(arr_row * sizeof(int *));
    for (int i = 0; i < arr_row; ++i) {
        grayscale_array[i] = malloc(arr_column * (sizeof(unsigned char)));
        for (int j = 0; j < arr_column; ++j) {
            unsigned char red = array[i][j][0];
            unsigned char green = array[i][j][1];
            unsigned char blue = array[i][j][2];

            grayscale_array[i][j] = made_grayscale_color(red, green, blue);
        }
    }

    return grayscale_array;
}

void print_grayscale_array(unsigned char **array, int row, int column) {
    for (int i = 0; i < row; ++i) {
        for (int j = 0; j < column; ++j) {
            printf("|%3.3d", array[i][j]);
        }
        printf("|\n");
    }
    printf("\n");
}

void delete_grayscale_array(unsigned char **array, int row) {
    for (int i = 0; i < row; ++i) {
        free(array[i]);
    }
    free(array);
}

int input_int() {
    char str_array_len[MAX_INPUT_ARRAY_LEN];
    fgets(str_array_len, MAX_INPUT_ARRAY_LEN, stdin);
    fflush(stdin);
    if (!(input_int_check(str_array_len))
        || strlen(str_array_len) == 1) {
        return -1;
    }

    return strtol(str_array_len, NULL, 10);
}

int input_int_check(char value_array[]) {
    for (int i = 0; i < strlen(value_array) - 1; ++i) {
        if ((value_array[i] < '0') || (value_array[i] > '9')) {
            return 0;
        }
    }
    return 1;
}

unsigned char made_grayscale_color(unsigned char red, unsigned char green,
                                   unsigned char blue) {
    return (unsigned char) round(0.3 * red + 0.59 * green + 0.11 * blue);
}