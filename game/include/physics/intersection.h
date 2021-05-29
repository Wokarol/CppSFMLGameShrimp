#pragma once

#include <SFML/Graphics.hpp>
#include <utils/mathUtils.h>

namespace wok::physics
{
    struct Intersection
    {
        bool hit;
        float distance;
        sf::Vector2f normal;
        m::Ray ray;

        Intersection() :
            hit(false),
            distance(0), normal(sf::Vector2f(0.f, 0.f)), ray()
        {}

        Intersection(float d, sf::Vector2f n, m::Ray ray) :
            hit(true),
            distance(d), normal(n), ray(ray)
        {}
    };
}