#pragma once

#include <utils/mathUtils.h>
#include <SFML/Graphics.hpp>

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

    Intersection rayWithCircle(m::Ray ray, const sf::CircleShape& circle);

    /// <param name="AABB"> Rectangle's rotation and position will be ignored </param>
    Intersection rayWithCenteredAABB(m::Ray ray, const sf::RectangleShape& aabb);
    /// <param name="AABB"> Rectangle's rotation will be ignored </param>
    Intersection rayWithAABB(m::Ray ray, const sf::RectangleShape& aabb);
    Intersection rayWithOBB(m::Ray ray, const sf::RectangleShape& obb);
    Intersection rayWithAny(m::Ray ray, const sf::Shape& s);


    /// Expected FwdIt has to be deferencable into sf::Shape* or equivalent and has to support ++ operator
    template <typename FwdIt>
    inline Intersection raycastAllShapes(m::Ray ray, FwdIt start, FwdIt end)
    {
        Intersection closestIntersection;
        float smallestDistance = std::numeric_limits<float>::infinity();

        for (auto it = start; it != end; it++)
        {
            auto shape = *it;
            auto raycastResult = rayWithAny(ray, *shape);

            if (!raycastResult.hit)
                continue;

            if (raycastResult.distance > smallestDistance)
                continue;

            closestIntersection = raycastResult;
            smallestDistance = raycastResult.distance;
        }

        return closestIntersection;
    }
}
