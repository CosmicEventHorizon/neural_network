#include <stdio.h>
#include <stdlib.h>
#include <time.h>
#include <math.h>

#define D 15      // number of test data
#define N 3       // number of inputs/ pixels excluding the bias
#define M 3       // number of layers excluding the input layer
#define O 1       // number of outputs
#define U 0.001   // learning rate
#define EPOCH 500 // epoch

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

// from a raw X input add the elemnts to X_data to be later converted to X
// Paremeters: pointer to raw X data, pointer to pointer array X_data
void buildXTFromRaw(double *X_raw, double *T_raw, double **X_data, double **T_data);

// randomly build a neural network model of M layers, with no_neurons neurons in each layer
// by default the weights are randomized and the bias are set to 0
// Paremeters: triple pointer layers, pointer to no_neurons array
void buildModel(double ***, int *);

// manually build a neural network model of M layers, with Ni neurons in each layer i
// dont include the bias as it will be written by other functions
// Paremeters:  pointer to manual weights, pointer to pointer array to store layers, pointer to no_neurons which will be
// a parallel array to the pointer array for layers and each i position will dictate the number of neurons
// in the corresponding layer
void testBuildModel(double *, double ***, int *);

// run the model
// Paremeters: pointer to pointer array X, triple pointer layers, pointer to no_neurons array
// pointer to pointer array z_outputs, pointer to pointer array y_outputs
void feedForward(double *, double ***, int *, char *, double **, double **);

// multiple an input X contained in pointer array Y
// with the neurons in layer I and store the output in Z and activated output in next Y
// Paremeters: pointer to pointer array Y, triple pointer layers, I, pointer to no_neurons array, pointer to pointer array Z
void multiplyNeurons(double **, double ***, int, int *, char *, double **);

// Paremeters: add a matrix X to a position I in matrix Y
// pointer to array X, size of array X, position I, pointer to pointer array Y
void addOutputToY(double *, int, int, double **);

// apply the backpropagation step
// Paremeters:  pointer to input array, pointer to the expected output, pointer to pointer array y_outputs,
//  pointer to pointer array z outputs,  pointer to pointer layers array,  pointer to no_neurons array
void backPropagate(double *, double *, double **, double **, double ***, int *, char *);

// add bias to a vector
// Paremeters: pointer to input array, size of the input array, pointer to pointer to output array
void addBias(double *, int, double **);

// perform the sigmoid activation function on the output matrix of a layer
// Paremeters:  output matrix of a layer, size of layer output matrix,  pointer to pointer output of function
void sigmoidActivationMatrix(double *, int, double **);

// perform the sigmoid activation function on an x double
// Paremeters: double x
double sigmoidActivation(double x);

// perform the sigmoid activation function derivative on an x double
// Paremeters: double x
double sigmoidActivationDerivative(double x);

// perform the tanh activation function on the output matrix of a layer
// Paremeters:  output matrix of a layer, size of layer output matrix,  pointer to pointer output of function
void tanhActivationMatrix(double *, int, double **);

// perform the tanh activation function on an x double
// Paremeters: double x
double tanhActivation(double x);

// perform the tanh activation function derivative on an x double
// Paremeters: double x
double tanhActivationDerivative(double x);

// perform the ReLU activation function on the output matrix of a layer
// Paremeters:  output matrix of a layer, size of layer output matrix,  pointer to pointer output of function
void reLUActivationMatrix(double *, int, double **);

// perform the ReLU activation function on an x double
// Paremeters: double x
double reLUActivation(double x);

// perform the ReLU activation function derivative on an x double
// Paremeters: double x
double reLUActivationDerivative(double x);

