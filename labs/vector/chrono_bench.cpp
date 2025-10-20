#include "vec.hpp"
#include <chrono>
#include <iostream>
#include <vector>
#include <cstring>

constexpr int SIZE = 1024;

struct Movable {
    std::unique_ptr<int[]> data;
    Movable() : data(std::make_unique<int[]>(SIZE)) {}
    Movable(const Movable& other) : data(std::make_unique<int[]>(SIZE)) {
        std::copy(other.data.get(), other.data.get() + SIZE, data.get());
    }
    Movable(Movable&& other) noexcept = default;
};

template <typename VecType>
void BenchmarkPushBack(const std::string& label, int n) {
    auto start = std::chrono::high_resolution_clock::now();
    for (int trial = 0; trial < 20; ++trial) {
        VecType v(0);
        v.Reserve(n);
        for (int i = 0; i < n; ++i) {
            v.PushBack({});
        }
        asm volatile("" :: "g"(&v) : "memory");
    }
    auto end = std::chrono::high_resolution_clock::now();
    std::chrono::duration<double> elapsed = end - start;
    std::cout << label << " -- Time: " << elapsed.count() / 20.0 << "s (avg)\n";
}

int main() {
    constexpr int N = 200000;

    BenchmarkPushBack<Vec<Movable>>("push_back movable", N);
}
