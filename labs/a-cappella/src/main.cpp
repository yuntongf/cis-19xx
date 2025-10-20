#include <iostream>

#include "effect.hpp"
#include "track.hpp"
#include "threadpool.hpp"
#include "mixer.hpp"

int main() {
    ThreadPool pool(4);
    Mixer engine(pool, 4);
    engine.InitDemo();

    sf::SoundBuffer buffer = engine.MixToSFML();

    sf::Sound sound(buffer);
    sound.play();

    std::cout << "Playing an A major chord in four voices...\n";
    while (sound.getStatus() == sf::Sound::Playing) {
        sf::sleep(sf::milliseconds(100));
    }
}
