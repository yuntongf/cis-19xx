#pragma once

#include <cstddef>
#include <algorithm>
#include <array>
#include <initializer_list>
#include <iostream>

template <typename T, size_t Rows, size_t Cols>
class Tensor {
public:
    T data_[Rows][Cols];

    constexpr T& operator()(size_t i, size_t j) { 
        return data_[i][j]; 
    }

    constexpr const T& operator()(size_t i, size_t j) const { 
        return data_[i][j]; 
    }

    static constexpr size_t NumRows() { 
        return Rows; 
    }

    static constexpr size_t NumCols() { 
        return Cols; 
    }

    void Print(const std::string& name = "") const {
        if (!name.empty()) std::cout << name << " =\n";
        for (size_t i = 0; i < Rows; ++i) {
            for (size_t j = 0; j < Cols; ++j)
                std::cout << data_[i][j] << " ";
            std::cout << "\n";
        }
        std::cout << "\n";
    }
};
