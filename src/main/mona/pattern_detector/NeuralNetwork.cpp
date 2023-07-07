#include <iostream>
#include <fstream> 
#include "NeuralNetwork.h"
#include "math.h"

double LEARNING_RATE = 0.01;

NeuralNetwork::NeuralNetwork() {
	mConfusion = nullptr;
}

NeuralNetwork::~NeuralNetwork() {
	if (mConfusion)
		delete mConfusion;
}

NeuralNetwork::NeuralNetwork(
	vector<int> architecture,
	double learningRate /*= LEARNING_RATE*/,
	Activation activation /*= TANH*/) {
	init(architecture, learningRate, activation);
}

void NeuralNetwork::init(
	vector<int> architecture,
	double learningRate /*= LEARNING_RATE*/,
	Activation activation/*= TANH*/) {
	mArchitecture = architecture;
	mLearningRate = learningRate;
	mActivation = activation;

	mNeurons.clear();
	mErrors.clear();
	mWeights.clear();

	for (unsigned int i = 0; i < architecture.size(); i++) {
		// add extra neuron to each layer as a bias (with weight = 1)
		int size = architecture[i] + (i != architecture.size() - 1);
		mNeurons.push_back(new RowVector(size));
		mErrors.push_back(new RowVector(size));

		mNeurons.back()->setZero();
		if (i < architecture.size() - 1)
			// set bias to 1 (used in vectors dot product)
			mNeurons.back()->coeffRef(architecture[i]) = 1.0;

		// initialize weights
		if (i > 0) {
			if (i != architecture.size() - 1) {
				mWeights.push_back(new Matrix(architecture[i - 1] + 1, architecture[i] + 1));
				mWeights.back()->setRandom();
				mWeights.back()->col(architecture[i]).setZero();
				mWeights.back()->coeffRef(architecture[i - 1], architecture[i]) = 1.0;
			} else {
			mWeights.push_back(new Matrix(architecture[i - 1] + 1, architecture[i]));
			mWeights.back()->setRandom();
			}
		}
	}
	mConfusion = new Matrix(architecture.back(), architecture.back());
	mConfusion->setZero();
}

double NeuralNetwork::activation(double x) {
	if (mActivation == TANH)
		return tanh(x);
	if (mActivation == SIGMOID)
		return 1.0 / (1.0 + exp(-x));
	return 0;
}

double NeuralNetwork::activationDerivative(double x) {
	if (mActivation == TANH)
		return 1 - tanh(x) * tanh(x);
	if (mActivation == SIGMOID)
		return x * (1.0 - x);
	return 0;
}

void NeuralNetwork::forward(RowVector& input) {
	// set first layer input
	mNeurons.front()->block(0, 0, 1, input.size()) = input;

	// propagate forward (vector multiplication)
	for (unsigned int i = 1; i < mArchitecture.size(); i++) {
		// copy values ingoring last neuron as it is a bias
		mNeurons[i]->block(0, 0, 1, mArchitecture[i]) = (*mNeurons[i - 1] * *mWeights[i - 1]).block(0, 0, 1, mArchitecture[i]);
		// apply activation function
		for (int col = 0; col < mArchitecture[i]; col++)
			mNeurons[i]->coeffRef(col) = activation(mNeurons[i]->coeffRef(col));
	}
}

void NeuralNetwork::test(RowVector& input, RowVector& output) {
	forward(input);
	// calculate last layer errors
	*mErrors.back() = output - *mNeurons.back();
}

void NeuralNetwork::resetConfusion() {
	if (mConfusion)
		mConfusion->setZero();
}

void NeuralNetwork::evaluate(RowVector& output) {
	double desired = 0, actual = 0;
	mConfusion->coeffRef(
		vote(output, desired),
		vote(*mNeurons.back(), actual)
	)++;
}

void NeuralNetwork::confusionMatrix(RowVector*& precision, RowVector*& recall) {
	int rows = (int)mConfusion->rows();
	int cols = (int)mConfusion->cols();

	precision = new RowVector(cols);
	for (int col = 0; col < cols; col++) {
		double colSum = 0;
		for (int row = 0; row < rows; row++)
			colSum += mConfusion->coeffRef(row, col);
		precision->coeffRef(col) = mConfusion->coeffRef(col, col) / colSum;
	}

	recall = new RowVector(rows);
	for (int row = 0; row < rows; row++) {
		double rowSum = 0;
		for (int col = 0; col < cols; col++)
			rowSum += mConfusion->coeffRef(row, col);
		recall->coeffRef(row) = mConfusion->coeffRef(row, row) / rowSum;
	}

	// convert confusion to percentage 
	for (int row = 0; row < rows; row++) {
		double rowSum = 0;
		for (int col = 0; col < cols; col++)
			rowSum += mConfusion->coeffRef(row, col);
		for (int col = 0; col < cols; col++)
			mConfusion->coeffRef(row, col) = mConfusion->coeffRef(row, col) * 100 / rowSum;
	}
}