// general equations:
// memory address of element ij of  matrix M with C columns and memory p
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
    double X_raw[N * D] = {
        3, 1500, 20,
        2, 800, 30,
        4, 2000, 15,
        3, 1700, 10,
        1, 500, 40,
        5, 2500, 5,
        3, 1600, 25,
        2, 1000, 35,
        4, 2100, 10,
        3, 1550, 22,
        2, 900, 28,
        3, 1800, 12,
        4, 2300, 8,
        3, 1400, 20,
        1, 600, 50}; // enter raw data from x0(0) to xN(D), size is N*D
    double T_raw[O * D] = {
        300,
        200,
        450,
        400,
        150,
        500,
        350,
        220,
        480,
        320,
        210,
        370,
        490,
        310,
        180};                                      // enter raw expected data from t0(0) to tO(D), size is O*D
    int no_neurons[M] = {10, 8, O};                // array containing the number of neurons in each layer excluding the bias and input layer with O being the final layer
    char activation_function[M] = {'s', 's', 'r'}; // array containing which activation function to use for each layer
    // double manual_weights[4] = {0.2, 0.5, -0.5};

    // algorithm parameters
    double **layers[M];

    // Run the model
    // testBuildModel(manual_weights, layers, no_neurons);
    srand(time(NULL));
    buildModel(layers, no_neurons);
    double *X_data[D]; // input array for all test data, needs buildX() to construct X
    double *T_data[D]; // output array for all test data, needs buildXTFromRaw() to construct T
    buildXTFromRaw(X_raw, T_raw, X_data, T_data);
    // printf("\t\tThe model architecture before\n");
    // printf("\t\t------------------------------\n\n");
    // printModelArchitecture(layers, no_neurons);
    for (int e = 0; e < EPOCH; e++)
    {
        // printf("Actual Price\t\tPredicted Price\n");
        double MSE = 0;
        //     loop through all test data
        for (int i = 0; i < D; i++)
        {

            double **z_outputs = (double **)malloc(M * sizeof(double *));
            double **y_outputs = (double **)malloc((M + 1) * sizeof(double *));
            double *X = X_data[i];
            double *T = T_data[i];
            feedForward(X, layers, no_neurons, activation_function, z_outputs, y_outputs);
            //printf("%12.3f\t\t%12.3f\n", *T, *(*(y_outputs + M)));
            MSE += (*(*(y_outputs + M)) - (*T)) * (*(*(y_outputs + M)) - (*T));
            backPropagate(X, T, y_outputs, z_outputs, layers, no_neurons, activation_function);
        }
        double RMSE = sqrt(MSE / (D));
        printf("RMSE is %f\n", RMSE);
    }
    // printf("\t\tThe model architecture after\n");
    // printf("\t\t------------------------------\n\n");
    // printModelArchitecture(layers, no_neurons);

    return 0;
}

void buildXTFromRaw(double *X_raw, double *T_raw, double **X_data, double **T_data)
{
    // general formula
    // for a test data i with j parameters (i=[0,Inf) and j=[0,Inf))
    // *(X_data+i) = (double*) malloc(j*sizeof(double))
    // *(*(X_data+i)+j) = xj

    for (int i = 0; i < D; i++)
    {
        *(X_data + i) = (double *)malloc(N * sizeof(double));
        *(T_data + i) = (double *)malloc(O * sizeof(double));
        for (int j = 0; j < N; j++)
        {
            double data_ij = *(X_raw + i * N + j);
            *(*(X_data + i) + j) = data_ij;
        }
        for (int j = 0; j < O; j++)
        {
            double data_ij = *(T_raw + i * O + j);
            *(*(T_data + i) + j) = data_ij;
        }
    }
}

void buildModel(double ***layers, int *no_neurons)
{
    // initialize number of neurons in each layer
    for (int i = 0; i < M; i++)
    {
        double ***layer_i = layers + i;
        int no_neurons_i = *(no_neurons + i);
        int no_neurons_im1;
        if (i == 0)
        {
            no_neurons_im1 = N + 1;
        }
        else
        {
            no_neurons_im1 = *(no_neurons + i - 1);
        }
        *layer_i = (double **)malloc(no_neurons_i * sizeof(double *));
        double **neurons_i = *layer_i;
        for (int j = 0; j < no_neurons_i; j++)
        {
            *(neurons_i + j) = (double *)malloc(no_neurons_im1 * sizeof(double));
            // neuron naming : neuron_layer_index
            double *neuron_i_j = *(neurons_i + j);
            double *neuron_i_j_parameter_0 = neuron_i_j;
            *neuron_i_j_parameter_0 = 0; // biases are set to 0
            for (int k = 1; k < no_neurons_im1 + 1; k++)
            {
                double *neuron_i_j_parameter_k = neuron_i_j + k;
                double range = (0.05);
                double div = RAND_MAX / range;
                double random_double = 0 + (rand() / div);
                *neuron_i_j_parameter_k = random_double;
            }
        }
    }
}

