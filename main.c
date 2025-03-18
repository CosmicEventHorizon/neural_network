#include <stdio.h>
#include <stdlib.h>
#include <time.h>
#include <math.h>

#define N 2   // number of inputs/ pixels excluding the bias
#define M 2   // number of layers excluding the input layer
#define O 1   // number of outputs
#define U 0.5 // learning rate

// multiply two matrices of dimensions RxK and KxC respectively
// Paremeters: pointer to matrix 1, pointer to matrix 2,
// R,  K,  C,  pointer to output matrix
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
// Parameters: triple pointer layers, pointer to no_neurons array
void printModelArchitecture(double ***layers, int *no_neurons);

// print the structure of a layer
// Parameters: pointer to array pointer neurons, layer index, pointer to no_neurons array
void printLayer(double **, int, int *);

// randomly build a neural network model of M layers, with Ni neurons in each layer i
// Paremeters: flattened pointer to pointer array to store layers, pointer to no_neurons which will be
// a parallel array to the pointer array for layers and each i position will dictate the number of neurons
// in the corresponding layer
void buildModel(double **, int *);

// manually build a neural network model of M layers, with Ni neurons in each layer i
// dont include the bias as it will be written by other functions
// Paremeters: flattened pointer to pointer array to store layers, pointer to no_neurons which will be
// a parallel array to the pointer array for layers and each i position will dictate the number of neurons
// in the corresponding layer
void testBuildModel(double ***, int *);

// run the model
// Paremeters: pointer to pointer array X, triple pointer layers, pointer to no_neurons array
// pointer to pointer array z_outputs, pointer to pointer array y_outputs
void feedForward(double *, double ***, int *, double **, double **);

// multiple an input X contained in pointer array Y
// with the neurons in layer I and store the output in Z and activated output in next Y
// Paremeters: pointer to pointer array Y, triple pointer layers, I, pointer to no_neurons array, pointer to pointer array Z
void multiplyNeurons(double **, double ***, int, int *, double **);

// Paremeters: add a matrix X to a position I in matrix Y
// pointer to array X, size of array X, position I, pointer to pointer array Y
void addOutputToY(double *, int, int, double **);

// apply the backpropagation step
// Paremeters:  pointer to input array, pointer to the expected output, pointer to pointer array y_outputs,
//  pointer to pointer array z outputs,  pointer to pointer layers array,  pointer to no_neurons array
void backPropagate(double *, double *, double **, double **, double ***, int *);

// add bias to a vector
// Paremeters: pointer to input array, size of the input array, pointer to pointer to output array
void addBias(double *, int, double **);

// perform the sigmoid activation function on the output matrix of a layer
// Paremeters: flattened output matrix of a layer, size of layer output matrix, flattened pointer to pointer output of function
void sigmoidActivationMatrix(double *, int, double **);

// perform the sigmoid activation function on an x double
// Paremeters: double x
double sigmoidActivation(double x);

// perform the sigmoid activation function derivative on an x double
// Paremeters: double x
double sigmoidActivationDerivative(double x);

// general equations:
// memory address of element ij of flattened matrix M with C columns and memory p
// p = p + i * C + j
// multiplication: element a(ij) of matrix C = A*B, where A and B have K columns and rows respectively
// a(ij) = sigma(a(ik)*b(kj), k=K)
// number of parameters = number of neurons in previous layer * number of neurons in current layer + number of neurons in current layer

// architecture:
// layers: It has M rows and each row will contain the weights for that layer in order from B1,W11,W12,B2,W21,W22,B3
// z_outputs: the output of each layer without sigmoid activation
// y_outputs: the output of each later with sigmoid activation

