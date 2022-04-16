#include <vector>
#include <iostream>
#include <cstdlib>
#include <cassert>
#include <cmath>
#include <sstream>
#include <fstream>

using namespace std;

class TrainingData
{
public:
    TrainingData(const string filename);
    bool isEof(void)
    {
        return m_trainingDataFile.eof();
    }
    void getTopology(vector<unsigned> &topology);

    // Returns the number of input values read from the file:
    unsigned getNextInputs(vector<double> &inputVals);
    unsigned getTargetOutputs(vector<double> &targetOutputVals);

private:
    ifstream m_trainingDataFile;
};

void TrainingData::getTopology(vector<unsigned> &topology)
{
    string line;
    string label;

    getline(m_trainingDataFile, line);
    stringstream ss(line);
    ss >> label;
    if (this->isEof() || label.compare("topology:") != 0)
    {
        abort();
    }

    while (!ss.eof())
    {
        unsigned n;
        ss >> n;
        topology.push_back(n);
    }
    return;
}

TrainingData::TrainingData(const string filename)
{
    m_trainingDataFile.open(filename.c_str());
}

unsigned TrainingData::getNextInputs(vector<double> &inputVals)
{
    inputVals.clear();

    string line;
    getline(m_trainingDataFile, line);
    stringstream ss(line);

    string label;
    ss >> label;
    if (label.compare("in:") == 0)
    {
        double oneValue;
        while (ss >> oneValue)
        {
            inputVals.push_back(oneValue);
        }
    }

    return inputVals.size();
}

unsigned TrainingData::getTargetOutputs(vector<double> &targetOutputVals)
{
    targetOutputVals.clear();

    string line;
    getline(m_trainingDataFile, line);
    stringstream ss(line);

    string label;
    ss >> label;
    if (label.compare("out:") == 0)
    {
        double oneValue;
        while (ss >> oneValue)
        {
            targetOutputVals.push_back(oneValue);
        }
    }

    return targetOutputVals.size();
}

struct Connection
{
    double weight;
    double deltaWeight;
};

class Neuron;

typedef vector<Neuron> Layer;
// ************************ NEURON CLASS ******************************* //

class Neuron
{
public:
    Neuron(unsigned NumOutputs, unsigned myIndex);
    void setOutputValue(double val) { m_outputValue = val; };
    double getOutputValue(void) const { return m_outputValue; };
    void feedForward(const Layer &prevLayer);
    void calculateGradients(double targetValues);
    void calculateHiddenGradients(const Layer &nextLayer);
    void updateInputWeights(Layer &prevLayer);

private:
    static double eta;   //[0.0 - 1.0] learning late
    static double alpha; //[0.0 ... n] multipluer of the last weight
    static double transferFunction(double x);
    static double transferFunctionDerivative(double x);
    static double randomWeight(void) { return rand() / double(RAND_MAX); }
    double sumDOW(const Layer &nextLayer) const;
    double m_outputValue;
    vector<Connection> m_outputWeights;
    unsigned m_myIndex;
    double m_gradient;
};

double Neuron::eta = 0.15;  // A mid - slow learning rate
double Neuron::alpha = 0.5; // Moderate momentum

void Neuron::updateInputWeights(Layer &prevLayer)
{
    // We take the weight from the preceding layer
    // Connection container and we update out current Neuron

    for (unsigned n = 0; n < prevLayer.size(); ++n)
    {
        Neuron &neuron = prevLayer[n];
        double oldDeltaWeight = neuron.m_outputWeights[m_myIndex].deltaWeight;

        // eta is the net learning rate
        // alpga is the momentum , a fration of the prev delta weight
        double newDeltaWeight = eta * neuron.getOutputValue() * m_gradient + alpha * oldDeltaWeight;

        neuron.m_outputWeights[m_myIndex].deltaWeight = newDeltaWeight;
        neuron.m_outputWeights[m_myIndex].weight += newDeltaWeight;
    }
}

double Neuron::sumDOW(const Layer &nextLayer) const
{
    double sum = 0.0;
    //  Sum our contribution of the erros at the nodes we feed

    for (unsigned n = 0; n < nextLayer.size() - 1; ++n)
    {
        sum += m_outputWeights[n].weight * nextLayer[n].m_gradient;
    }

    return sum;
}

void Neuron::calculateHiddenGradients(const Layer &nextLayer)
{
    double dow = sumDOW(nextLayer);
    m_gradient = dow * Neuron::transferFunctionDerivative(m_outputValue);
};

void Neuron::calculateGradients(double targetValues)
{
    double delta = targetValues - m_outputValue;
    m_gradient = delta * Neuron::transferFunctionDerivative(m_outputValue);
};

double Neuron::transferFunction(double x)
{
    // tanh - output range[-1.0,1.0]
    return tanh(x);
    // sigmod [0.0, 1.0]

    // return (1 / (1 + 1 / exp(x)));
}

double Neuron::transferFunctionDerivative(double x)
{
    // tanh derivative
    return 1.0 - x * x;
    // sigmoid derivative
    // return x * (1 - x);
}

void Neuron::feedForward(const Layer &prevLayer)
{
    double sum = 0.0;
    // Sum previous layer outputs includint the bias node

    for (unsigned i = 0; i < prevLayer.size(); ++i)
    {
        sum += prevLayer[i].getOutputValue() * prevLayer[i].m_outputWeights[m_myIndex].weight;
    }

    m_outputValue = Neuron::transferFunction(sum);
}

