#pragma once

#include <utils/mathUtils.h>
#include <SFML/Graphics.hpp>
#include <physics\hitboxes.h>

namespace wok::intersect
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

    Intersection rayWithCircle(m::Ray ray, const physics::Circle& circle);

    /// <param name="AABB"> Rectangle's rotation and position will be ignored </param>
    Intersection rayWithCenteredAABB(m::Ray ray, const physics::AABB& aabb);
    /// <param name="AABB"> Rectangle's rotation will be ignored </param>
    Intersection rayWithAABB(m::Ray ray, const physics::AABB& aabb);
    Intersection rayWithOBB(m::Ray ray, const physics::OBB& obb);
    Intersection rayWithAny(m::Ray ray, const physics::Hitbox& s);
}
