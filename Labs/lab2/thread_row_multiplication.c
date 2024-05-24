/*
 * @ author: Alaa Abdelmoneim
 * @ date  : 3/14/2024
 * @ breif : implementation of the single thread for calculation each row in the resultant matrix
*/

#include <stdio.h>
#include <stdlib.h>
#include <pthread.h>
#include <sys/time.h>

#define MAX_ROWS 100
#define MAX_COLS 100

int result[MAX_ROWS][MAX_COLS];
int result_number_of_cols;
int result_number_of_rows;

int mat1[MAX_ROWS][MAX_COLS];
int mat2[MAX_ROWS][MAX_COLS];
int row1, col1, row2, col2;



void* thread_function(void* arg);

void multiply_matrices_row_threads() {
    result_number_of_rows = row1;
    result_number_of_cols = col2;

    pthread_t threads[row1];

    for (int i = 0; i < row1; i++) {
        pthread_create(&threads[i], NULL, thread_function, (void *)i);
    }

    for (int i = 0; i < row1; i++) {
        pthread_join(threads[i], NULL);
    }
}


void* thread_function(void* arg) {
    int row_number = ((int )arg);

    for (int i = 0; i < col2; i++) {
        int total = 0;
        for (int j = 0; j < row2; j++) {
            total += mat1[row_number][j] * mat2[j][i];
        }
        result[row_number][i] = total;
    }

    return NULL;
}



void read_matrix_from_file(const char *filename, int matrix[][MAX_COLS], int *row, int *col) {
    FILE *file = fopen(filename, "r");
    if (file == NULL) {
        printf("Could not open file %s.\n", filename);
        return;
    }

    fscanf(file, "row=%d col=%d\n", row, col);
    for (int i = 0; i < *row; i++) {
        for (int j = 0; j < *col; j++) {
            fscanf(file, "%d", &matrix[i][j]);
        }
    }

    fclose(file);
}

int main() {
    struct timeval stop, start;
    read_matrix_from_file("a.txt", mat1, &row1, &col1);
    read_matrix_from_file("b.txt", mat2, &row2, &col2);

    gettimeofday(&start, NULL);


    multiply_matrices_row_threads();
    //multiply_matrices_cell_threads();

    gettimeofday(&stop, NULL);

    printf("Seconds taken: %lu\n", stop.tv_sec - start.tv_sec);
    printf("Microseconds taken: %lu\n", stop.tv_usec - start.tv_usec);
    printf("Result of matrix multiplication:\n");
    for (int i = 0; i < result_number_of_rows; i++) {
        for (int j = 0; j < result_number_of_cols; j++) {
            printf("%d\t", result[i][j]);
        }
        printf("\n");
    }

    return 0;
}
