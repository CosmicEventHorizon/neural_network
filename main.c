#include <stdio.h>
#include <stdlib.h>
#include <time.h>

#define N 2 // number of inputs/ pixels
#define M 4 // number of layers excluding the input layer

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

// given the matrix A and B in AX=B, apply the guassian algorithm with partial pivoting to A and B
// the dimensions are A: RxR, B: Rx1
// Paremeters: flattened pointer to A, R
void gaussianAlgorithm(double *, double *, int);

// given the matrix A and B in AX=B, apply partial pivoting to A and B
// the dimensions are A: RxR, B: Rx1
// Paremeters: flattened pointer to A, R
void pivotingAlgorithm(double *, double *, int);

// given the matrix equation AX=B, solve for x using backward substitution
// the dimensions are A: RxR, X: Rx1, B: Rx1
// Paremeters: flattened pointer to A, flattened pointer to B, R, flattened pointer to X
void backwardSubstitutionAlgorithm(double *, double *, int, double *);

// build a neural network model of M layers, with Ni neurons in each layer i
// Paremeters: flattened pointer to pointer array to store layers, pointer to no_neurons which will be
// a parallel array to the pointer array for layers and each i position will dictate the number of neurons
// in the corresponding layer
void buildModel(double **, int *);

// general equations:
// memory address of element ij of flattened matrix M with C columns and memory p
// p = p + i * C + j
// multiplication: element a(ij) of matrix C = A*B, where A and B have K columns and rows respectively
// a(ij) = sigma(a(ik)*b(kj), k=K)

int main()
{
    // parameters to modify
    double X[N + 1][1] = {1, 2, 3};              // input array
    int no_neurons[M][1] = {{2}, {3}, {2}, {4}}; // array containing the number of neurons in each layer
    int u = 2;                                   // learning rate

    // algorithm parameters
    double *layers[M][1];
    srand(time(NULL));
    buildModel((double **)layers, (int *)no_neurons);

    for (int i = 0; i < M; i++)
    {
        printf("Layer %d:\n\n", i+1);
        double *sub_vector;
        for (int j = 0; j < no_neurons[i][0]; j++)
        {
            if (i == 0)
            {
                sub_vector = layers[i][0] + j * no_neurons[i][0];
                printf("Neuron %d:\n", j+1);
                printMatrix((double *)sub_vector, 1, no_neurons[i][0]);
            }
            else
            {
                sub_vector = layers[i][0] + j * no_neurons[i-1][0];
                printf("Neuron %d:\n", j+1);
                printMatrix((double *)sub_vector, 1, no_neurons[i-1][0]);
            }
        }
    }

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
            printf("%7.2lf ", *(matrix + i * C + j));
        }
        printf("\n");
    }
    printf("\n");
}

void gaussianAlgorithm(double *A, double *B, int R)
{
    for (int i = 0; i < R; i++)
    {
        pivotingAlgorithm(A, B, R);
        for (int j = i + 1; j < R; j++)
        {
            double c = (*(A + j * R + i)) / (*(A + i * R + i));
            for (int k = 0; k < R; k++)
            {
                double *A_jk = A + j * R + k;
                *A_jk = *A_jk - c * (*(A + i * R + k));
            }
            double *B_j0 = B + j;
            *B_j0 = *B_j0 - c * (*(B + i));
        }
    }
}

void backwardSubstitutionAlgorithm(double *A, double *B, int R, double *X)
{
    int n = R - 1;
    double *X_n = X + n;
    *X_n = (*(B + n)) / (*(A + n * R + n));
    for (int i = n - 1; i >= 0; i--)
    {
        double sum = 0;
        for (int j = i + 1; j < R; j++)
        {
            sum += (*(A + i * R + j)) * (*(X + j));
        }
        double *X_i = X + i;
        *X_i = ((*(B + i)) - sum) / (*(A + i * R + i));
    }
}

void pivotingAlgorithm(double *A, double *B, int R)
{
    for (int i = 0; i < R; i++)
    {
        double max = abs(*(A + i * R + i));
        int max_j = i;
        for (int j = i + 1; j < R; j++)
        {
            if (max < abs(*(A + j * R + i)))
            {
                max = *(A + j * R + i);
                max_j = j;
            }
        }
        if (max_j > i)
        {
            for (int k = 0; k < R; k++)
            {
                double temp = *(A + i * R + k);
                double *A_ik = A + i * R + k;
                *A_ik = *(A + max_j * R + k);
                double *A_max_jk = A + max_j * R + k;
                *A_max_jk = temp;
            }
            double tempB = *(B + i);
            double *B_i0 = B + i;
            *B_i0 = *(B + max_j);
            double *B_max_j0 = B + max_j;
            *B_max_j0 = tempB;
        }
    }
}
