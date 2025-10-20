#pragma once

#include <vector>
#include <memory>
#include <cmath>

#include "effect.hpp"
#include "buffer.hpp"

class Track {
public:
    explicit Track(float freq = 440.0f);

    void AddEffect(std::unique_ptr<Effect> e);

    void GenerateTone(float sampleRate = 44100.f);

    void Process();

    const AudioBuffer& getBuffer() const { return buffer_; }

private:
    std::vector<std::unique_ptr<Effect>> effects_;
    AudioBuffer buffer_;
    float frequency_;
};