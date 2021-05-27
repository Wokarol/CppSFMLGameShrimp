#pragma once

#include <SFML/System/Vector2.hpp>

namespace wok::physics
{
    struct Hitbox
    {
        virtual void dummyPolymorphicMethod() {};
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
    };

    struct OBB : public Hitbox
    {
        sf::Vector2f position;
        sf::Vector2f size;
        float rotation;

        OBB(sf::Vector2f position, sf::Vector2f size, float rotation) :
            size(size), position(position), rotation(rotation)
        {}
    };

    struct Circle : public Hitbox
    {
        sf::Vector2f position;
        float radius;

        Circle(sf::Vector2f position, float radius) :
            radius(radius), position(position)
        {}
    };
}
