#pragma once

#include <deque>

#include "buffer.hpp"

class Effect {
public:
    virtual void Process(AudioBuffer& buffer) = 0;
    virtual ~Effect() = default;
};

class Delay : public Effect {
public:
    Delay(size_t delaySamples = 4000) : delayLine_(delaySamples, 0.0f) {}

    void Process(AudioBuffer& b) override {
        for (auto& s : b.samples) {
            float out = s + 0.4f * delayLine_.front();
            delayLine_.pop_front();
            delayLine_.push_back(out);
            s = out;
        }
    }
private:
    std::deque<float> delayLine_;
};

class Gain : public Effect {
    float gain_;
public:
    Gain(float g) : gain_(g) {}
    
    void Process(AudioBuffer& b) override {
        for (auto& s : b.samples) {
            s *= gain_;
        }
    }
};