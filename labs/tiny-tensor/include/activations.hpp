#pragma once

#include "tensor.hpp"
#include <cmath>
#include <algorithm>

template <typename T, size_t R, size_t C>
constexpr Tensor<T, R, C> Relu(const Tensor<T, R, C>& x) {
    Tensor<T, R, C> out{};
    for (size_t i = 0; i < R; ++i)
        for (size_t j = 0; j < C; ++j)
            out(i,j) = std::max<T>(0, x(i,j));
    return out;
}

template <typename T, size_t R, size_t C>
constexpr Tensor<T, R, C> Sigmoid(const Tensor<T, R, C>& x) {
    Tensor<T, R, C> out{};
    for (size_t i = 0; i < R; ++i)
        for (size_t j = 0; j < C; ++j)
            out(i,j) = 1 / (1 + std::exp(-x(i,j)));
    return out;
}
