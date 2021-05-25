#include <physics\intersections.h>

auto wok::intersect::rayWithCircle(m::Ray ray, const sf::CircleShape& circle) -> Intersection
{

    // We offset it so the centre of the circle is in the middle
    sf::Vector2f optimalPivot = sf::Vector2f(1.f, 1.f) * circle.getRadius();
    sf::Vector2f pivotOffset = circle.getOrigin() - optimalPivot;

    ray.move(-circle.getPosition() + pivotOffset);
    ray.direction = m::normalize(ray.direction);

    // Algorithm is based on this article:
    // https://www.scratchapixel.com/lessons/3d-basic-rendering/minimal-ray-tracer-rendering-simple-shapes/ray-sphere-intersection
    sf::Vector2f L = -ray.origin;
    float r = circle.getRadius();

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

/// <param name="AABB"> Rectangle's rotation and position will be ignored </param>
auto wok::intersect::rayWithCenteredAABB(m::Ray ray, const sf::RectangleShape& aabb) -> Intersection
{
    // We offset it so the aabb's top left corner is at [0, 0]
    ray.move(aabb.getOrigin());
    ray.direction = m::normalize(ray.direction);
    auto size = aabb.getSize();

    //console::log(ray.origin.x);

    if (ray.direction.x > 0 && ray.origin.x < 0)
    {
        // Ray from left onto left wall
        float distanceToWallOnXAxis = -ray.origin.x;
        float distanceToWall = distanceToWallOnXAxis / ray.direction.x;
        float yOnWall = ray.getPoint(distanceToWall).y;

        if (yOnWall >= 0 && yOnWall <= size.y)
        {
            return Intersection(distanceToWall, sf::Vector2f(-1.f, 0.f), ray);
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
            return Intersection(distanceToWall, sf::Vector2f(1.f, 0.f), ray);
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
            return Intersection(distanceToWall, sf::Vector2f(0.f, -1.f), ray);
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
            return Intersection(distanceToWall, sf::Vector2f(0.f, 1.f), ray);
        }
    }

    return {};
}

/// <param name="AABB"> Rectangle's rotation will be ignored </param>
auto wok::intersect::rayWithAABB(m::Ray ray, const sf::RectangleShape& aabb) -> Intersection
{
    ray.move(-aabb.getPosition());
    return rayWithCenteredAABB(ray, aabb);
}

auto wok::intersect::rayWithOBB(m::Ray ray, const sf::RectangleShape& obb) -> Intersection
{
    // We convert this scenario into Ray and AABB to simplify calculations
    ray.move(-obb.getPosition());

    auto angle = -obb.getRotation();
    ray.rotateAround(sf::Vector2f(0, 0), angle);

    auto res = rayWithCenteredAABB(ray, obb);
    res.normal = m::rotate(res.normal, -angle);

    return res;
}

auto wok::intersect::rayWithAny(m::Ray ray, const sf::Shape& s) -> Intersection
{
    if (auto circle = dynamic_cast<const sf::CircleShape*>(&s))
    {
        return rayWithCircle(ray, *circle);
    }

    if (auto obb = dynamic_cast<const sf::RectangleShape*>(&s))
    {
        return rayWithOBB(ray, *obb);
    }

    // Our shape is unsupported
    return {};
}
