#include <iostream>
#include <vector>
#include <cmath>
#include <cstdlib>
#include <ctime>

using namespace std;

// Función de activación sigmoide
double sigmoid(double x) {
    return 1.0 / (1.0 + exp(-x));
}

// Derivada de la función de activación sigmoide
double sigmoid_derivative(double x) {
    return x * (1.0 - x);
}

// Clase para la red neuronal
class NeuralNetwork {
public:
    NeuralNetwork(int inputSize, int hiddenSize, int outputSize);
    void train(const vector<vector<double>>& inputs, const vector<vector<double>>& outputs, int epochs, double learningRate);
    vector<double> predict(const vector<double>& input);
    
private:
    int inputSize;
    int hiddenSize;
    int outputSize;
    
    vector<vector<double>> weightsInputHidden;
    vector<double> biasHidden;
    vector<vector<double>> weightsHiddenOutput;
    vector<double> biasOutput;

    vector<double> feedForward(const vector<double>& input);
    void backpropagate(const vector<double>& input, const vector<double>& target, double learningRate);
};

NeuralNetwork::NeuralNetwork(int inputSize, int hiddenSize, int outputSize)
    : inputSize(inputSize), hiddenSize(hiddenSize), outputSize(outputSize) {
    srand(static_cast<unsigned int>(time(nullptr)));
    
    // Inicializar pesos y sesgos
    weightsInputHidden.resize(inputSize, vector<double>(hiddenSize));
    weightsHiddenOutput.resize(hiddenSize, vector<double>(outputSize));
    biasHidden.resize(hiddenSize);
    biasOutput.resize(outputSize);

    for (int i = 0; i < inputSize; ++i)
        for (int j = 0; j < hiddenSize; ++j)
            weightsInputHidden[i][j] = (rand() % 1000) / 1000.0 - 0.5;
    
    for (int i = 0; i < hiddenSize; ++i)
        for (int j = 0; j < outputSize; ++j)
            weightsHiddenOutput[i][j] = (rand() % 1000) / 1000.0 - 0.5;
    
    for (int i = 0; i < hiddenSize; ++i)
        biasHidden[i] = (rand() % 1000) / 1000.0 - 0.5;
    
    for (int i = 0; i < outputSize; ++i)
        biasOutput[i] = (rand() % 1000) / 1000.0 - 0.5;
}

vector<double> NeuralNetwork::feedForward(const vector<double>& input) {
    // Cálculo de la capa oculta
    vector<double> hidden(hiddenSize);
    for (int j = 0; j < hiddenSize; ++j) {
        hidden[j] = biasHidden[j];
        for (int i = 0; i < inputSize; ++i)
            hidden[j] += input[i] * weightsInputHidden[i][j];
        hidden[j] = sigmoid(hidden[j]);
    }

    // Cálculo de la capa de salida
    vector<double> output(outputSize);
    for (int j = 0; j < outputSize; ++j) {
        output[j] = biasOutput[j];
        for (int i = 0; i < hiddenSize; ++i)
            output[j] += hidden[i] * weightsHiddenOutput[i][j];
        output[j] = sigmoid(output[j]);
    }

    return output;
}

void NeuralNetwork::backpropagate(const vector<double>& input, const vector<double>& target, double learningRate) {
    // Propagación hacia adelante
    vector<double> hidden(hiddenSize);
    vector<double> output = feedForward(input);

    for (int j = 0; j < hiddenSize; ++j) {
        hidden[j] = biasHidden[j];
        for (int i = 0; i < inputSize; ++i)
            hidden[j] += input[i] * weightsInputHidden[i][j];
        hidden[j] = sigmoid(hidden[j]);
    }

    // Cálculo de errores
    vector<double> outputError(outputSize);
    vector<double> hiddenError(hiddenSize);
    for (int j = 0; j < outputSize; ++j) {
        double error = target[j] - output[j];
        outputError[j] = error * sigmoid_derivative(output[j]);
    }

    // Ajuste de pesos y sesgos para la capa de salida
    for (int j = 0; j < outputSize; ++j) {
        biasOutput[j] += learningRate * outputError[j];
        for (int i = 0; i < hiddenSize; ++i)
            weightsHiddenOutput[i][j] += learningRate * outputError[j] * hidden[i];
    }

    // Cálculo del error para la capa oculta
    for (int j = 0; j < hiddenSize; ++j) {
        double error = 0.0;
        for (int k = 0; k < outputSize; ++k)
            error += outputError[k] * weightsHiddenOutput[j][k];
        hiddenError[j] = error * sigmoid_derivative(hidden[j]);
    }

    // Ajuste de pesos y sesgos para la capa oculta
    for (int j = 0; j < hiddenSize; ++j) {
        biasHidden[j] += learningRate * hiddenError[j];
        for (int i = 0; i < inputSize; ++i)
            weightsInputHidden[i][j] += learningRate * hiddenError[j] * input[i];
    }
}

void NeuralNetwork::train(const vector<vector<double>>& inputs, const vector<vector<double>>& outputs, int epochs, double learningRate) {
    for (int epoch = 0; epoch < epochs; ++epoch) {
        for (size_t i = 0; i < inputs.size(); ++i) {
            backpropagate(inputs[i], outputs[i], learningRate);
        }
    }
}

vector<double> NeuralNetwork::predict(const vector<double>& input) {
    return feedForward(input);
}

int main() {
    // Datos de entrenamiento
    vector<vector<double>> inputs = {{0, 0}, {0, 1}, {1, 0}, {1, 1}};
    vector<vector<double>> outputs = {{0}, {1}, {1}, {0}}; // XOR

    NeuralNetwork nn(2, 4, 1); // 2 entradas, 4 neuronas en la capa oculta, 1 salida

    nn.train(inputs, outputs, 10000, 0.5);

    cout << "Predicciones después del entrenamiento:" << endl;
    for (const auto& input : inputs) {
        vector<double> result = nn.predict(input);
        cout << input[0] << " XOR " << input[1] << " = " << result[0] << endl;
    }

    return 0;
}
