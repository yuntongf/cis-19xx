#include "mixer.hpp"


Mixer::Mixer(ThreadPool& pool, int numTracks) : pool_(pool) {
    // A major chord: A3, C#4, E4, A4
    float freqs[] = {220.00f, 277.18f, 329.63f, 440.00f};
    for (int i = 0; i < numTracks; ++i) {
        tracks_.emplace_back(freqs[i % 4]);
    }
}

void Mixer::InitDemo() {
    for (int i = 0; i < (int)tracks_.size(); ++i) {
        tracks_[i].AddEffect(std::make_unique<Gain>(0.7f - 0.1f * i));
        tracks_[i].AddEffect(std::make_unique<Delay>(1500 + 800 * i));
        tracks_[i].GenerateTone();
    }
}

sf::SoundBuffer Mixer::MixToSFML() {
    const size_t N = tracks_[0].getBuffer().samples.size();
    std::vector<float> mix(N, 0.0f);

    std::vector<std::future<void>> futs;
    for (auto& t : tracks_) {
        futs.push_back(pool_.Submit([&t]{ t.Process(); }));
    }
    for (auto& f : futs) {
        f.get();
    }

    for (auto& t : tracks_) {
        const auto& s = t.getBuffer().samples;
        for (size_t i = 0; i < N; ++i)
            mix[i] += s[i] / tracks_.size();
    }

    // convert [-1,1] to int16_t (why?)
    std::vector<sf::Int16> final(N);
    for (size_t i = 0; i < N; ++i) {
        final[i] = static_cast<sf::Int16>(mix[i] * 30000);
    }

    sf::SoundBuffer buf;
    buf.loadFromSamples(final.data(), N, 1, AudioBuffer::BUFF_SIZE);
    return buf;
}
