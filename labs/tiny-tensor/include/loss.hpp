#pragma once

#include "tensor.hpp"
#include <cmath>

// [TODO] implement the mean square error loss function
template <typename T, size_t R, size_t C>
T MseLoss(const Tensor<T, R, C>& pred, const Tensor<T, R, C>& target) {
    T sum = 0;
    for (size_t i = 0; i < R; ++i)
        for (size_t j = 0; j < C; ++j) {
            T diff = pred(i,j) - target(i,j);
            sum += diff * diff;
        }
    return sum / static_cast<T>(R * C);
}

// [TODO] Implement calculation of derivative of MSE loss w.r.t prediction
template <typename T, size_t R, size_t C>
Tensor<T, R, C> MseGrad(const Tensor<T, R, C>& pred, const Tensor<T, R, C>& target) {
    Tensor<T, R, C> grad{};
    for (size_t i = 0; i < R; ++i)
        for (size_t j = 0; j < C; ++j)
            grad(i,j) = 2 * (pred(i,j) - target(i,j)) / static_cast<T>(R * C);
    return grad;
}
