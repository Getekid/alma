import numpy as np


def sigmoid(x):
    # Our activation function: f(x) = 1 / (1 + e^(-x))
    return 1 / (1 + np.exp(-x))


def deriv_sigmoid(x):
    # Derivative of sigmoid: f'(x) = f(x) * (1 - f(x))
    fx = sigmoid(x)
    return fx * (1 - fx)


def mse_loss(y_true, y_pred):
    # y_true and y_pred are numpy arrays of the same length.
    return ((y_true - y_pred) ** 2).mean()


class Neuron:
    def __init__(self, weights, bias):
        self.weights = weights
        self.bias = bias

    def get_total(self, inputs):
        # Weight inputs, add bias.
        return (self.weights @ inputs) + self.bias

    def feedforward(self, total):
        # Use the activation function
        return sigmoid(total)

    def train(self, y_pred):
        self.weights = np.array([])


class OurNeuralNetwork:
    """
    A neural network with:
    - 2 inputs
    - a hidden layer with 2 neurons (h1, h2)
    - an output layer with 1 neuron (o1)
    Each neuron has the same weights and bias:
    - w = [0, 1]
    - b = 0
    """
    def __init__(self):
        self.h1 = Neuron(np.array([np.random.normal(), np.random.normal()]), np.random.normal())
        self.h2 = Neuron(np.array([np.random.normal(), np.random.normal()]), np.random.normal())
        self.o1 = Neuron(np.array([np.random.normal(), np.random.normal()]), np.random.normal())
        learn_rate = 0.1

    def feedforward(self, x):
        sum_h1 = self.h1.get_total(x)
        out_h1 = self.h1.feedforward(sum_h1)
        sum_h2 = self.h2.get_total(x)
        out_h2 = self.h2.feedforward(sum_h2)
        sum_o1 = self.o1.get_total(np.array([out_h1, out_h2]))
        return self.o1.feedforward(sum_o1)

    def train(self, data, all_y_trues):
        """
        - data is a (n x 2) numpy array, n = # of samples in the dataset.
        - all_y_trues is a numpy array with n elements.
          Elements in all_y_trues correspond to those in data.
        """
        epochs = 1000  # number of times to loop through the entire dataset

        for epoch in range(epochs):
            for x, y_true in zip(data, all_y_trues):
                y_pred = self.feedforward(x)
                self.h1.train(y_pred)
                self.h2.train(y_pred)
                self.o1.train(y_pred)

                # --- Calculate partial derivatives.
                # --- Naming: d_L_d_w1 represents "partial L / partial w1"
                d_L_d_ypred = -2 * (y_true - y_pred)


network = OurNeuralNetwork()
x = np.array([2, 3])
print(network.feedforward(x))  # 0.7216325609518421