void testBuildModel(double *manual_weights, double ***layers, int *no_neurons)
{
    // initialize number of neurons in each layer
    for (int h = 0; h < M; h++)
    {
        double ***layer_h = layers + h;
        int no_neurons_h = *(no_neurons + h);
        int no_neurons_hm1;
        int no_neurons_hm2;
        if (h == 0)
        {
            no_neurons_hm1 = N + 1;
            no_neurons_hm2 = 0;
        }
        else
        {
            no_neurons_hm1 = *(no_neurons + h - 1);
        }
        *layer_h = (double **)malloc(no_neurons_h * sizeof(double *));
        double **neurons_h = *layer_h;
        // loop through each neuron
        for (int i = 0; i < no_neurons_h; i++)
        {
            *(neurons_h + i) = (double *)malloc(no_neurons_hm1 * sizeof(double));
            // neuron naming : neuron_layer_index
            double *neuron_h_i = *(neurons_h + i);
            for (int j = 0; j < no_neurons_hm1; j++)
            {
                double *neuron_h_i_parameter_j = neuron_h_i + j;
                double manual_weights_h_i_j = *(manual_weights + j + i * no_neurons_hm1 + h * no_neurons_hm1 * no_neurons_hm2);
                *neuron_h_i_parameter_j = manual_weights_h_i_j;
            }
            no_neurons_hm2 = no_neurons_hm1;
        }
    }
}

void feedForward(double *X, double ***layers, int *no_neurons,
                 char *activation_function, double **z_outputs, double **y_outputs)
{
    // use the input layers
    double *X_bias;
    // printMatrix(X, 1, N);
    addBias(X, N, &X_bias);
    addOutputToY(X_bias, N + 1, 0, y_outputs);
    //     printMatrix(*y_outputs, 1, N + 1);
    int no_neurons_0 = *no_neurons;
    *z_outputs = (double *)malloc(no_neurons_0 * sizeof(double));
    multiplyNeurons(y_outputs, layers, 0, no_neurons, activation_function, z_outputs);
    // printMatrix(*z_outputs, 1, no_neurons_0);

    // use the next layers
    for (int i = 1; i < M; i++)
    {
        int no_neurons_i = *(no_neurons + i);
        double *X_i = *(y_outputs + i);
        multiplyNeurons(y_outputs, layers, i, no_neurons, activation_function, z_outputs);
    }
    // printMatrix(*(y_outputs + M), 2, 1);
}

