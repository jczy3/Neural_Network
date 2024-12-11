#pragma once

#include "matrix.h" 

typedef struct {
    int id;
    Matrix* m1;
    Matrix* m2;
    Matrix* result;
    double scalar; 
    double (*func)(double); 
} Task;

// Modified operation functions
Matrix* dot_runner(Matrix *m1, Matrix *m2);
Matrix* apply_runner(double (*func)(double), Matrix* m);
Matrix* subtract_runner(Matrix *m1, Matrix *m2);
Matrix* add_runner(Matrix *m1, Matrix *m2);
Matrix* transpose_runner(Matrix *m);
Matrix* multiply_runner(Matrix *m1, Matrix *m2);
Matrix* scale_runner(double n, Matrix *m);