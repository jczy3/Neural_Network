#pragma once

typedef struct {
	double** entries;
	int rows;
	int cols;
} Matrix;

// Existing functions
Matrix* matrix_create(int row, int col);
void matrix_fill(Matrix *m, int n);
void matrix_free(Matrix *m);
void matrix_print(Matrix *m);
Matrix* matrix_copy(Matrix *m);
void matrix_save(Matrix* m, char* file_string);
Matrix* matrix_load(char* file_string);
void matrix_randomize(Matrix* m, int n);
int matrix_argmax(Matrix* m);
Matrix* matrix_flatten(Matrix* m, int axis);

// 🔥 Added missing function declarations
void dot(Matrix* a, Matrix* b, Matrix* result);
void add(Matrix* a, Matrix* b, Matrix* result);
void subtract(Matrix* a, Matrix* b, Matrix* result);
void multiply(Matrix* a, Matrix* b, Matrix* result);
void scale(Matrix* a, double scalar);
void transpose(Matrix* a, Matrix* result);
void apply(Matrix* a, double (*func)(double));
