#pragma once

#include <SFML/System/Vector2.hpp>
#include <utils/mathUtils.h>
#include <physics/intersection.h>

namespace wok::physics
{
    struct Hitbox
    {
        virtual Intersection raycast(m::Ray ray) const = 0;
        virtual ~Hitbox() = default;
    };

    struct AABB : public Hitbox
    {
        sf::Vector2f position;
        sf::Vector2f size;

        AABB(sf::Vector2f position, sf::Vector2f size) :
            size(size), position(position)
        {}

        AABB(sf::FloatRect r) :
            size(r.width, r.height), position(r.left, r.top)
        {}

        virtual Intersection raycast(m::Ray ray) const override;
    };

    struct OBB : public Hitbox
    {
        sf::Vector2f position;
        sf::Vector2f size;
        float rotation;

        OBB(sf::Vector2f position, sf::Vector2f size, float rotation) :
            size(size), position(position), rotation(rotation)
        {}

        virtual Intersection raycast(m::Ray ray) const override;
    };

    struct Circle : public Hitbox
    {
        sf::Vector2f position;
        float radius;

        Circle(sf::Vector2f position, float radius) :
            radius(radius), position(position)
        {}

        virtual Intersection raycast(m::Ray ray) const override;
    };
}
