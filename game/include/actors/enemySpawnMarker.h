#pragma once

#include <world.h>
#include <actor.h>
#include <SFML/Graphics.hpp>

#include <actors/basicEnemy.h>

namespace wok
{
    class EnemySpawnMarker : public Actor, public Tickable, public wok::Drawable
    {
    public:
        EnemySpawnMarker(float spawnDuration, sf::Vector2f position, std::function<void()> spawnCallback);

        virtual void update(const GameClock& time) override;
        virtual void draw(sf::RenderTarget& target, sf::RenderStates& states) override;

    private:
        float timeLeftUntilSpawn = 0;
        float spawnDuration = 0;
        std::function<void()> spawnCallback;
        sf::Vector2f position;
    };
}
