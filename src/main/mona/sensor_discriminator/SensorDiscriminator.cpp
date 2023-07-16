// Sensor discriminator.
// Discriminate input sensors by detecting signals that are important to the output.

#include <iostream>
#include <fstream>
#include "NeuralNetwork.h"

// Usage.
const char* Usage =
"Usage:\n"
"    sensor_discriminator\n"
"      [-sensor_indexes <indexes> :: = <sensors>; <sensors>; ... where <sensors> :: = <index>, <index>, ...]\n"
"      [-sensor_output_file <output file name>]\n"
"      [-signal_range <range percent>]\n"
"      [-noise_probability <probability>]\n"
"      [-dataset_size <size>]\n"
"      [-network_dimensions <input_dimension>, <hidden_dimension>]\n"
"      [-learning_rate <rate>]\n"
"      [-epochs <epochs>]\n"
"      [-random_seed <seed>]\n"
"      [-verbose <true | false> (default=true)]\n"
"  or\n"
"    sensor_discriminator\n"
"      -sensor_input_file <input file name>\n"
"      -sensor_output_file <output file name>\n"
"      [-signal_range <range percent>]\n"
"      [-network_hidden_dimension <dimension>]\n"
"      [-learning_rate <rate>]\n"
"      [-epochs <epochs>]\n"
"      [-random_seed <seed>]\n"
"      [-verbose <true | false> (default=true)]\n"
"Exit codes:\n"
"  0=success\n"
"  1=error\n";

// Dimensions.
int input_dim = 8;
int hidden_dim = 128;
int output_dim;

// Sensors.
vector<vector<int>> sensor_idxs = { {1}, {4}, {1, 4}, {3} };

// Files.
string sensor_input_filename;
string sensor_output_filename;

// Signal range percentage.
double signal_range = 0.8;

// Noise probability.
double noise_probability = 0.0;

// Dataset size.
int dataset_size = 20;

// Training epochs.
int epochs = 500;

// Random seed.
int random_seed = 4517;

// Verbose.
bool Verbose = true;

// Dataset.
vector<RowVector*> input, output;

// Sensors.
vector<vector<int>> sensors;

// Declarations.
void generateSensorDataset();
void readSensorDataset();
void train(NeuralNetwork& net);
void test(NeuralNetwork& net);
void discriminateSensors(NeuralNetwork& net, RowVector rowIn, RowVector rowOut);
void writeSensors();

// Split string on delimiter.
int split(string const& str, const char delim, vector<string>& out);

double unary(double x)
{
    return x > 1 ? 1 : x < 0 ? 0 : x;
}