int main()
{
    // parameters to modify
    double X[N] = {1, 1};       // input array for one test data excluding the bias, call testBuildX if its a simple 1xM double array
    double T[O] = {0};          // expected output array
    int no_neurons[M] = {2, O}; // array containing the number of neurons in each layer excluding the bias and input layer with O being the final layer

    // algorithm parameters
    double **layers[M];
    double *z_outputs[M];
    double *y_outputs[M + 1];

    // Run the model
    // srand(time(NULL));
    // buildModel(layers, no_neurons);
    testBuildModel(layers, no_neurons);
    printModelArchitecture(layers, no_neurons);
    feedForward(X, layers, no_neurons, z_outputs, y_outputs);
    backPropagate(X, T, y_outputs, z_outputs, layers, no_neurons);

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

void testBuildModel(double ***layers, int *no_neurons)
{
    // initialize number of neurons in each layer
    double ***layer_1 = layers;
    double ***layer_2 = layers + 1;
    *layer_1 = (double **)malloc(2 * sizeof(double *));
    *layer_2 = (double **)malloc(1 * sizeof(double *));

    // neuron naming : neuron_layer_index
    double **neurons_1 = *layer_1;
    *neurons_1 = (double *)malloc(3 * sizeof(double));
    double *neuron_1_1 = *neurons_1;
    double *neuron_1_1_parameter_1 = neuron_1_1;
    double *neuron_1_1_parameter_2 = neuron_1_1 + 1;
    double *neuron_1_1_parameter_3 = neuron_1_1 + 2;
    *neuron_1_1_parameter_1 = 0.6;
    *neuron_1_1_parameter_2 = 0.5;
    *neuron_1_1_parameter_3 = -0.3;

    *(neurons_1 + 1) = (double *)malloc(3 * sizeof(double));
    double *neuron_1_2 = *(neurons_1 + 1);
    double *neuron_1_2_parameter_1 = neuron_1_2;
    double *neuron_1_2_parameter_2 = neuron_1_2 + 1;
    double *neuron_1_2_parameter_3 = neuron_1_2 + 2;
    *neuron_1_2_parameter_1 = -0.4;
    *neuron_1_2_parameter_2 = 0.2;
    *neuron_1_2_parameter_3 = 0.5;

    double **neurons_2 = *layer_2;
    *neurons_2 = (double *)malloc(3 * sizeof(double));
    double *neuron_2_1 = *neurons_2;
    double *neuron_2_1_parameter_1 = neuron_2_1;
    double *neuron_2_1_parameter_2 = neuron_2_1 + 1;
    double *neuron_2_1_parameter_3 = neuron_2_1 + 2;
    *neuron_2_1_parameter_1 = 0.8;
    *neuron_2_1_parameter_2 = 0.1;
    *neuron_2_1_parameter_3 = 0.3;
}

void feedForward(double *X, double ***layers, int *no_neurons, double **z_outputs, double **y_outputs)
{
    // use the input layers
    double *X_bias;
    addBias(X, N, &X_bias);
    addOutputToY(X_bias, N + 1, 0, y_outputs);
    int no_neurons_0 = *no_neurons;
    *z_outputs = (double *)malloc(no_neurons_0 * sizeof(double));
    multiplyNeurons(y_outputs, layers, 0, no_neurons, z_outputs);
    // printMatrix(*z_outputs, 1, no_neurons_0);

    // use the next layers
    for (int i = 1; i < M; i++)
    {
        int no_neurons_i = *(no_neurons + i);
        double *X_i = *(y_outputs + i);
        multiplyNeurons(y_outputs, layers, i, no_neurons, z_outputs);
    }
    // printMatrix(*(y_outputs + M), O, 1);
}

void multiplyNeurons(double **y_outputs, double ***layers, int k, int *no_neurons, double **z_outputs)
{
    int no_neurons_k = *(no_neurons + k);
    int no_neurons_km1;
    if (k == 0)
    {
        no_neurons_km1 = N;
    }
    else
    {
        no_neurons_km1 = *(no_neurons + k - 1);
    }
    double **neurons_k = *(layers + k);
    *(z_outputs + k) = (double *)malloc(no_neurons_k * sizeof(double));
    double *output = *(z_outputs + k);
    for (int i = 0; i < no_neurons_k; i++)
    {
        double *X = *(y_outputs + k);
        // printMatrix(X, 1, no_neurons_km1 + 1);
        double *Y = *(neurons_k + i);
        // printMatrix(Y, 1, no_neurons_km1 + 1);
        double result[1];
        multiplyMatrix(X, Y, 1, no_neurons_km1 + 1, 1, result);
        // printMatrix(result, 1, 1);
        *(output + i) = *result;
    }
    // printMatrix(output,1,no_neurons_k);
    double *output_sigmoid;
    sigmoidActivationMatrix(output, no_neurons_k, &output_sigmoid);
    // printMatrix(output_sigmoid, no_neurons_k, 1);
    double *output_sigmoid_bias;
    // do not add bias to the final output
    if (k < M - 1)
    {
        addBias(output_sigmoid, no_neurons_k, &output_sigmoid_bias);
        addOutputToY(output_sigmoid_bias, no_neurons_k + 1, k + 1, y_outputs);
    }
    else
    {
        addOutputToY(output_sigmoid, no_neurons_k, k + 1, y_outputs);
    }
    // printMatrix(output_sigmoid_bias, no_neurons_k + 1, 1);
    // printMatrix(*(y_outputs + k + 1), no_neurons_k+1, 1);
}

void addOutputToY(double *X, int size, int position, double **y_outputs)
{
    *(y_outputs + position) = (double *)malloc(size * sizeof(double));
    for (int i = 0; i < size; i++)
    {
        *(*(y_outputs + position) + i) = *(X + i);
    }
}

void backPropagate(double *X, double *t_outputs, double **y_outputs, double **z_outputs, double ***layers, int *no_neurons)
{
    // compute the final layer descent
    int no_neurons_M = *(no_neurons + M - 1);
    int no_neurons_Mm1 = *(no_neurons + M - 2);
    double ***layers_M = layers + M - 1;
    double **neurons_M = *layers_M;
    double **neurons_M_copy = (double **)malloc(O * sizeof(double *));
    double *z_outputs_M = *(z_outputs + M - 1);
    double *y_outputs_M = *(y_outputs + M);
    double *y_outputs_Mm1 = *(y_outputs + M - 1);

    for (int i = 0; i < no_neurons_M; i++)
    {
        double *neuron_M_i = *(neurons_M + i);
        *(neurons_M_copy + i) = (double *)malloc((no_neurons_Mm1 + 1) * sizeof(double));
        double *neuron_M_copy_i = *(neurons_M_copy + i);
        for (int j = 0; j < no_neurons_Mm1 + 1; j++)
        {
            double dE_daij = 0;
            for (int k = 0; k < O; k++)
            {
                double t_k = *(t_outputs + k);
                double y_k = *(y_outputs_M + k);
                double tk_yk = t_k - y_k;
                //printf("tk_yk is %f\n", tk_yk);
                double z_k = *(z_outputs_M + k);
                double z_k_derivative = sigmoidActivationDerivative(z_k);
                //printf("z_k is %f and g'(z_k) is %f\n", z_k, z_k_derivative);
                double y_j = *(y_outputs_Mm1 + j);
                //printf("y_j is %f\n", y_j);
                dE_daij += y_j * z_k_derivative * tk_yk;
            }
            dE_daij *= -1;
            double a_ij = *(neuron_M_i + j);
            //printf("dE_daij for aij = %f is %f\n", a_ij, dE_daij);
            *(neuron_M_copy_i + j) = a_ij;
            *(neuron_M_i + j) = *(neuron_M_i + j) - U * dE_daij;
        }
    }
    printf("The final layer weights are now:\n");
    printLayer(neurons_M, M - 1, no_neurons);

    // compute the hidden layers descent
    // loop through each layer
    for (int h = 1; h < M; h++)
    {
        int no_neurons_h = *(no_neurons + h - 1);
        int no_neurons_hm1;
        if (h == 1)
        {
            no_neurons_hm1 = N + 1;
        }
        else
        {
            no_neurons_hm1 = *(no_neurons + h - 2);
        }
        double ***layers_h = layers + h - 1;
        double **neurons_h = *layers_h;
        double *z_outputs_h = *(z_outputs + h - 1);
        double *y_outputs_h = *(y_outputs + h);
        double *y_outputs_hm1 = *(y_outputs + h - 1);
        for (int j = 0; j < no_neurons_h; j++)
        {
            double *neuron_h_j = *(neurons_h + j);

            for (int k = 0; k < no_neurons_hm1; k++)
            {

                double dE_dajk = 0;
                for (int i = 0; i < O; i++)
                {
                    //printf("j is %d, k is %d, i is %d\n", j, k, i);
                    double t_i = *(t_outputs + i);
                    double y_i = *(y_outputs_M + i);
                    double ti_yi = t_i - y_i;
                    //printf("ti_yi is %f\n", ti_yi);
                    double z_i = *(z_outputs_M + i);
                    double z_i_derivative = sigmoidActivationDerivative(z_i);
                    //printf("z_i is %f and g'(z_i) is %f\n", z_i, z_i_derivative);
                    double *neurons_M_copy_i = *(neurons_M_copy + i);
                    double a_ij = *(neurons_M_copy_i + j + 1);
                    //printf("a_ij is %f\n", a_ij);
                    double z_j = *(z_outputs_h + j);
                    double z_j_derivative = sigmoidActivationDerivative(z_j);
                    //printf("z_j is %f and g'(z_j) is %f\n", z_j, z_j_derivative);
                    double y_k = *(y_outputs_hm1 + k);
                    //printf("y_k is %f\n", y_k);
                    dE_dajk += ti_yi * z_i_derivative * a_ij * z_j_derivative * y_k;
                }
                dE_dajk *= -1;
                double a_jk = *(neuron_h_j + k);
                //printf("dE_dajk for ajk = %f is %f\n", a_jk, dE_dajk);
                *(neuron_h_j + k) = *(neuron_h_j + k) - U * dE_dajk;
            }
        }
        printf("Layer %d weights are now:\n", h-1);
        printLayer(neurons_h, h - 1, no_neurons);
    }
}

void addBias(double *vector, int size, double **output)
{
    *output = (double *)malloc((size + 1) * sizeof(double));
    **output = 1;
    for (int i = 1; i < size + 1; i++)
    {
        double *output_i = *output + i;
        double *vector_i = vector + (i - 1);
        *output_i = *vector_i;
    }
}

double sigmoidActivation(double x)
{
    return 1 / (1 + exp(x * -1));
}

double sigmoidActivationDerivative(double x)
{
    return sigmoidActivation(x) * (1 - sigmoidActivation(x));
}

void sigmoidActivationMatrix(double *Z, int size_z, double **Y)
{
    *Y = malloc(size_z * sizeof(double));
    for (int i = 0; i < size_z; i++)
    {
        double *Y_i = *Y + i;
        *Y_i = sigmoidActivation((*(Z + i)));
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
    printf("\n\n");
}

void printLayer(double **neurons, int i, int *no_neurons)
{
    if (i == 0)
    {
        for (int j = 0; j < N; j++)
        {
            printf("\tNeuron %d:\n", j + 1);
            double *neuron_i_j = *(neurons + j);
            printMatrix(neuron_i_j, N + 1, 1);
        }
    }
    else
    {
        int no_neurons_i = *(no_neurons + i);
        int no_neurons_im1 = *(no_neurons + i - 1);
        for (int j = 0; j < no_neurons_i; j++)
        {
            printf("\tNeuron %d:\n", j + 1);
            double *neuron_i_j = *(neurons + j);
            printMatrix(neuron_i_j, no_neurons_im1 + 1, 1);
        }
    }
}

void printModelArchitecture(double ***layers, int *no_neurons)
{
    for (int i = 0; i < M; i++)
    {
        printf("Layer %d:\n", i);
        double ***layers_i = layers + i;
        double **neurons_i = *layers_i;
        int no_neurons_i = *(no_neurons + i);
        if (i == 0)
        {
            for (int j = 0; j < no_neurons_i; j++)
            {
                printf("\tNeuron %d:\n", j + 1);
                double *neuron_i_j = *(neurons_i + j);
                printMatrix(neuron_i_j, N + 1, 1);
            }
        }
        else
        {
            int no_neurons_im1 = *(no_neurons + i - 1);
            for (int j = 0; j < no_neurons_i; j++)
            {
                printf("\tNeuron %d:\n", j + 1);
                double *neuron_i_j = *(neurons_i + j);
                printMatrix(neuron_i_j, no_neurons_im1 + 1, 1);
            }
        }
    }
}
