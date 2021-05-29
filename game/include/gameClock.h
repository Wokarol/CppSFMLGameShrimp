#pragma once

#include <SFML/System/Clock.hpp>
#include <input.h>

namespace wok
{
    class GameClock
    {
    public:
        float delta = 0.f;
        float absolute = 0.f;
        uint32_t frames = 0;

        void Tick()
        {
            delta = clock.restart().asSeconds();

            const float maxDelta = 0.1f;
            if (delta > maxDelta)
                delta = maxDelta;

            if (input::slowMode)
                delta *= 0.25f;

            absolute += delta;
            frames++;
        }

    private:
        sf::Clock clock;
    };
}