// Main.
int main(int argc, char *args[]) 
{
    // Get options.
    bool gotNetworkDimensions = false;
    bool gotNetworkHiddenDimension = false;
    bool gotSensorIndexes = false;
    bool gotDatasetSize = false;
    for (int i = 1; i < argc; i++)
    {
        if (strcmp(args[i], "-?") == 0 || strcmp(args[i], "-help") == 0)
        {
            printf(Usage);
            exit(0);
        }
        if (strcmp(args[i], "-i") == 0 || strcmp(args[i], "-sensor_input_file") == 0)
        {
            i++;
            if (i >= argc)
            {
                fprintf(stderr, "Invalid sensor_input_file option\n");
                fprintf(stderr, Usage);
                exit(1);
            }
            sensor_input_filename = args[i];
            continue;
        }
        if (strcmp(args[i], "-o") == 0 || strcmp(args[i], "-sensor_output_file") == 0)
        {
            i++;
            if (i >= argc)
            {
                fprintf(stderr, "Invalid sensor_output_file option\n");
                fprintf(stderr, Usage);
                exit(1);
            }
            sensor_output_filename = args[i];
            continue;
        }
        if (strcmp(args[i], "-d") == 0 || strcmp(args[i], "-network_dimensions") == 0)
        {
            i++;
            if (i >= argc)
            {
                fprintf(stderr, "Invalid network_dimensions option\n");
                fprintf(stderr, Usage);
                exit(1);
            }
            vector<string> dimensions;
            if (split(string(args[i]), ',', dimensions) != 2)
            {
                fprintf(stderr, "invalid network_dimensions");
                fprintf(stderr, Usage);
                exit(1);
            }
            input_dim = atoi(dimensions[0].c_str());
            if (input_dim <= 0)
            {
                fprintf(stderr, "invalid input dimension");
                exit(1);
            }
            hidden_dim = atoi(dimensions[1].c_str());
            if (hidden_dim <= 0)
            {
                fprintf(stderr, "invalid hidden dimension");
                exit(1);
            }
            gotNetworkDimensions = true;
            continue;
        }
        if (strcmp(args[i], "-h") == 0 || strcmp(args[i], "-network_hidden_dimension") == 0)
        {
            i++;
            if (i >= argc)
            {
                fprintf(stderr, "Invalid hidden_dimension option\n");
                fprintf(stderr, Usage);
                exit(1);
            }
            hidden_dim = atoi(args[i]);
            if (hidden_dim <= 0)
            {
                fprintf(stderr, "invalid hidden dimension");
                exit(1);
            }
            gotNetworkHiddenDimension = true;
            continue;
        }
        if (strcmp(args[i], "-x") == 0 || strcmp(args[i], "-sensor_indexes") == 0)
        {
            i++;
            if (i >= argc)
            {
                fprintf(stderr, "Invalid sensor_indexes option\n");
                fprintf(stderr, Usage);
                exit(1);
            }
            sensor_idxs.clear();
            vector<string> sensors;
            split(string(args[i]), ';', sensors);
            for (int j = 0; j < sensors.size(); j++)
            {
                vector<string> sidxs;
                vector<int> idxs;
                split(sensors[j], ',', sidxs);
                for (string idx : sidxs)
                {
                    idxs.push_back(atoi(idx.c_str()));
                }
                sensor_idxs.push_back(idxs);
            }
            gotSensorIndexes = true;
            continue;
        }
        if (strcmp(args[i], "-s") == 0 || strcmp(args[i], "-signal_range") == 0)
        {
            i++;
            if (i >= argc)
            {
                fprintf(stderr, "Invalid signal_range option\n");
                fprintf(stderr, Usage);
                exit(1);
            }
            signal_range = atof(args[i]);
            if (signal_range < 0.0)
            {
                fprintf(stderr, "invalid signal_range");
                exit(1);
            }
            continue;
        }
        if (strcmp(args[i], "-p") == 0 || strcmp(args[i], "-noise_probability") == 0)
        {
            i++;
            if (i >= argc)
            {
                fprintf(stderr, "Invalid noise_probability option\n");
                fprintf(stderr, Usage);
                exit(1);
            }
            noise_probability = atof(args[i]);
            if (noise_probability < 0.0 || noise_probability > 1.0)
            {
                fprintf(stderr, "invalid noise_probability");
                exit(1);
            }
            continue;
        }
        if (strcmp(args[i], "-n") == 0 || strcmp(args[i], "-dataset_size") == 0)
        {
            i++;
            if (i >= argc)
            {
                fprintf(stderr, "Invalid dataset_size option\n");
                fprintf(stderr, Usage);
                exit(1);
            }
            dataset_size = atoi(args[i]);
            if (dataset_size < 0)
            {
                fprintf(stderr, "invalid dataset_size");
                exit(1);
            }
            gotDatasetSize = true;
            continue;
        }
        if (strcmp(args[i], "-e") == 0 || strcmp(args[i], "-epochs") == 0)
        {
            i++;
            if (i >= argc)
            {
                fprintf(stderr, "Invalid epochs option\n");
                fprintf(stderr, Usage);
                exit(1);
            }
            epochs = atoi(args[i]);
            if (epochs < 0)
            {
                fprintf(stderr, "invalid epochs");
                exit(1);
            }
            continue;
        }
        if (strcmp(args[i], "-l") == 0 || strcmp(args[i], "-learning_rate") == 0)
        {
            i++;
            if (i >= argc)
            {
                fprintf(stderr, "Invalid learning_rate option\n");
                fprintf(stderr, Usage);
                exit(1);
            }
            LEARNING_RATE = atof(args[i]);
            if (LEARNING_RATE <= 0.0)
            {
                fprintf(stderr, "invalid learning_rate");
                exit(1);
            }
            continue;
        }
        if (strcmp(args[i], "-r") == 0 || strcmp(args[i], "-random_seed") == 0)
        {
            i++;
            if (i >= argc)
            {
                fprintf(stderr, "Invalid random_seed option\n");
                fprintf(stderr, Usage);
                exit(1);
            }
            random_seed = atoi(args[i]);
            continue;
        }
        if (strcmp(args[i], "-v") == 0 || strcmp(args[i], "-verbose") == 0)
        {
            i++;
            if (i >= argc)
            {
                fprintf(stderr, "Invalid verbose option\n");
                fprintf(stderr, Usage);
                exit(1);
            }
            string v = args[i];
            if (v == "true")
            {
                Verbose = true;
            }
            else if (v == "false")
            {
                Verbose = false;
            }
            else {
                fprintf(stderr, "Invalid verbose option\n");
                fprintf(stderr, Usage);
                exit(1);
            }
            continue;
        }
        printf(Usage);
        exit(1);
    }
    if (sensor_input_filename.empty())
    {
        if (gotNetworkHiddenDimension)
        {
            fprintf(stderr, Usage);
            exit(1);
        }
        output_dim = (int)sensor_idxs.size();
    }
    else {
        if (gotNetworkDimensions || gotSensorIndexes || gotDatasetSize)
        {
            fprintf(stderr, Usage);
            exit(1);
        }
        if (sensor_output_filename.empty())
        {
            printf(Usage);
            exit(1);
        }
    }

    // Seed random numbers.
    srand(random_seed);

    vector<RowVector*> input, output;
    if (sensor_input_filename.empty())
    {
        // Generate sensor dataset.
        generateSensorDataset();
    }
    else {
        // Read sensor dataset.
        readSensorDataset();

    }

    // Run NN.
	NeuralNetwork net({ input_dim, hidden_dim, output_dim }, 0.05, NeuralNetwork::Activation::TANH);	
	train(net);
	test(net);
	//net.save("params.txt");
    if (Verbose)
    {
        cout << endl << "Neurons:" << endl;
        for (int i = 0; i < net.mNeurons.size(); i++)
            cout << *net.mNeurons[i] << endl;
        cout << endl << "Weights:" << endl;
        for (int i = 0; i < net.mWeights.size(); i++)
            cout << *net.mWeights[i] << endl;
    }

    // Write discriminated sensors.
    if (!sensor_output_filename.empty())
    {
        writeSensors();
    }

	return 0;
}

