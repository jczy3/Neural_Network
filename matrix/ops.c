#include <stdio.h>
#include <stdlib.h>
#include <pthread.h>

typedef struct {
    double** entries;
    int rows;
    int cols;
} Matrix;

typedef struct {
    int id;
    int row_start;
    int row_end;
    Matrix* a;
    Matrix* b;
    Matrix* result;
    Matrix* m;
} Task;

void* multiply_task(void* arg) {
    Task* task = (Task*)arg;
    for (int i = task->row_start; i < task->row_end; i++) {
        for (int j = 0; j < task->b->cols; j++) {
            task->result->entries[i][j] = 0;
            for (int k = 0; k < task->a->cols; k++) {
                task->result->entries[i][j] += task->a->entries[i][k] * task->b->entries[k][j];
            }
        }
    }
    return NULL;
}

void matrix_multiply_parallel(Matrix* a, Matrix* b, Matrix* result, int ntasks) {
    pthread_t threads[ntasks];
    Task tasks[ntasks];
    int rows_per_task = a->rows / ntasks;

    for (int i = 0; i < ntasks; i++) {
        int row_start = i * rows_per_task;
        int row_end = (i == ntasks - 1) ? a->rows : row_start + rows_per_task;

        tasks[i] = (Task){ .id = i, .row_start = row_start, .row_end = row_end, .a = a, .b = b, .result = result };
        pthread_create(&threads[i], NULL, multiply_task, &tasks[i]);
    }

    for (int i = 0; i < ntasks; i++) {
        pthread_join(threads[i], NULL);
    }
}

void* add_task(void* arg) {
    Task* task = (Task*)arg;
    for (int i = task->row_start; i < task->row_end; i++) {
        for (int j = 0; j < task->a->cols; j++) {
            task->result->entries[i][j] = task->a->entries[i][j] + task->b->entries[i][j];
        }
    }
    return NULL;
}

void matrix_add_parallel(Matrix* a, Matrix* b, Matrix* result, int ntasks) {
    pthread_t threads[ntasks];
    Task tasks[ntasks];
    int rows_per_task = a->rows / ntasks;

    for (int i = 0; i < ntasks; i++) {
        int row_start = i * rows_per_task;
        int row_end = (i == ntasks - 1) ? a->rows : row_start + rows_per_task;

        tasks[i] = (Task){ .id = i, .row_start = row_start, .row_end = row_end, .a = a, .b = b, .result = result };
        pthread_create(&threads[i], NULL, add_task, &tasks[i]);
    }

    for (int i = 0; i < ntasks; i++) {
        pthread_join(threads[i], NULL);
    }
}

void* subtract_task(void* arg) {
    Task* task = (Task*)arg;
    for (int i = task->row_start; i < task->row_end; i++) {
        for (int j = 0; j < task->a->cols; j++) {
            task->result->entries[i][j] = task->a->entries[i][j] - task->b->entries[i][j];
        }
    }
    return NULL;
}

void matrix_subtract_parallel(Matrix* a, Matrix* b, Matrix* result, int ntasks) {
    pthread_t threads[ntasks];
    Task tasks[ntasks];
    int rows_per_task = a->rows / ntasks;

    for (int i = 0; i < ntasks; i++) {
        int row_start = i * rows_per_task;
        int row_end = (i == ntasks - 1) ? a->rows : row_start + rows_per_task;

        tasks[i] = (Task){ .id = i, .row_start = row_start, .row_end = row_end, .a = a, .b = b, .result = result };
        pthread_create(&threads[i], NULL, subtract_task, &tasks[i]);
    }

    for (int i = 0; i < ntasks; i++) {
        pthread_join(threads[i], NULL);
    }
}

void* transpose_task(void* arg) {
    Task* task = (Task*)arg;
    for (int i = task->row_start; i < task->row_end; i++) {
        for (int j = i + 1; j < task->m->cols; j++) {
            double temp = task->m->entries[i][j];
            task->m->entries[i][j] = task->m->entries[j][i];
            task->m->entries[j][i] = temp;
        }
    }
    return NULL;
}

void matrix_transpose_parallel(Matrix* m, int ntasks) {
    pthread_t threads[ntasks];
    Task tasks[ntasks];
    int rows_per_task = m->rows / ntasks;

    for (int i = 0; i < ntasks; i++) {
        int row_start = i * rows_per_task;
        int row_end = (i == ntasks - 1) ? m->rows : row_start + rows_per_task;

        tasks[i] = (Task){ .id = i, .row_start = row_start, .row_end = row_end, .m = m };
        pthread_create(&threads[i], NULL, transpose_task, &tasks[i]);
    }

    for (int i = 0; i < ntasks; i++) {
        pthread_join(threads[i], NULL);
    }
}

