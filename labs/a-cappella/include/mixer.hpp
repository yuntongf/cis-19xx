#pragma once

#include <SFML/Audio.hpp>

#include "track.hpp"
#include "threadpool.hpp"

class Mixer {
public:
    Mixer(ThreadPool& pool, int numTracks);

    void InitDemo();

    sf::SoundBuffer MixToSFML();

private:
    ThreadPool& pool_;
    std::vector<Track> tracks_;
};