// Generate sensor dataset.
void generateSensorDataset()
{
    // off = 0.0, on = 1.0
    input.clear();
    output.clear();
    for (int i = 0; i < dataset_size; i++)
    {
        RowVector* rowIn = new RowVector(input_dim);
        for (int j = 0; j < input_dim; j++)
        {
            if ((rand() % 100) < (int)(noise_probability * 100.0f))
            {
                rowIn->coeffRef(j) = 1.0f;
            }
            else {
                rowIn->coeffRef(j) = 0.0f;
            }
        }
        int idx = rand() % (int)(sensor_idxs.size());
        for (int j = 0; j < sensor_idxs.size(); j++)
        {
            if (j != idx)
            {
                for (int k : sensor_idxs[j])
                {
                    rowIn->coeffRef(k) = 0.0f;
                }
            }
        }
        for (int j : sensor_idxs[idx])
        {
            rowIn->coeffRef(j) = 1.0f;
        }
        input.push_back(rowIn);
        RowVector* rowOut = new RowVector(output_dim);
        for (int j = 0; j < output_dim; j++)
        {
            if (j == idx)
            {
                rowOut->coeffRef(j) = 1.0f;
            }
            else {
                rowOut->coeffRef(j) = 0.0f;
            }
        }
        output.push_back(rowOut);
    }
}