void NeuralNetwork::backward(RowVector& output) {
	// calculate last layer errors
	*mErrors.back() = output - *mNeurons.back();

	// calculate hidden layers' errors (vector multiplication)
	for (size_t i = mErrors.size() - 2; i > 0; i--)
		*mErrors[i] = *mErrors[i + 1] * mWeights[i]->transpose();

	// update weights
	for (size_t i = 0, size = mWeights.size(); i < size; i++)
		for (int row = 0, rows = (int)mWeights[i]->rows(); row < rows; row++)
			for (int col = 0, cols = (int)mWeights[i]->cols(); col < cols; col++)
				mWeights[i]->coeffRef(row, col) +=
				mLearningRate *
				mErrors[i + 1]->coeffRef(col) *
				mNeurons[i]->coeffRef(row) *
				activationDerivative(mNeurons[i + 1]->coeffRef(col));
}

void NeuralNetwork::train(RowVector& input, RowVector& output) {
	forward(input);
	backward(output);
}

// mean square error
double NeuralNetwork::mse() {
	return sqrt((*mErrors.back()).dot((*mErrors.back())) / mErrors.back()->size());
}

int NeuralNetwork::vote(double& value) {
	auto it = mNeurons.back();
	return vote(*it, value);
}

int NeuralNetwork::vote(RowVector& v, double& value) {
	int index = 0;
	for (int i = 1; i < v.cols(); i++)
		if (v[i] > v[index])
			index = i;
	value = v[index];
	return index;
}

double NeuralNetwork::output(int col) {
	auto it = mNeurons.back();
	return (*it)[col];
}

void NeuralNetwork::save(const char* filename) {
	stringstream tplgy;
	for (auto it = mArchitecture.begin(), _end = mArchitecture.end(); it != _end; it++)
		tplgy << *it << (it != _end - 1 ? "," : "");

	stringstream wts;
	for (auto it = mWeights.begin(), _end = mWeights.end(); it != _end; it++)
		wts << **it << (it != _end - 1 ? "," : "") << endl;

	ofstream file(filename);
	file << "learningRate: " << mLearningRate << endl;
	file << "architecture: " << tplgy.str() << endl;
	file << "activation: " << mActivation << endl;
	file << "weights: " << endl << wts.str() << endl;
	file.close();
}

bool NeuralNetwork::load(const char* filename) {
	mArchitecture.clear();

	ifstream file(filename);
	if (!file.is_open())
		return false;
	string line, name, value;
	if (!getline(file, line, '\n'))
		return false;
	stringstream lr(line);

	// read learning rate
	getline(lr, name, ':');
	if (name != "learningRate")
		return false;
	if (!getline(lr, value, '\n'))
		return false;
	mLearningRate = atof(value.c_str());

	// read topoplogy
	getline(file, line, '\n');
	stringstream ss(line);
	getline(ss, name, ':');
	if (name != "architecture")
		return false;
	while (getline(ss, value, ','))
		mArchitecture.push_back(atoi(value.c_str()));

	// read activation
	getline(file, line, '\n');
	stringstream sss(line);
	getline(sss, name, ':');
	if (name != "activation")
		return false;
	if (!getline(sss, value, '\n'))
		return false;
	mActivation = (Activation)atoi(value.c_str());

	// initialize using read architecture
	init(mArchitecture, mLearningRate, mActivation);

	// read weights
	getline(file, line, '\n');
	stringstream we(line);
	getline(we, name, ':');
	if (name != "weights")
		return false;

	string matrix;
	for (int i = 0; i < mArchitecture.size(); i++)
		if (getline(file, matrix, ',')) {
			stringstream ss(matrix);
			int row = 0;
			while (getline(ss, value, '\n'))
				if (!value.empty()) {
					stringstream word(value);
					int col = 0;
					while (getline(word, value, ' '))
						if (!value.empty())
							mWeights[i]->coeffRef(row, col++) = atof(value.c_str());
					row++;
				}
		}

	file.close();
	return true;
}