/*
 * @ author: Alaa Abdelmoneim
 * @ date  : 3/14/2024
 * @ breif : implementation of the single thread for calculation each cell in the resultant matrix
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

typedef struct cell_index {
    int i;
    int j;
} cell_index_t;

void* thread_function_cells(void* arg) {
    cell_index_t* cell_index = (cell_index_t*)arg;
    int total = 0;
    for (int k = 0; k < row2; k++) {
        total += mat1[cell_index->i][k] * mat2[k][cell_index->j];
    }
    result[cell_index->i][cell_index->j] = total;
    free(cell_index); // Free memory allocated for cell_index
    return NULL;
}

void multiply_matrices_cell_threads() {
    result_number_of_rows = row1;
    result_number_of_cols = col2;
    pthread_t threads[row1 * col2]; // Allocate enough space for all threads

    int thread_count = 0;
    for (int i = 0; i < row1; i++) {
        for (int j = 0; j < col2; j++) {
            // Allocate memory for each cell_index_t object
            cell_index_t* cell_index = (cell_index_t*)malloc(sizeof(cell_index_t));
            cell_index->i = i;
            cell_index->j = j;
            // Create thread for each cell
            pthread_create(&threads[thread_count], NULL, thread_function_cells, cell_index);
            thread_count++;
        }
    }

    // Wait for all threads to finish
    for (int i = 0; i < row1 * col2; i++) {
        pthread_join(threads[i], NULL);
    }
}

void read_matrix_from_file(const char* filename, int matrix[][MAX_COLS], int* row, int* col) {
    FILE* file = fopen(filename, "r");
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

    multiply_matrices_cell_threads();

    gettimeofday(&stop, NULL);

    printf("Seconds taken for cell threads: %lu\n", stop.tv_sec - start.tv_sec);
    printf("Microseconds taken for cell threads : %lu\n", stop.tv_usec - start.tv_usec);
    printf("Result of matrix multiplication:\n");
    for (int i = 0; i < result_number_of_rows; i++) {
        for (int j = 0; j < result_number_of_cols; j++) {
            printf("%d\t", result[i][j]);
        }
        printf("\n");
    }

    return 0;
}