void multiplyNeurons(double **y_outputs, double ***layers, int k, int *no_neurons,
                     char *activation_function, double **z_outputs)
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
    double *X = *(y_outputs + k);
    char activation_k = *(activation_function + k);
    // printf("k is %d\n", k);
    // printMatrix(X, 1, no_neurons_km1 + 1);
    for (int i = 0; i < no_neurons_k; i++)
    {
        double *Y = *(neurons_k + i);
        // printf("k is %d and i is %d\n", k, i);
        // printMatrix(Y, 1, no_neurons_km1 + 1);
        double result[1];
        multiplyMatrix(X, Y, 1, no_neurons_km1 + 1, 1, result);
        // printMatrix(result, 1, 1);
        *(output + i) = *result;
    }
    // printMatrix(output, 1, no_neurons_k);
    double *output_activated;
    if (activation_k == 's')
    {
        sigmoidActivationMatrix(output, no_neurons_k, &output_activated);
    }
    else if (activation_k == 'r')
    {
        reLUActivationMatrix(output, no_neurons_k, &output_activated);
    }
    else if (activation_k == 't')
    {
        tanhActivationMatrix(output, no_neurons_k, &output_activated);
    }
    // printf("k is %d \n", k);
    // printMatrix(output_activated, no_neurons_k, 1);
    double *output_activated_bias;
    // do not add bias to the final output
    if (k < M - 1)
    {
        addBias(output_activated, no_neurons_k, &output_activated_bias);
        addOutputToY(output_activated_bias, no_neurons_k + 1, k + 1, y_outputs);
    }
    else
    {
        addOutputToY(output_activated, no_neurons_k, k + 1, y_outputs);
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

void backPropagate(double *X, double *t_outputs, double **y_outputs, double **z_outputs,
                   double ***layers, int *no_neurons, char *activation_function)
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
    char activation_M = *(activation_function + M - 1);

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
                // printf("t_k is %f and y_kl is %f and  tk_yk is %f\n", t_k, y_k, tk_yk);
                double z_k = *(z_outputs_M + k);
                double z_k_derivative;
                if (activation_M == 's')
                {
                    z_k_derivative = sigmoidActivationDerivative(z_k);
                }
                else if (activation_M == 'r')
                {
                    z_k_derivative = reLUActivationDerivative(z_k);
                }
                else if (activation_M == 't')
                {
                    z_k_derivative = tanhActivationDerivative(z_k);
                }
                // printf("z_k is %f and g'(z_k) is %f\n", z_k, z_k_derivative);
                double y_j = *(y_outputs_Mm1 + j);
                // printf("y_j is %f\n", y_j);
                dE_daij += y_j * z_k_derivative * tk_yk;
            }
            dE_daij *= -1;
            double a_ij = *(neuron_M_i + j);
            // printf("dE_daij for aij = %f is %f\n", a_ij, dE_daij);
            *(neuron_M_copy_i + j) = a_ij;
            *(neuron_M_i + j) = *(neuron_M_i + j) - U * dE_daij;
        }
    }
    // printf("The final layer weights are now:\n");
    // printLayer(neurons_M, M - 1, no_neurons);

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
        char activation_h = *(activation_function + h - 1);
        for (int j = 0; j < no_neurons_h; j++)
        {
            double *neuron_h_j = *(neurons_h + j);

            for (int k = 0; k < no_neurons_hm1 + 1; k++)
            {

                double dE_dajk = 0;
                for (int i = 0; i < O; i++)
                {
                    // printf("j is %d, k is %d, i is %d\n", j, k, i);
                    double t_i = *(t_outputs + i);
                    double y_i = *(y_outputs_M + i);
                    double ti_yi = t_i - y_i;
                    // printf("ti_yi is %f\n", ti_yi);
                    double z_i = *(z_outputs_M + i);
                    double z_i_derivative;
                    if (activation_M == 's')
                    {

                        z_i_derivative = sigmoidActivationDerivative(z_i);
                    }
                    else if (activation_M == 'r')
                    {

                        z_i_derivative = reLUActivationDerivative(z_i);
                    }
                    else if (activation_M == 't')
                    {

                        z_i_derivative = tanhActivationDerivative(z_i);
                    }
                    // printf("z_i is %f and g'(z_i) is %f\n", z_i, z_i_derivative);
                    double *neurons_M_copy_i = *(neurons_M_copy + i);
                    double a_ij = *(neurons_M_copy_i + j + 1);
                    // printf("a_ij is %f\n", a_ij);
                    double z_j = *(z_outputs_h + j);
                    double z_j_derivative;
                    if (activation_h == 's')
                    {

                        z_j_derivative = sigmoidActivationDerivative(z_j);
                    }
                    else if (activation_h == 'r')
                    {

                        z_j_derivative = reLUActivationDerivative(z_j);
                    }
                    else if (activation_h == 't')
                    {

                        z_j_derivative = tanhActivationDerivative(z_j);
                    }
                    // printf("z_j is %f and g'(z_j) is %f\n", z_j, z_j_derivative);
                    double y_k = *(y_outputs_hm1 + k);
                    // printf("y_k is %f\n", y_k);
                    dE_dajk += ti_yi * z_i_derivative * a_ij * z_j_derivative * y_k;
                }
                dE_dajk *= -1;
                double a_jk = *(neuron_h_j + k);
                // printf("dE_dajk for ajk = %f is %f\n", a_jk, dE_dajk);
                *(neuron_h_j + k) = *(neuron_h_j + k) - U * dE_dajk;
            }
        }
        // printf("Layer %d weights are now:\n", h - 1);
        // printLayer(neurons_h, h - 1, no_neurons);
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

double tanhActivation(double x)
{
    return (exp(x) - exp(-x)) / (exp(x) + exp(-x));
}

double tanhActivationDerivative(double x)
{
    return 1 - tanhActivation(x) * tanhActivation(x);
}

void tanhActivationMatrix(double *Z, int size_z, double **Y)
{
    *Y = malloc(size_z * sizeof(double));
    for (int i = 0; i < size_z; i++)
    {
        double *Y_i = *Y + i;
        *Y_i = tanhActivation((*(Z + i)));
    }
}

double reLUActivation(double x)
{
    return x <= 0 ? 0 : x;
}

double reLUActivationDerivative(double x)
{
    return x <= 0 ? 0 : 1;
}

void reLUActivationMatrix(double *Z, int size_z, double **Y)
{
    *Y = malloc(size_z * sizeof(double));
    for (int i = 0; i < size_z; i++)
    {
        double *Y_i = *Y + i;
        *Y_i = reLUActivation((*(Z + i)));
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
