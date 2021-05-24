#pragma once

#include <utils/mathUtils.h>
#include <SFML/Graphics.hpp>

namespace wok::collide
{
    struct Reaction
    {
        bool hit;
        sf::Vector2f penetration;

        Reaction() :
            hit(false),
            penetration(sf::Vector2f(0,0))
        {}

        Reaction(sf::Vector2f penetration) :
            hit(true),
            penetration(penetration)
        {}
    };

    inline Reaction AABBWithAABB(sf::FloatRect penetrator, sf::FloatRect receiver)
    {
        if (penetrator.intersects(receiver))
        {
            return { sf::Vector2f(10, 10) };
        }
        
        return {};
    }
}