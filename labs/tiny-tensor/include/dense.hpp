#pragma once

#include "tensor.hpp"
#include "ops.hpp"

// dense (fully-connected) layer: z = xW + b
template <typename T, size_t In, size_t Out>
constexpr Tensor<T, 1, Out> Dense(const Tensor<T, 1, In>& input,
                                  const Tensor<T, In, Out>& weights,
                                  const Tensor<T, 1, Out>& bias) {
    auto z = input * weights;
    Tensor<T, 1, Out> out{};
    for (size_t j = 0; j < Out; ++j)
        out(0,j) = z(0,j) + bias(0,j);
    return out;
}