#pragma once

#include <utils/mathUtils.h>
#include <SFML/Graphics.hpp>

#include <console.h>

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
        sf::FloatRect intersection;
        if (!penetrator.intersects(receiver, intersection))
            return {};

        sf::Vector2f penetratorCentre(
            penetrator.left + penetrator.width / 2.f,
            penetrator.top + penetrator.height / 2.f
        );

        sf::Vector2f receiverCentre(
            receiver.left + receiver.width / 2.f,
            receiver.top + receiver.height / 2.f
        );

        sf::Vector2f targetsOffset = penetratorCentre - receiverCentre;

        if (intersection == penetrator || intersection == receiver)
        {
            // We check on what side of the object the penetrator is on
            if (targetsOffset.x < targetsOffset.y == -targetsOffset.x < targetsOffset.y)
            {
                // Top/Bottom
                float dir = m::sign(targetsOffset.y);
                float depth = std::abs(targetsOffset.y) + (penetrator.height / 2.f);
                return { {0, -depth * dir} };
            }
            else
            {
                // Left/Right
                float dir = m::sign(targetsOffset.x);
                float depth = std::abs(targetsOffset.x) + (penetrator.width / 2.f);
                return { {-depth * dir, 0} };
            }
        }
        
        return {};
    }
}