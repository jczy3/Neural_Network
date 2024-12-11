#include "ops.h"
#include <stdlib.h>
#include <stdio.h>
#include <pthread.h>

#define NUM_THREADS 4

// Operation functions using threading
void* dot_worker(void* arg) {
    Task* task = (Task*) arg;
    for (int i = task->id; i < task->result->rows; i += NUM_THREADS) {
        for (int j = 0; j < task->result->cols; j++) {
            double sum = 0;
            for (int k = 0; k < task->m1->cols; k++) {
                sum += task->m1->entries[i][k] * task->m2->entries[k][j];
            }
            task->result->entries[i][j] = sum;
        }
    }
    return NULL;
}

void* apply_worker(void* arg) {
    Task* task = (Task*) arg;
    for (int i = task->id; i < task->m1->rows; i += NUM_THREADS) {
        for (int j = 0; j < task->m1->cols; j++) {
            task->result->entries[i][j] = task->func(task->m1->entries[i][j]);
        }
    }
    return NULL;
}

void* subtract_worker(void* arg) {
    Task* task = (Task*) arg;
    for (int i = task->id; i < task->result->rows; i += NUM_THREADS) {
        for (int j = 0; j < task->result->cols; j++) {
            task->result->entries[i][j] = task->m1->entries[i][j] - task->m2->entries[i][j];
        }
    }
    return NULL;
}

void* add_worker(void* arg) {
    Task* task = (Task*) arg;
    for (int i = task->id; i < task->result->rows; i += NUM_THREADS) {
        for (int j = 0; j < task->result->cols; j++) {
            task->result->entries[i][j] = task->m1->entries[i][j] + task->m2->entries[i][j];
        }
    }
    return NULL;
}

void* transpose_worker(void* arg) {
    Task* task = (Task*) arg;
    for (int i = task->id; i < task->m1->rows; i += NUM_THREADS) {
        for (int j = 0; j < task->m1->cols; j++) {
            task->result->entries[j][i] = task->m1->entries[i][j];
        }
    }
    return NULL;
}

void* multiply_worker(void* arg) {
    Task* task = (Task*) arg;
    for (int i = task->id; i < task->result->rows; i += NUM_THREADS) {
        for (int j = 0; j < task->result->cols; j++) {
            task->result->entries[i][j] = task->m1->entries[i][j] * task->m2->entries[i][j];
        }
    }
    return NULL;
}

void* scale_worker(void* arg) {
    Task* task = (Task*) arg;
    for (int i = task->id; i < task->result->rows; i += NUM_THREADS) {
        for (int j = 0; j < task->result->cols; j++) {
            task->result->entries[i][j] = task->m1->entries[i][j] * task->scalar;
        }
    }
    return NULL;
}

Matrix* dot_runner(Matrix *m1, Matrix *m2) {
    Matrix *result = matrix_create(m1->rows, m2->cols);
    pthread_t threads[NUM_THREADS];
    Task tasks[NUM_THREADS];

    for (int i = 0; i < NUM_THREADS; i++) {
        tasks[i] = (Task){.id = i, .m1 = m1, .m2 = m2, .result = result};
        pthread_create(&threads[i], NULL, dot_worker, &tasks[i]);
    }
    for (int i = 0; i < NUM_THREADS; i++) {
        pthread_join(threads[i], NULL);
    }
    return result;
}

Matrix* apply_runner(double (*func)(double), Matrix* m) {
    Matrix *result = matrix_copy(m);
    pthread_t threads[NUM_THREADS];
    Task tasks[NUM_THREADS];

    for (int i = 0; i < NUM_THREADS; i++) {
        tasks[i] = (Task){.id = i, .m1 = m, .result = result, .func = func};
        pthread_create(&threads[i], NULL, apply_worker, &tasks[i]);
    }
    for (int i = 0; i < NUM_THREADS; i++) {
        pthread_join(threads[i], NULL);
    }
    return result;
}

Matrix* subtract_runner(Matrix *m1, Matrix *m2) {
    Matrix *result = matrix_create(m1->rows, m1->cols);
    pthread_t threads[NUM_THREADS];
    Task tasks[NUM_THREADS];

    for (int i = 0; i < NUM_THREADS; i++) {
        tasks[i] = (Task){.id = i, .m1 = m1, .m2 = m2, .result = result};
        pthread_create(&threads[i], NULL, subtract_worker, &tasks[i]);
    }
    for (int i = 0; i < NUM_THREADS; i++) {
        pthread_join(threads[i], NULL);
    }
    return result;
}

Matrix* add_runner(Matrix *m1, Matrix *m2) {
    Matrix *result = matrix_create(m1->rows, m1->cols);
    pthread_t threads[NUM_THREADS];
    Task tasks[NUM_THREADS];

    for (int i = 0; i < NUM_THREADS; i++) {
        tasks[i] = (Task){.id = i, .m1 = m1, .m2 = m2, .result = result};
        pthread_create(&threads[i], NULL, add_worker, &tasks[i]);
    }
    for (int i = 0; i < NUM_THREADS; i++) {
        pthread_join(threads[i], NULL);
    }
    return result;
}

Matrix* transpose_runner(Matrix *m) {
    Matrix *result = matrix_create(m->cols, m->rows);
    pthread_t threads[NUM_THREADS];
    Task tasks[NUM_THREADS];

    for (int i = 0; i < NUM_THREADS; i++) {
        tasks[i] = (Task){.id = i, .m1 = m, .result = result};
        pthread_create(&threads[i], NULL, transpose_worker, &tasks[i]);
    }
    for (int i = 0; i < NUM_THREADS; i++) {
        pthread_join(threads[i], NULL);
    }
    return result;
}

Matrix* multiply_runner(Matrix *m1, Matrix *m2) {
    Matrix *result = matrix_create(m1->rows, m1->cols);
    pthread_t threads[NUM_THREADS];
    Task tasks[NUM_THREADS];

    for (int i = 0; i < NUM_THREADS; i++) {
        tasks[i] = (Task){.id = i, .m1 = m1, .m2 = m2, .result = result};
        pthread_create(&threads[i], NULL, multiply_worker, &tasks[i]);
    }
    for (int i = 0; i < NUM_THREADS; i++) {
        pthread_join(threads[i], NULL);
    }
    return result;
}

Matrix* scale_runner(double scalar, Matrix *m) {
    Matrix *result = matrix_copy(m);
    pthread_t threads[NUM_THREADS];
    Task tasks[NUM_THREADS];

    for (int i = 0; i < NUM_THREADS; i++) {
        tasks[i] = (Task){.id = i, .m1 = m, .result = result, .scalar = scalar};
        pthread_create(&threads[i], NULL, scale_worker, &tasks[i]);
    }
    for (int i = 0; i < NUM_THREADS; i++) {
        pthread_join(threads[i], NULL);
    }
    return result;
}
