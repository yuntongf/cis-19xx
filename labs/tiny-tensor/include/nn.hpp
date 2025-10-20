#pragma once

#include "tensor.hpp"
#include "dense.hpp"
#include "activations.hpp"

template <typename T>
struct TinyNN {
    // hard-coded architecture: 2->2->1 for XOR
    Tensor<T, 2, 2> W1;
    Tensor<T, 1, 2> B1;
    Tensor<T, 2, 1> W2;
    Tensor<T, 1, 1> B2;

    constexpr TinyNN()
        : W1{{{5.0f, -5.0f}, {-5.0f, 5.0f}}},   // two opposite directions
        B1{{{-2.0f, -2.0f}}},                 // center thresholds near 0.5
        W2{{{5.0f}, {5.0f}}},                 // both hidden units contribute positively
        B2{{{-4.0f}}}                         // pushes down unless one is on
    {}

    constexpr Tensor<T, 1, 1> Forward(const Tensor<T, 1, 2>& x) const {
        auto h = Relu(Dense(x, W1, B1));
        return Sigmoid(Dense(h, W2, B2));
    }
};
