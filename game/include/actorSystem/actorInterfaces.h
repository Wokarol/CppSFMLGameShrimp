#pragma once

#include <SFML/Graphics.hpp>
#include <gameClock.h>
#include <physics/intersections.h>

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
        virtual intersect::Intersection getClosestHit(const m::Ray& ray) = 0;
        virtual void reactToHit([[maybe_unused]] const intersect::Intersection& intersection, [[maybe_unused]] int damage) {};
    };
}
