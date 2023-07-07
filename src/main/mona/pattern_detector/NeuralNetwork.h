// Source code: https://www.codeproject.com/Articles/5292985/Artificial-Neural-Network-Cplusplus-Class
// Requires Eigen package: https://eigen.tuxfamily.org

#pragma once

#include "Eigen/Eigen" 
#include <vector> 

typedef Eigen::RowVectorXd RowVector;
typedef Eigen::MatrixXd Matrix;
using namespace std;

extern double LEARNING_RATE;

class NeuralNetwork {
public:
    enum Activation { TANH, SIGMOID } mActivation;
    // network learning rate
    double mLearningRate;
    // network layers architecture (input-hidden-output) 
    vector<int> mArchitecture;
    // network layers vectors
    vector<RowVector*> mNeurons;
    // neurons' output errors
    vector<RowVector*> mErrors;
    // connections' weights
    vector<Matrix*> mWeights;
    // confusion matrix
    Matrix* mConfusion;

    // constructors
    NeuralNetwork();
    ~NeuralNetwork();
    NeuralNetwork(
        vector<int> architecture, 
        double learningRate = LEARNING_RATE,
        Activation = TANH);
    void init(
        vector<int> architecture, 
        double learningRate = LEARNING_RATE,
        Activation = TANH);
    // load from file
    bool load(const char* file);

    // save to file
    void save(const char* file);

    // data forward propagation
    void forward(RowVector& input);
    double activation(double x);

    // backward propagation of errors
    void backward(RowVector& output);
    double activationDerivative(double x);

    // train the neural network given an input
    void train(RowVector& input, RowVector& output);

    // test the neural network given an input
    void test(RowVector& input, RowVector& output);

    void resetConfusion();
    void evaluate(RowVector& output);
    void confusionMatrix(RowVector*& precision, RowVector*& recall);

    // get max output index
    int vote(double& value);
    int vote(RowVector& v, double& value);

    // get last layer output
    double output(int col);
    
    // get output layer mean squere error
    double mse();
};