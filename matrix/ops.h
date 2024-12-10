#pragma once

#include "matrix.h" // Ensure access to the Matrix structure definition

typedef struct {
    int id;
    Matrix* m1;
    Matrix* m2;
    Matrix* result;
    double scalar; // Used for scale_runner
    double (*func)(double); // Used for apply_runner
} Task;

// Function prototypes for pthread-based matrix operations
Matrix* dot_runner(Matrix *m1, Matrix *m2);
Matrix* apply_runner(double (*func)(double), Matrix* m);
Matrix* subtract_runner(Matrix *m1, Matrix *m2);
Matrix* add_runner(Matrix *m1, Matrix *m2);
Matrix* transpose_runner(Matrix *m);
Matrix* multiply_runner(Matrix *m1, Matrix *m2);
Matrix* scale_runner(double n, Matrix *m);