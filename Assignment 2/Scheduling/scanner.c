#include <stdbool.h>
#include <stdio.h>
#include <stdlib.h>

#include "Process.h"

int scanner(Process **processes) {
    int **matrix = NULL;
    int *count = NULL;
    int numArrays = 0;
    int numElements;

    while (1) {
        int num;
        if (scanf("%d", &num) == EOF)  // Check for EOF here
            break;

        matrix = (int **)realloc(matrix, (numArrays + 1) * sizeof(int *));
        if (matrix == NULL) {
            printf("Memory allocation failed!\n");
            exit(1);
        }
        matrix[numArrays] = NULL;  // Initialize the pointer to NULL

        numElements = 0;
        while (1) {
            if (num == -1) {
                matrix[numArrays] = (int *)realloc(matrix[numArrays], (numElements + 1) * sizeof(int));
                if (matrix[numArrays] == NULL) {
                    printf("Memory allocation failed!\n");
                    exit(1);
                }
                matrix[numArrays][numElements] = num;
                numElements++;
                break;
            }

            matrix[numArrays] = (int *)realloc(matrix[numArrays], (numElements + 1) * sizeof(int));
            if (matrix[numArrays] == NULL) {
                printf("Memory allocation failed!\n");
                exit(1);
            }
            matrix[numArrays][numElements] = num;
            numElements++;

            scanf("%d", &num);
        }

        count = (int *)realloc(count, (numArrays + 1) * sizeof(int));
        if (count == NULL) {
            printf("Memory allocation failed!\n");
            exit(1);
        }
        count[numArrays] = numElements;
        numArrays++;
    }

    (*processes) = malloc(numArrays * sizeof(Process));
    if (*processes == NULL) {  // Check if malloc failed
        printf("Memory allocation failed!\n");
        exit(1);
    }

    for (int i = 0; i < numArrays; i++) {
        (*processes)[i] = new_process(count[i], matrix[i]);
    }

    for (int i = 0; i < numArrays; i++) {
        free(matrix[i]);
    }

    free(matrix);
    free(count);

    return numArrays;
}
