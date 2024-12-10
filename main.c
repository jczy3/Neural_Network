#include <math.h>
#include <time.h>
#include <stdio.h>
#include <stdlib.h>
#include <sys/time.h>
#include <sys/resource.h>
#include "util/img.h"
#include "neural/activations.h"
#include "neural/nn.h"
#include "matrix/matrix.h"
#include "matrix/ops.h"

int main() {
    srand(time(NULL));

    // Start measuring time
    struct timespec start_time, end_time;
    struct rusage usage_start, usage_end;

    clock_gettime(CLOCK_MONOTONIC, &start_time);
    getrusage(RUSAGE_SELF, &usage_start);

    // TRAINING
    int number_imgs = 10000;
    printf("Loading %d images for training...\n", number_imgs);
    Img** imgs = csv_to_imgs("./data/mnist_test.csv", number_imgs);
    if (!imgs) {
        printf("Error: Failed to load images for training.\n");
        return 1;
    }

    printf("Creating neural network...\n");
    NeuralNetwork* net = network_create(784, 300, 10, 0.1);
    if (!net) {
        printf("Error: Failed to create neural network.\n");
        return 1;
    }

    printf("Starting training...\n");
    network_train_batch_imgs(net, imgs, number_imgs);
    printf("Training complete. Saving the network...\n");

    network_save(net, "testing_net"); // No need to capture return value, it's a void function

    imgs_free(imgs, number_imgs);
    network_free(net);

    // PREDICTING
    printf("\n--- Starting Prediction ---\n");
    int predict_number_imgs = 3000;
    printf("Loading %d images for prediction...\n", predict_number_imgs);
    Img** predict_imgs = csv_to_imgs("./data/mnist_test.csv", predict_number_imgs);
    if (!predict_imgs) {
        printf("Error: Failed to load images for prediction.\n");
        return 1;
    }

    printf("Loading saved network...\n");
    NeuralNetwork* predict_net = network_load("testing_net");
    if (!predict_net) {
        printf("Error: Failed to load neural network.\n");
        return 1;
    }

    printf("Starting prediction...\n");
    double score = network_predict_imgs(predict_net, predict_imgs, 1000);
    printf("Prediction complete. Score: %1.5f\n", score);

    imgs_free(predict_imgs, predict_number_imgs);
    network_free(predict_net);

    // End measuring time
    clock_gettime(CLOCK_MONOTONIC, &end_time);
    getrusage(RUSAGE_SELF, &usage_end);

    // Calculate time differences
    double real_time = (end_time.tv_sec - start_time.tv_sec) + 
                       (end_time.tv_nsec - start_time.tv_nsec) / 1e9;
    double user_time = (usage_end.ru_utime.tv_sec - usage_start.ru_utime.tv_sec) + 
                       (usage_end.ru_utime.tv_usec - usage_start.ru_utime.tv_usec) / 1e6;
    double sys_time = (usage_end.ru_stime.tv_sec - usage_start.ru_stime.tv_sec) + 
                      (usage_end.ru_stime.tv_usec - usage_start.ru_stime.tv_usec) / 1e6;

    // Print the execution times
    printf("\nreal    %0.3fs\n", real_time);
    printf("user    %0.3fs\n", user_time);
    printf("sys     %0.3fs\n", sys_time);

    return 0;
}
