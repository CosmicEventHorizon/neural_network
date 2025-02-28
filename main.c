#include <stdio.h>
#include <stdlib.h>
#include <time.h>
#include <math.h>

#define N 1 // number of inputs/ pixels
#define M 2 // number of layers excluding the input layer

// transpose a given matrix of dimensions RxC
// Paremeters: flattened pointer to input matrix, R, C, flattened pointer to output matrix
void transposeMatrix(double *, int, int, double *);

// multiply two matrices of dimensions RxK and KxC respectively
// Paremeters: flattened pointer to matrix 1, flattened pointer to matrix 2,
// R,  K,  C, flattened pointer to output matrix
// Multiplying a tranpose of a matrix with the matrix always yields a square matrix
void multiplyMatrix(double *, double *, int, int, int, double *);

// print a given matrix of dimensions RxC
// Paremeters: pointer to matrix, R, C
void printMatrix(double *, int, int);

// print the structure of the neural network
// the layers array has M pointers corresponding to the number of layers
// each element is a memory address pointing to a double array corresponding to all parameters
// of all the neurons in the layer. the no_neurons array is a parallel array containing the number
// of neurons in each layer
// Parameters: flattened pointer to pointer array layers, flattened pointer to no_neurons array
void printLayerNeurons(double **layers, int *no_neurons);

// randomly build a neural network model of M layers, with Ni neurons in each layer i
// Paremeters: flattened pointer to pointer array to store layers, pointer to no_neurons which will be
// a parallel array to the pointer array for layers and each i position will dictate the number of neurons
// in the corresponding layer
void buildModel(double **, int *);

// manually build a neural network model of M layers, with Ni neurons in each layer i
// Paremeters: flattened pointer to pointer array to store layers, pointer to no_neurons which will be
// a parallel array to the pointer array for layers and each i position will dictate the number of neurons
// in the corresponding layer
void testBuildModel(double **, int *);

// perform the sigmoid activation function on the output matrix of a layer
// Paremeters: flattened output matrix of a layer, size of layer output matrix, flattened output of function
void sigmoidActivation(double *, int, double *);

// general equations:
// memory address of element ij of flattened matrix M with C columns and memory p
// p = p + i * C + j
// multiplication: element a(ij) of matrix C = A*B, where A and B have K columns and rows respectively
// a(ij) = sigma(a(ik)*b(kj), k=K)

int main()
{
    // parameters to modify
    double X[N + 1][1] = {1, 0.33};    // input array
    int no_neurons[M][1] = {{1}, {1}}; // array containing the number of neurons in each layer
    int u = 2;                         // learning rate
    // algorithm parameters

    double *layers[M][1];
    srand(time(NULL));
    // buildModel((double **)layers, (int *)no_neurons);
    testBuildModel((double **)layers, (int *)no_neurons);

    // printLayerNeurons((double **)layers, (int *)no_neurons);

    // multiply X and L(h)
    double Zh[1][1];
    multiplyMatrix((double *)layers[0][0], (double *)X, 1, N + 1, 1, (double *)Zh);
    printMatrix((double *)Zh, 1, 1);
    double Yh[2][1];
    sigmoidActivation((double *)Zh, 1, (double *)Yh);
    Yh[0][0] = 1;
    printMatrix((double *)Yh, 2, 1);

    // multiply X and L(f)
    double Zf[1][1];
    multiplyMatrix((double *)layers[1][0], (double *)Yh, 1, 2, 1, (double *)Zf);
    printMatrix((double *)Zf, 1, 1);
    double Yf[2][1];
    sigmoidActivation((double *)Zf, 1, (double *)Yf);
    printMatrix((double *)Yf, 2, 1);

    return 0;
}

void buildModel(double **layers, int *no_neurons)
{
    double **layers_i;
    for (int i = 0; i < M; i++)
    {
        int no_parameters;
        if (i == 0) // if first layer then the number of parameters in layer = number of neurons * number of inputs
        {
            no_parameters = (*(no_neurons)) * N;
        }
        else
        { // number of parameters in layer = number of neurons * number of neurons in previous layer
            no_parameters = (*(no_neurons + i)) * (*(no_neurons + i - 1));
        }
        layers_i = layers + i;
        // each layer will be a size of number of neurons * number of parameters
        *layers_i = (double *)malloc(no_parameters * sizeof(double));
        // initialize weight parameters
        for (int j = 0; j < no_parameters; j++)
        {
            double *layers_j = *layers_i + j;
            *layers_j = rand() % 20;
        }
    }
}

void testBuildModel(double **layers, int *no_neurons)
{
    double **layer_1 = layers;
    double **layer_2 = layers + 1;
    *layer_1 = (double *)malloc(2 * sizeof(double));
    *layer_2 = (double *)malloc(2 * sizeof(double));

    double *layer_1_parameter_1 = *layer_1;
    double *layer_1_parameter_2 = *layer_1 + 1;
    *layer_1_parameter_1 = 1.86;
    *layer_1_parameter_2 = -3.3;

    double *layer_2_parameter_1 = *layer_2;
    double *layer_2_parameter_2 = *layer_2 + 1;
    *layer_2_parameter_1 = -1.5;
    *layer_2_parameter_2 = 4.4;
}

void transposeMatrix(double *matrix, int R, int C, double *matrix_transpose)
{
    for (int i = 0; i < R; i++)
    {
        for (int j = 0; j < C; j++)
        {
            double *matrix_ji = matrix_transpose + j * R + i;
            *matrix_ji = *(matrix + i * C + j);
        }
    }
}

void multiplyMatrix(double *matrix_1, double *matrix_2, int R, int K, int C, double *output_matrix)
{
    for (int i = 0; i < R; i++)
    {
        for (int j = 0; j < C; j++)
        {
            double sum = 0;
            for (int k = 0; k < K; k++)
            {
                sum += (*(matrix_1 + i * K + k)) * (*(matrix_2 + k * C + j));
            }
            double *output_matrix_ij = output_matrix + i * C + j;
            *output_matrix_ij = sum;
        }
    }
}

void printMatrix(double *matrix, int R, int C)
{
    for (int i = 0; i < R; i++)
    {
        for (int j = 0; j < C; j++)
        {
            printf("%7.4lf ", *(matrix + i * C + j));
        }
        printf("\n");
    }
    printf("\n");
}

void sigmoidActivation(double *Z, int size_z, double *Y)
{
    for (int i = 1; i < size_z+1; i++)
    {
        double *Y_i = Y + i;
        *Y_i = 1 / (1 + exp((*(Z + i-1))*-1));
    }
}

void printLayerNeurons(double **layers, int *no_neurons)
{
    for (int i = 0; i < M; i++)
    {
        printf("Layer %d:\n\n", i + 1);
        double *sub_vector;
        for (int j = 0; j < (*(no_neurons + i)); j++)
        {
            if (i == 0)
            {
                sub_vector = *(layers + i) + j * (N + 1);
                printf("Neuron %d:\n", j + 1);
                printMatrix((double *)sub_vector, 1, (N + 1));
            }
            else
            {
                sub_vector = *(layers + i) + j * ((*(no_neurons + i - 1)) + 1);
                printf("Neuron %d:\n", j + 1);
                printMatrix((double *)sub_vector, 1, *(no_neurons + i - 1) + 1);
            }
        }
    }
}
