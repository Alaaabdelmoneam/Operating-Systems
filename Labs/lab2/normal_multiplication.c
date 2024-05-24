/*
 * @ author: Alaa Abdelmoneim
 * @ date  : 3/14/2024
 * @ breif : implementation of the normal matrix multiplication without the use of threads
*/

#include <stdio.h>
#include <stdlib.h>
#include <sys/time.h>
#include <pthread.h>


#define MAX_ROWS 100
#define MAX_COLS 100

int  result[MAX_ROWS][MAX_COLS];
char result_number_of_cols;
char result_number_of_rows;

int mat1[MAX_ROWS][MAX_COLS];
int mat2[MAX_ROWS][MAX_COLS];
int row1, col1, row2, col2;


void multiply_matrices(int mat1[][MAX_COLS], int mat2[][MAX_COLS], int result[][MAX_COLS], int row1, int col1, int row2, int col2) {
    if (col1 != row2) {
        printf("Matrices cannot be multiplied due to incompatible dimensions.\n");
        result_number_of_cols = 0;
        result_number_of_rows = 0;
        return;
    }
    result_number_of_rows = row1;
    result_number_of_cols = col2;
    for (int i = 0; i < row1; i++) {
        for (int j = 0; j < col2; j++) {
            result[i][j] = 0;
            for (int k = 0; k < col1; k++) {
                result[i][j] += mat1[i][k] * mat2[k][j];
            }
        }
    }
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


void* thread_function(void* arg){
    int row_number = *((int *)arg);
    int total = 0;
    for (int m = 0; m < col2; m++){
        for (int n = 0; n < row2; n++){
            total += mat1[row_number][n] * mat2[m][n];

        }
        result[row_number][m] = total;
        //printf("total is = %d\n",total);
        total = 0;
    }

}

void multiply_matrices_row_threads() {
    pthread_t threads[row1];
    for (int i = 0; i < row1; i++){
        pthread_create(&threads[i], NULL, thread_function, &i);
    }
    for (int i = 0; i < row1; i++){
        pthread_join(threads[i], NULL);
    }

}

int main() {
    struct timeval stop, start;
    read_matrix_from_file("a.txt", mat1, &row1, &col1);
    read_matrix_from_file("b.txt", mat2, &row2, &col2);

    gettimeofday(&start, NULL); //start checking time



    multiply_matrices(mat1, mat2, result, row1, col1, row2, col2);

    //multiply_matrices_from_files("a.txt", "b.txt", result);

    gettimeofday(&stop, NULL);

    printf("Seconds taken for normal matrix multiplication %lu\n", stop.tv_sec - start.tv_sec);
    printf("Microseconds taken for normal matrix multiplication: %lu\n", stop.tv_usec - start.tv_usec);



//    gettimeofday(&start, NULL); //start checking time



    // multiply_matrices_row_threads(mat1, mat2, result, row1, col1, row2, col2);

//    gettimeofday(&stop, NULL);

//    printf("Seconds taken for row thread multiplication %lu\n", stop.tv_sec - start.tv_sec);
//    printf("Microseconds taken for row thread multiplication: %lu\n", stop.tv_usec - start.tv_usec);



    printf("Result of matrix multiplication:\n");
    for (int i = 0; i < result_number_of_cols; i++) {
        for (int j = 0; j < result_number_of_rows; j++) {
            printf("%d\t", result[i][j]);
        }
        printf("\n");
    }

    return 0;
}