// Read sensor dataset.
void readSensorDataset()
{
    input.clear();
    output.clear();
    ifstream file;
    file.open(sensor_input_filename);
    if (!file.is_open())
    {
        fprintf(stderr, "Cannot open sensor dataset file %s\n", sensor_input_filename.c_str());
        exit(1);
    }
    string line;
    vector<string> values;
    dataset_size = 0;
    input_dim = 0;
    output_dim = -1;
    while (getline(file, line)) 
    {
        dataset_size++;
        split(line, ',', values);
        input_dim = (int)values.size() - 1;
        int response = atoi(values[values.size() - 1].c_str());
        if (response > output_dim)
        {
            output_dim = response;
        }
    }
    output_dim++;
    file.close();
    file.open(sensor_input_filename);
    if (!file.is_open())
    {
        fprintf(stderr, "Cannot open sensor dataset file %s\n", sensor_input_filename.c_str());
        exit(1);
    }
    while (getline(file, line))
    {
        split(line, ',', values);
        RowVector* rowIn = new RowVector(input_dim);
        for (int i = 0; i < input_dim; i++)
        {
            rowIn->coeffRef(i) = atof(values[i].c_str());
        }
        input.push_back(rowIn);
        int response = atoi(values[values.size() - 1].c_str());
        RowVector* rowOut = new RowVector(output_dim);
        for (int i = 0; i < output_dim; i++)
        {
            if (i == response)
            {
                rowOut->coeffRef(i) = 1.0f;
            }
            else {
                rowOut->coeffRef(i) = 0.0f;
            }
        }
        output.push_back(rowOut);
    }
    file.close();
}

// Train.
void train(NeuralNetwork& net)
{
    if (Verbose)
    {
        cout << "Training:" << endl;
    }

    int stop = 0;
    for (int i = 0; stop < 8 && i < epochs; i++) {
        if (Verbose)
        {
            cout << i + 1 << endl;
        }
        for (int num = 0; stop < 8 && num < dataset_size; num++) {
            net.train(*input[num], *output[num]);
            double mse = net.mse();
            if (Verbose)
            {
                cout << "In [" << *input[num] << "] "
                    << " Desired [" << *output[num] << "] "
                    << " Out [" << net.mNeurons.back()->unaryExpr(ptr_fun(unary)) << "] "
                    << " MSE [" << mse << "]" << endl;
            }
            stop = mse < 0.2 ? stop + 1 : 0;
        }
    }
}

// Test.
void test(NeuralNetwork& net)
{
    if (Verbose)
    {
        cout << "Testing:" << endl;
    }

    sensors.clear();
    int correct = 0;
    for (int num = 0; num < dataset_size; num++) 
    {
        net.test(*input[num], *output[num]);

        double mse = net.mse();
        if (Verbose)
        {
            cout << "In [" << *input[num] << "] "
                << " Desired [" << *output[num] << "] "
                << " Out [" << net.mNeurons.back()->unaryExpr(ptr_fun(unary)) << "] "
                << " MSE [" << mse << "]" << endl;
        }

        int maxidx1 = -1;
        int maxidx2 = -1;
        double v1, v2;
        RowVector* activations = net.mNeurons.back();
        for (int i = 0; i < output_dim; i++)
        {
            double v = activations->coeffRef(i);
            if (maxidx1 == -1 || v > v1)
            {
                maxidx1 = i;
                v1 = v;
            }
            v = output[num]->coeffRef(i);
            if (maxidx2 == -1 || v > v2)
            {
                maxidx2 = i;
                v2 = v;
            }
        }
        if (maxidx1 == maxidx2)
        {
            correct++;

            // Sensor discrimination.
            discriminateSensors(net, *input[num], *output[num]);
        }
    }

    if (dataset_size > 0)
    {
        cout << "Correct=" << correct << "/" << dataset_size << " (" << (((double)correct / (double)dataset_size) * 100.0) << "%)" << endl;
    }
}

