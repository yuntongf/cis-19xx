#pragma once

#include <iostream>

#include "nn.hpp"
#include "loss.hpp"

template <typename T>
void TrainStep(TinyNN<T>& model,
                const Tensor<T, 1, 2>& x,
                const Tensor<T, 1, 1>& y_true,
                T lr = 0.1) {

    // ---- forward ----
    auto z1 = Dense(x, model.W1, model.B1);
    auto h = Relu(z1);
    auto z2 = Dense(h, model.W2, model.B2);
    auto y_pred = Sigmoid(z2);

    // ---- loss and gradient ----
    auto dL_dy = MseGrad(y_pred, y_true);

    // ---- Backward ----
    // Sigmoid derivative: s'(x) = s(x) * (1 - s(x))
    Tensor<T, 1, 1> dy_dz2{};
    dy_dz2(0,0) = y_pred(0,0) * (1 - y_pred(0,0));

    auto dL_dz2 = Tensor<T, 1, 1>{{{dL_dy(0,0) * dy_dz2(0,0)}}};

    // Gradients for W2, B2
    Tensor<T, 2, 1> dL_dW2{};
    for (size_t i = 0; i < 2; ++i)
        dL_dW2(i,0) = h(0,i) * dL_dz2(0,0);

    Tensor<T, 1, 1> dL_dB2 = dL_dz2;

    // Propagate to hidden layer
    Tensor<T, 1, 2> dL_dh{};
    for (size_t j = 0; j < 2; ++j)
        dL_dh(0,j) = dL_dz2(0,0) * model.W2(j,0);

    // relu derivative
    Tensor<T, 1, 2> dh_dz1{};
    for (size_t j = 0; j < 2; ++j)
        dh_dz1(0,j) = (z1(0,j) > 0) ? 1 : 0;

    Tensor<T, 1, 2> dL_dz1{};
    for (size_t j = 0; j < 2; ++j)
        dL_dz1(0,j) = dL_dh(0,j) * dh_dz1(0,j);

    // Gradients for W1, B1
    Tensor<T, 2, 2> dL_dW1{};
    for (size_t i = 0; i < 2; ++i)
        for (size_t j = 0; j < 2; ++j)
            dL_dW1(i,j) = x(0,i) * dL_dz1(0,j);

    Tensor<T, 1, 2> dL_dB1 = dL_dz1;

    // gradient descent update
    for (size_t i = 0; i < 2; ++i)
        for (size_t j = 0; j < 2; ++j)
            model.W1(i,j) -= lr * dL_dW1(i,j);

    for (size_t j = 0; j < 2; ++j)
        model.B1(0,j) -= lr * dL_dB1(0,j);

    for (size_t i = 0; i < 2; ++i)
        model.W2(i,0) -= lr * dL_dW2(i,0);

    model.B2(0,0) -= lr * dL_dB2(0,0);

    T loss = MseLoss(y_pred, y_true);
    // std::cout << "Loss: " << loss << "\n";
}
