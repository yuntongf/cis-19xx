#include "track.hpp"

Track::Track(float freq) : buffer_(44100 * 3), frequency_(freq) {}

void Track::AddEffect(std::unique_ptr<Effect> e) {
    effects_.push_back(std::move(e));
}

void Track::GenerateTone(float sampleRate) {
    for (size_t i = 0; i < buffer_.samples.size(); ++i) {
        buffer_.samples[i] = std::sin(2 * M_PI * frequency_ * i / sampleRate);
    }
}

void Track::Process() {
    for (auto& e : effects_)
        e->Process(buffer_);
}