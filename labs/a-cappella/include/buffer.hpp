#pragma once

#include <vector>

struct AudioBuffer {
    static constexpr size_t BUFF_SIZE = 44100;
    std::vector<float> samples;
    explicit AudioBuffer(size_t n = BUFF_SIZE): samples(n, 0.0f) {}
};