// Discriminate sensors.
void discriminateSensors(NeuralNetwork& net, RowVector rowIn, RowVector rowOut)
{
    vector<int> signals(input_dim);
    RowVector* activations = net.mNeurons.back();
    int maxOutIdx = -1;
    double maxOutVal = 0.0;
    for (int i = 0; i < output_dim; i++)
    {
        double v = activations->coeffRef(i);
        if (maxOutIdx == -1 || v > maxOutVal)
        {
            maxOutIdx = i;
            maxOutVal = v;
        }
    }
    if (Verbose)
    {
        cout << "activations: [ ";
        for (int i = 0; i < output_dim; i++)
        {
            cout << activations->coeffRef(i) << " ";
        }
        cout << "]" << endl;
        cout << "maxOutIdx=" << maxOutIdx << ", maxOutVal=" << maxOutVal << endl;
    }
    if (maxOutIdx != -1 && maxOutVal > 0.0)
    {
        int maxInIdx = -1;
        double maxInDelta = 0.0;
        for (int i = 0; i < input_dim; i++)
        {
            if (rowIn.coeffRef(i) > 0.0)
            {
                double v = rowIn.coeffRef(i);
                rowIn.coeffRef(i) = 0.0;
                net.test(rowIn, rowOut);
                activations = net.mNeurons.back();
                rowIn.coeffRef(i) = v;
                if (Verbose)
                {
                    cout << "zero index=" << i << ", activations: [ ";
                    for (int j = 0; j < output_dim; j++)
                    {
                        cout << activations->coeffRef(j) << " ";
                    }
                    cout << "]" << endl;
                }
                double d = maxOutVal - activations->coeffRef(maxOutIdx);
                if (d > maxOutVal) d = maxOutVal;
                if (d > 0.0 && (maxInIdx == -1 || d > maxInDelta))
                {
                    maxInIdx = i;
                    maxInDelta = d;
                }
            }
        }
        if (maxInIdx != -1)
        {
            if (Verbose)
            {
                cout << "maxInIdx=" << maxInIdx << ", maxInDelta=" << maxInDelta << endl;
            }
            for (int i = 0; i < input_dim; i++)
            {
                if (rowIn.coeffRef(i) > 0.0)
                {
                    double v = rowIn.coeffRef(i);
                    rowIn.coeffRef(i) = 0.0;
                    net.test(rowIn, rowOut);
                    activations = net.mNeurons.back();
                    rowIn.coeffRef(i) = v;
                    double d = maxOutVal - activations->coeffRef(maxOutIdx);
                    if (d > maxOutVal) d = maxOutVal;
                    if (d > 0.0 && (d / maxInDelta) >= signal_range)
                    {
                        signals[i] = 1;
                    }
                }
            }
        }
    }
    bool match = true;
    for (int i = 0; i < input_dim; i++)
    {
        if (rowIn.coeffRef(i) > 0.0)
        {
            if (signals[i] == 0)
            {
                match = false;
                break;
            }
        }
        else {
            if (signals[i] == 1)
            {
                match = false;
                break;
            }
        }
    }
    if (!match)
    {
        sensors.push_back(signals);
    }
    if (Verbose)
    {
        cout << "Sensors [ ";
        for (int i : signals)
        {
            cout << i << " ";
        }
        cout << "]" << endl;
    }
}

// Write sensors.
void writeSensors()
{
    ofstream file;
    file.open(sensor_output_filename);
    if (!file.is_open())
    {
        fprintf(stderr, "Cannot open sensor output file %s\n", sensor_output_filename.c_str());
        exit(1);
    }
    for (int i = 0, n = (int)sensors.size(); i < n; i++)
    {
        bool write = true;
        for (int j = 0; j < i; j++)
        {
            bool dup = true;
            for (int k = 0; k < input_dim; k++)
            {
                if (sensors[j][k] != sensors[i][k])
                {
                    dup = false;
                    break;
                }
            }
            if (dup)
            {
                write = false;
                break;
            }
        }
        if (write)
        {
            for (int j = 0; j < input_dim; j++)
            {
                file << sensors[i][j];
                if (j < input_dim - 1)
                {
                    file << ",";
                }
            }
            file << endl;
        }
    }
    file.close();
}

// Split string.
int split(string const& str, const char delim, vector<string>& out)
{
    size_t start;
    size_t end = 0;

    out.clear();
    while ((start = str.find_first_not_of(delim, end)) != string::npos)
    {
        end = str.find(delim, start);
        out.push_back(str.substr(start, end - start));
    }
    return (int)out.size();
}

