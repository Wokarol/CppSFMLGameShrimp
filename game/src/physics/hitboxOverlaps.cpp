#include <physics/hitboxes.h>
#include <utils/mathUtils.h>
#include <console.h>

float sqr(float x)
{
    return x * x;
}

float overZero(float x)
{
    return std::max(x, 0.f);
}

bool circleWithAABB(const wok::physics::AABB& aabb, const wok::physics::Circle& circle)
{
    sf::Vector2f halfSize = aabb.size / 2.f;
    sf::Vector2f aabbCentre = aabb.topLeft + halfSize;

    // We measure distance of circle to the centre of AABB
    // std::abs collapses this situation to only one quadrant of the aabb
    float circleDistX = std::abs(circle.position.x - aabbCentre.x);
    float circleDistY = std::abs(circle.position.y - aabbCentre.y);

    // If we are far from the aabb, the circle won't overlap
    if (circleDistX > (halfSize.x + circle.radius))
        return false;
    if (circleDistY > (halfSize.y + circle.radius))
        return false;

    // If circles centre falls within aabb, we for sure overlap
    if (circleDistX < halfSize.x)
        return true;
    if (circleDistX < halfSize.x)
        return true;

    // Otherwise we check distance to the edge
    float distSqr = sqr(overZero(circleDistX - halfSize.x)) + sqr(overZero(circleDistY - halfSize.y));
    return (distSqr <= sqr(circle.radius));
}

// --------
// Circles
// --------
bool wok::physics::AABB::overlapsCircle(sf::Vector2f otherPosition, float otherRadius) const
{
    return circleWithAABB(*this, Circle(otherPosition, otherRadius));
}
bool wok::physics::OBB::overlapsCircle(sf::Vector2f, float) const
{
    console::log("Unsupported shape overlap <Circle> and <OBB>");
    return false;
}
bool wok::physics::Circle::overlapsCircle(sf::Vector2f otherPosition, float otherRadius) const
{
    return m::dist(otherPosition, position) < (radius + otherRadius);
}


// --------
// RECTS
// --------
bool wok::physics::AABB::overlapsRect(sf::FloatRect rect) const
{
    return sf::FloatRect(topLeft, size).intersects(rect);
}
bool wok::physics::OBB::overlapsRect(sf::FloatRect) const
{
    console::log("Unsupported shape overlap <AABB> and <OBB>");
    return false;
}
bool wok::physics::Circle::overlapsRect(sf::FloatRect rect) const
{
    return circleWithAABB(AABB(rect), *this);
}