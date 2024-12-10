#include "nn.h"
#include <stdlib.h>
#include <stdio.h>
#include <string.h>
#include "../matrix/ops.h"
#include "../neural/activations.h"

NeuralNetwork* network_create(int input, int hidden, int output, double lr) {
    NeuralNetwork* net = malloc(sizeof(NeuralNetwork));
    net->input = input;
    net->hidden = hidden;
    net->output = output;
    net->learning_rate = lr;
    net->hidden_weights = matrix_create(hidden, input);
    net->output_weights = matrix_create(output, hidden);
    matrix_randomize(net->hidden_weights, hidden);
    matrix_randomize(net->output_weights, output);
    return net;
}

void network_train(NeuralNetwork* net, Matrix* input, Matrix* output) {
    Matrix* hidden_inputs = dot_runner(net->hidden_weights, input);
    Matrix* hidden_outputs = apply_runner(sigmoid, hidden_inputs);
    Matrix* final_inputs = dot_runner(net->output_weights, hidden_outputs);
    Matrix* final_outputs = apply_runner(sigmoid, final_inputs);

    Matrix* output_errors = subtract_runner(output, final_outputs);
    Matrix* transposed_mat = transpose_runner(net->output_weights);
    Matrix* hidden_errors = dot_runner(transposed_mat, output_errors);
    matrix_free(transposed_mat);

    // Backpropagation
    Matrix* sigmoid_primed_mat = apply_runner(sigmoidPrime, final_outputs);
    Matrix* multiplied_mat = multiply_runner(output_errors, sigmoid_primed_mat);
    transposed_mat = transpose_runner(hidden_outputs);
    Matrix* dot_mat = dot_runner(multiplied_mat, transposed_mat);
    Matrix* scaled_mat = scale_runner(net->learning_rate, dot_mat);
    Matrix* added_mat = add_runner(net->output_weights, scaled_mat);

    matrix_free(net->output_weights);
    net->output_weights = added_mat;

    matrix_free(sigmoid_primed_mat);
    matrix_free(multiplied_mat);
    matrix_free(transposed_mat);
    matrix_free(dot_mat);
    matrix_free(scaled_mat);

    // Update hidden weights
    sigmoid_primed_mat = apply_runner(sigmoidPrime, hidden_outputs);
    multiplied_mat = multiply_runner(hidden_errors, sigmoid_primed_mat);
    transposed_mat = transpose_runner(input);
    dot_mat = dot_runner(multiplied_mat, transposed_mat);
    scaled_mat = scale_runner(net->learning_rate, dot_mat);
    added_mat = add_runner(net->hidden_weights, scaled_mat);

    matrix_free(net->hidden_weights);
    net->hidden_weights = added_mat; 

    matrix_free(sigmoid_primed_mat);
    matrix_free(multiplied_mat);
    matrix_free(transposed_mat);
    matrix_free(dot_mat);
    matrix_free(scaled_mat);

    matrix_free(hidden_inputs);
    matrix_free(hidden_outputs);
    matrix_free(final_inputs);
    matrix_free(final_outputs);
    matrix_free(output_errors);
    matrix_free(hidden_errors);
}
