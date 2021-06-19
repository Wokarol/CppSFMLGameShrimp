#include <physics/hitboxes.h>
#include <utils/mathUtils.h>

namespace wok
{
    auto physics::AABB::raycast(m::Ray ray) const -> Intersection
    {
        ray.move(-topLeft);
        ray.direction = m::normalize(ray.direction);

        //console::log(ray.origin.x);

        if (ray.direction.x > 0 && ray.origin.x < 0)
        {
            // Ray from left onto left wall
            float distanceToWallOnXAxis = -ray.origin.x;
            float distanceToWall = distanceToWallOnXAxis / ray.direction.x;
            float yOnWall = ray.getPoint(distanceToWall).y;

            if (yOnWall >= 0 && yOnWall <= size.y)
            {
                return physics::Intersection(distanceToWall, sf::Vector2f(-1.f, 0.f), ray);
            }
        }

        if (ray.direction.x < 0 && ray.origin.x > size.x)
        {
            // Ray from right onto right wall
            float distanceToWallOnXAxis = ray.origin.x - size.x;
            float distanceToWall = -distanceToWallOnXAxis / ray.direction.x;
            float yOnWall = ray.getPoint(distanceToWall).y;

            if (yOnWall >= 0 && yOnWall <= size.y)
            {
                return physics::Intersection(distanceToWall, sf::Vector2f(1.f, 0.f), ray);
            }
        }

        if (ray.direction.y > 0 && ray.origin.y < 0)
        {
            // Ray from up onto upper wall
            float distanceOnWallOnYAxis = -ray.origin.y;
            float distanceToWall = distanceOnWallOnYAxis / ray.direction.y;
            float xOnWall = ray.getPoint(distanceToWall).x;

            if (xOnWall >= 0 && xOnWall <= size.x)
            {
                return physics::Intersection(distanceToWall, sf::Vector2f(0.f, -1.f), ray);
            }
        }

        if (ray.direction.y < 0 && ray.origin.y > size.y)
        {
            // Ray from down onto bottom wall
            float distanceOnWallOnYAxis = ray.origin.y - size.y;
            float distanceToWall = -distanceOnWallOnYAxis / ray.direction.y;
            float xOnWall = ray.getPoint(distanceToWall).x;

            if (xOnWall >= 0 && xOnWall <= size.x)
            {
                return physics::Intersection(distanceToWall, sf::Vector2f(0.f, 1.f), ray);
            }
        }

        return {};
    }

    auto physics::OBB::raycast(m::Ray ray) const -> Intersection
    {
        ray.rotateAround(position, rotation);
        auto result = physics::AABB(position, size).centered().raycast(ray);
        result.normal = m::rotate(result.normal, -rotation);
        return result;
    }

    auto physics::Circle::raycast(m::Ray ray) const -> Intersection
    {
        // We offset it so the centre of the circle is in the middle
        ray.move(-position);
        ray.direction = m::normalize(ray.direction);

        // Algorithm is based on this article:
        // https://www.scratchapixel.com/lessons/3d-basic-rendering/minimal-ray-tracer-rendering-simple-shapes/ray-sphere-intersection
        sf::Vector2f L = -ray.origin;
        float r = radius;

        if (m::sqrLength(L) < (r * r))
        {
            // We are inside the circle
            return {};
        }

        float tca = m::dot(L, ray.direction);
        if (tca < 0)
        {
            // We point away from the circle
            return {};
        }

        float d = std::sqrt(m::dot(L, L) - (tca * tca));
        if (d > r)
        {
            // We do not hit the circle
            return {};
        }

        float thc = std::sqrt(r * r - d * d);
        float distToHit = tca - thc;

        sf::Vector2f hitPoint = ray.getPoint(distToHit);
        sf::Vector2f normal = m::normalize(hitPoint);

        return Intersection(distToHit, normal, ray);
    }
}
