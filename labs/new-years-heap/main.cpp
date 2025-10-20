#include "allocator.hpp"

#include <SFML/Graphics.hpp>
#include <vector>
#include <unordered_map>
#include <memory>
#include <cmath>
#include <cstdlib>
#include <iostream>

struct Particle {
    sf::Vector2f pos, vel;
    float life;
    sf::Color color;

    Particle(float x, float y, float vx, float vy, sf::Color c)
        : pos(x, y), vel(vx, vy), life(1.5f), color(c) {}

    void update(float dt) {
        vel.y += 50 * dt; // gravity
        pos += vel * dt;
        life -= dt;
        color.a = static_cast<sf::Uint8>(std::max(0.f, life) * 255);
    }

    bool alive() const { return life > 0; }

    void draw(sf::RenderWindow& win) const {
        sf::CircleShape s(2);
        s.setPosition(pos);
        s.setFillColor(color);
        win.draw(s);
    }
};

struct Firework {
    enum class State { Launching, Exploding };
    sf::Vector2f pos, vel;
    State state = State::Launching;
    std::vector<Particle*> particles;
    float timer = 0.f;
    bool alive = true;

    Firework(float x, float y)
        : pos(x, y), vel(0, -300 - rand() % 100) {}

    void update(float dt, SlabAllocator& alloc) {
        if (state == State::Launching) {
            pos += vel * dt;
            vel.y += 150 * dt;
            timer += dt;
            if (vel.y > 0 || timer > 2.5f) {
                explode(alloc);
                state = State::Exploding;
            }
        } else {
            for (auto it = particles.begin(); it != particles.end();) {
                (*it)->update(dt);
                if (!(*it)->alive()) {
                    alloc.Deallocate(*it);
                    it = particles.erase(it);
                } else ++it;
            }
            if (particles.empty()) alive = false;
        }
    }

    void explode(SlabAllocator& alloc) {
        sf::Color base(rand() % 255, rand() % 255, rand() % 255);
        for (int i = 0; i < 100; ++i) {
            float angle = (rand() % 360) * 3.14159f / 180.f;
            float speed = 100 + rand() % 300;
            float vx = std::cos(angle) * speed;
            float vy = std::sin(angle) * speed;
            sf::Color c = base;
            c.r = std::min(255, c.r + rand() % 50);
            c.g = std::min(255, c.g + rand() % 50);
            c.b = std::min(255, c.b + rand() % 50);
            particles.push_back(alloc.Allocate<Particle>(pos.x, pos.y, vx, vy, c));
        }
    }

    void draw(sf::RenderWindow& win) const {
        if (state == State::Launching) {
            sf::CircleShape s(3);
            s.setPosition(pos);
            s.setFillColor(sf::Color::White);
            win.draw(s);
        } else {
            for (auto* p : particles)
                p->draw(win);
        }
    }
};

int main() {
    sf::RenderWindow window(sf::VideoMode(900, 700), "New Year's Heap");
    window.setFramerateLimit(60);

    SlabAllocator allocator;
    std::vector<Firework*> fireworks;

    sf::Clock clock;
    float launchTimer = 0.f;

    while (window.isOpen()) {
        sf::Event e;
        while (window.pollEvent(e)) {
            if (e.type == sf::Event::Closed)
                window.close();
            if (e.type == sf::Event::MouseButtonPressed) {
                fireworks.push_back(allocator.Allocate<Firework>(e.mouseButton.x, 650.f));
            }
        }

        float dt = clock.restart().asSeconds();
        launchTimer += dt;
        if (launchTimer > 1.2f) {
            launchTimer = 0;
            fireworks.push_back(allocator.Allocate<Firework>(rand() % 900, 650.f));
        }

        for (auto it = fireworks.begin(); it != fireworks.end();) {
            Firework* f = *it;
            f->update(dt, allocator);
            if (!f->alive) {
                allocator.Deallocate(f);
                it = fireworks.erase(it);
            } else ++it;
        }

        window.clear(sf::Color(5, 5, 15));
        for (auto* f : fireworks) {
            f->draw(window);
        }
        window.display();
    }
}
