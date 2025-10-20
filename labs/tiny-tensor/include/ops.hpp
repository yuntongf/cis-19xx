#pragma once

#include <cmath>

#include "tensor.hpp"

template <typename T, size_t R, size_t C>
constexpr Tensor<T, R, C> operator+(const Tensor<T, R, C>& a, const Tensor<T, R, C>& b) {
    Tensor<T, R, C> out{};
    for (size_t i = 0; i < R; ++i)
        for (size_t j = 0; j < C; ++j)
            out(i,j) = a(i,j) + b(i,j);
    return out;
}

template <typename T, size_t R, size_t K, size_t C>
constexpr Tensor<T, R, C> operator*(const Tensor<T, R, K>& a, const Tensor<T, K, C>& b) {
    Tensor<T, R, C> out{};
    for (size_t i = 0; i < R; ++i)
        for (size_t j = 0; j < C; ++j)
            for (size_t k = 0; k < K; ++k)
                out(i,j) += a(i,k) * b(k,j);
    return out;
}

// Specialization: 4x4 float (loop unrolled)
// [TODO] Implement me
template <>
constexpr Tensor<float, 4, 4> operator*(const Tensor<float, 4, 4>& a,
                                        const Tensor<float, 4, 4>& b) {
    Tensor<float, 4, 4> out{};
    for (int i = 0; i < 4; ++i)
        for (int j = 0; j < 4; ++j)
            out(i,j) = a(i,0)*b(0,j) + a(i,1)*b(1,j) +
                       a(i,2)*b(2,j) + a(i,3)*b(3,j);
    return out;
}