Neuron::Neuron(unsigned numOutputs, unsigned myIndex)
{
    for (unsigned c = 0; c < numOutputs; ++c)
    {
        m_outputWeights.push_back(Connection());
        m_outputWeights.back().weight = randomWeight();
    }

    m_myIndex = myIndex;
}

// ************************ NEURON NETWORK CLASS ******************************* //
class Network
{
public:
    Network(const vector<unsigned> &topology);
    void feedForward(const vector<double> &inputValues);
    void backPropagate(const vector<double> &targetValues);
    void getResults(vector<double> &resultValues) const;
    double getRecentAverageError(void) const { return m_recentAverageError; };

private:
    vector<Layer> m_layers; // Layers[layernum][neuronnum]
    double m_error;
    double m_recentAverageError;
    double m_recentAverageSmoothingFactor;
};

void Network::getResults(vector<double> &resultValues) const
{
    resultValues.clear();

    for (unsigned n = 0; n < m_layers.back().size() - 1; ++n)
    {
        resultValues.push_back(m_layers.back()[n].getOutputValue());
    }
};

void Network::backPropagate(const vector<double> &targetValues)
{
    // Calculate overall  root mean square error

    Layer &outputLayer = m_layers.back();
    m_error = 0.0;

    for (unsigned n = 0; n < outputLayer.size(); ++n)
    {
        double delta = targetValues[n] - outputLayer[n].getOutputValue();
        m_error += delta * delta;
    }
    m_error /= outputLayer.size() - 1; // Average error squared
    m_error = sqrt(m_error);           // RMS

    // Recent average error
    m_recentAverageError =
        (m_recentAverageError * m_recentAverageSmoothingFactor + m_error) / (m_recentAverageSmoothingFactor + 1.0);

    // Calculate output layer gradients

    for (unsigned n = 0; n < outputLayer.size(); ++n)
    {
        outputLayer[n].calculateGradients(targetValues[n]);
    }

    // Calculate gradients on hidden layers

    for (unsigned layerNum = m_layers.size() - 2; layerNum > 0; --layerNum)
    {
        Layer &hiddenLayer = m_layers[layerNum];
        Layer &nextLayer = m_layers[layerNum + 1];

        for (unsigned n = 0; n < hiddenLayer.size(); ++n)
        {
            hiddenLayer[n].calculateHiddenGradients(nextLayer);
        }
    }

    // Update weights to both first and hidden layers the connection weights

    for (unsigned layerNum = m_layers.size() - 1; layerNum > 0; --layerNum)
    {
        Layer &layer = m_layers[layerNum];
        Layer &prevLayer = m_layers[layerNum - 1];

        for (unsigned n = 0; n < layer.size() - 1; ++n)
        {
            layer[n].updateInputWeights(prevLayer);
        }
    }
}

void Network::feedForward(const vector<double> &inputValues)
{
    assert(inputValues.size() == m_layers[0].size() - 1);

    // Assign the input vaules into the input neurons
    for (unsigned i = 0; i < inputValues.size(); ++i)
    {
        m_layers[0][i].setOutputValue(inputValues[i]);
    }

    // Forward propagate

    for (unsigned layerNum = 1; layerNum < m_layers.size(); ++layerNum)
    {
        Layer &prevLayer = m_layers[layerNum - 1];
        for (unsigned n = 0; n < m_layers[layerNum].size() - 1; ++n)
        {
            m_layers[layerNum][n].feedForward(prevLayer);
        }
    }
};

Network::Network(const vector<unsigned> &topology)
{
    unsigned numLayers = topology.size();
    for (unsigned layerNum = 0; layerNum < numLayers; ++layerNum)
    {
        // Add a new layer
        m_layers.push_back(Layer());
        unsigned numOutputs = layerNum == topology.size() - 1 ? 0 : topology[layerNum + 1];
        // Now add the neuros and a bias neuron to this layer.
        for (unsigned neuronNum = 0; neuronNum <= topology[layerNum]; ++neuronNum)
        {
            m_layers.back().push_back(Neuron(numOutputs, neuronNum));
            // cout << "Added a neuron" << endl;
        }
        // last neuron must have bias 1.0
        m_layers.back().back().setOutputValue(1.0);
    }
}

void showVectorValues(string label, vector<double> &v)
{
    cout << label << " ";
    for (unsigned i = 0; i < v.size(); ++i)
    {
        cout << v[i] << " ";
    }
    cout << endl;
}

int main()
{
    TrainingData trainData("./tmp/train.txt");

    vector<unsigned> topology;
    trainData.getTopology(topology);
    Network myNetwork(topology);

    vector<double> inputValues, targetValues, resultValues;
    int trainingPass = 0;
    // int epochs = 10;
    // for (int i = 0; i < epochs; i++)
    // {
    while (!trainData.isEof())
    {
        ++trainingPass;
        cout << endl
             << "Imput number: " << trainingPass;

        // Get new input data and feed forward:
        if (trainData.getNextInputs(inputValues) != topology[0])
        {
            break;
        }
        showVectorValues(": Inputs:", inputValues);
        myNetwork.feedForward(inputValues);

        // Collect neural nets actual results:
        myNetwork.getResults(resultValues);
        showVectorValues("Outputs:", resultValues);

        // Train the network
        trainData.getTargetOutputs(targetValues);
        showVectorValues("Target:", targetValues);
        assert(targetValues.size() == topology.back());

        myNetwork.backPropagate(targetValues);

        // Report network efficiency
        cout << "Neural Network average error: "
             << myNetwork.getRecentAverageError() << endl;
    }
    // }
    cout << endl
         << "Done";
}