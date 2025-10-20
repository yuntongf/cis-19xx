#include "tensor.hpp"
#include "ops.hpp"
#include "activations.hpp"
#include "dense.hpp"
#include "nn.hpp"
#include "loss.hpp"
#include "train.hpp"
#include <iostream>

int main() {
    TinyNN<float> model;

    constexpr Tensor<float, 4, 2> X {{{0,0}, {0,1}, {1,0}, {1,1}}};
    constexpr Tensor<float, 4, 1> Y {{{0}, {1}, {1}, {0}}};

    // simple training loop
    for (int epoch = 0; epoch < 20000; ++epoch) {
        float total_loss = 0.0f;
        for (int i = 0; i < 4; ++i) {
            Tensor<float, 1, 2> x = {{{X(i,0), X(i,1)}}};
            Tensor<float, 1, 1> y_true = {{{Y(i,0)}}};
            TrainStep(model, x, y_true, 0.1f);
        }
        // if (epoch % 200 == 0) {
        //     std::cout << "[Epoch " << epoch << "]\n";
        // }
    }

    std::cout << "Predictions:\n";
    for (int i = 0; i < 4; ++i) {
        Tensor<float, 1, 2> x = {{{X(i,0), X(i,1)}}};
        auto y_pred = model.Forward(x);
        std::cout << "(" << X(i,0) << "," << X(i,1)
                  << ") -> " << y_pred(0,0) << "\n";
    }
}
