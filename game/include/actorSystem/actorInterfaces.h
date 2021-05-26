#pragma once

#include <SFML/Graphics.hpp>
#include <gameClock.h>
#include <functional>
#include <physics/hitboxes.h>

namespace wok
{
    class Drawable
    {
    public:
        virtual void draw(sf::RenderTarget& target, sf::RenderStates& states) = 0;
        virtual int getSortingOrder() { return 0; }
        virtual float getSortingYPos() { return 0; }
        virtual bool shouldDrawAlways() { return false; } // This overrites "Gizmo Only" mode
    };

    class Tickable
    {
    public:
        virtual void update(const GameClock& time) = 0;
    };

    class Hittable
    {
    public:
        struct HitData
        {
            const sf::Vector2f direction;
            const float damage;

            HitData(sf::Vector2f direction, float damage) : direction(direction), damage(damage) {}
        };

        virtual void reactToHit([[maybe_unused]] HitData) {};
    };

    class Collideable
    {
    public:
        virtual void getColliders(const std::function<void(sf::FloatRect)> yield) { };
        virtual void getHitboxes(const std::function<void(const physics::Hitbox&)> yield)
        {
            getColliders([&yield](auto rect)
                {
                    yield(AABB(rect));
                });
        }
    };
}
