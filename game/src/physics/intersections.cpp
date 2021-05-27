#include <physics/intersections.h>

auto wok::intersect::rayWithCircle(m::Ray ray, const physics::Circle& circle) -> Intersection
{
    // We offset it so the centre of the circle is in the middle
    ray.move(-circle.position);
    ray.direction = m::normalize(ray.direction);

    // Algorithm is based on this article:
    // https://www.scratchapixel.com/lessons/3d-basic-rendering/minimal-ray-tracer-rendering-simple-shapes/ray-sphere-intersection
    sf::Vector2f L = -ray.origin;
    float r = circle.radius;

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

auto wok::intersect::rayWithCenteredAABB(m::Ray ray, const physics::AABB& aabb) -> Intersection
{
    ray.direction = m::normalize(ray.direction);

    //console::log(ray.origin.x);

    if (ray.direction.x > 0 && ray.origin.x < 0)
    {
        // Ray from left onto left wall
        float distanceToWallOnXAxis = -ray.origin.x;
        float distanceToWall = distanceToWallOnXAxis / ray.direction.x;
        float yOnWall = ray.getPoint(distanceToWall).y;

        if (yOnWall >= 0 && yOnWall <= aabb.size.y)
        {
            return Intersection(distanceToWall, sf::Vector2f(-1.f, 0.f), ray);
        }
    }

    if (ray.direction.x < 0 && ray.origin.x > aabb.size.x)
    {
        // Ray from right onto right wall
        float distanceToWallOnXAxis = ray.origin.x - aabb.size.x;
        float distanceToWall = -distanceToWallOnXAxis / ray.direction.x;
        float yOnWall = ray.getPoint(distanceToWall).y;

        if (yOnWall >= 0 && yOnWall <= aabb.size.y)
        {
            return Intersection(distanceToWall, sf::Vector2f(1.f, 0.f), ray);
        }
    }

    if (ray.direction.y > 0 && ray.origin.y < 0)
    {
        // Ray from up onto upper wall
        float distanceOnWallOnYAxis = -ray.origin.y;
        float distanceToWall = distanceOnWallOnYAxis / ray.direction.y;
        float xOnWall = ray.getPoint(distanceToWall).x;

        if (xOnWall >= 0 && xOnWall <= aabb.size.x)
        {
            return Intersection(distanceToWall, sf::Vector2f(0.f, -1.f), ray);
        }
    }

    if (ray.direction.y < 0 && ray.origin.y > aabb.size.y)
    {
        // Ray from down onto bottom wall
        float distanceOnWallOnYAxis = ray.origin.y - aabb.size.y;
        float distanceToWall = -distanceOnWallOnYAxis / ray.direction.y;
        float xOnWall = ray.getPoint(distanceToWall).x;

        if (xOnWall >= 0 && xOnWall <= aabb.size.x)
        {
            return Intersection(distanceToWall, sf::Vector2f(0.f, 1.f), ray);
        }
    }

    return {};
}

/// <param name="AABB"> Rectangle's rotation will be ignored </param>
auto wok::intersect::rayWithAABB(m::Ray ray, const physics::AABB& aabb) -> Intersection
{
    ray.move(-aabb.position);
    return rayWithCenteredAABB(ray, aabb);
}

auto wok::intersect::rayWithOBB(m::Ray ray, const physics::OBB& obb) -> Intersection
{
    // We convert this scenario into Ray and AABB to simplify calculations
    ray.move(-obb.position);

    auto angle = -obb.rotation;
    ray.rotateAround(sf::Vector2f(0, 0), angle);

    auto res = rayWithCenteredAABB(ray, physics::AABB(obb.position, obb.size));
    res.normal = m::rotate(res.normal, -angle);

    return res;
}

auto wok::intersect::rayWithAny(m::Ray ray, const physics::Hitbox& hitbox) -> Intersection
{
    if (auto circle = dynamic_cast<const physics::Circle*>(&hitbox))
    {
        return rayWithCircle(ray, *circle);
    }

    if (auto obb = dynamic_cast<const physics::OBB*>(&hitbox))
    {
        return rayWithOBB(ray, *obb);
    }

    if (auto aabb = dynamic_cast<const physics::AABB*>(&hitbox))
    {
        return rayWithAABB(ray, *aabb);
    }

    // Our shape is unsupported
    return {};
}
