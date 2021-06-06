#include <actors/components/movement2D.h>
#include <utils/mathUtils.h>

#include <physics/collisions.h>
#include <world.h>

void wok::Movement2D::moveBy(sf::Transformable& transform, sf::Vector2f input, float delta)
{
    applyInputToVelocity(input, delta);
    moveWithReaction(transform, delta);
}

void wok::Movement2D::setOrientation(sf::Transformable& transform, sf::Vector2f target)
{
    bool shouldFaceRight = target.x > transform.getPosition().x;
    if (shouldFaceRight != isFacingRight)
    {
        flip(transform);
    }
}

void wok::Movement2D::applyPushback(sf::Vector2f force)
{
    velocity += force;
}

// =========== PRIVATE =========== 
void wok::Movement2D::applyInputToVelocity(sf::Vector2f input, float dt)
{
    sf::Vector2f desiredVelocity = input * settings->movementSpeed;

    if (m::length(desiredVelocity) < m::length(velocity))
    {
        // We check if the player tries to go against the velocity
        float aligment = 1.f - m::dot(input, m::normalize(velocity));
        if (m::length(input) < 0.2f)
        {
            aligment = 0.f;
        }

        float dec = m::lerp(300.f, 500.f, std::clamp(aligment, 0.f, 1.f));

        velocity -= m::normalize(velocity) * dec * dt;
    }
    else
    {
        velocity += m::normalize(desiredVelocity) * 500.f * dt;
    }
}

void wok::Movement2D::moveWithReaction(sf::Transformable& transform, float dt)
{
    transform.move(velocity * dt);

    if (sf::Keyboard::isKeyPressed(sf::Keyboard::Space))
        return;

    // Rection to world geometry
    sf::Vector2f accumulatedReaction;
    world::checkForCollisions(colliderGetter(), [&accumulatedReaction](collide::Reaction r)
        {
            auto p = -r.penetration;
            if (p.x != 0 && p.y != 0)
            {
                if (std::abs(p.x) < std::abs(p.y))
                    p.y = 0;
                else if (std::abs(p.x) > std::abs(p.y))
                    p.x = 0;
            }

            if (std::abs(p.x) > std::abs(accumulatedReaction.x))
                accumulatedReaction.x = p.x;
            if (std::abs(p.y) > std::abs(accumulatedReaction.y))
                accumulatedReaction.y = p.y;
        });

    if (accumulatedReaction.x > 0 != velocity.x > 0 && accumulatedReaction.x != 0)
        velocity.x = 0;


    if (accumulatedReaction.y > 0 != velocity.y > 0 && accumulatedReaction.y != 0)
        velocity.y = 0;

    transform.move(accumulatedReaction);
}

void wok::Movement2D::flip(sf::Transformable& transform)
{
    isFacingRight = !isFacingRight;
    float nextScale = isFacingRight ? 1.f : -1.f;

    if (flipTween)
        flipTween->kill();

    flipTween = std::make_shared<LerpTweener<float>>(owner,
        [&transform]() { return transform.getScale().x; },
        [&transform](float v) { transform.setScale(v, 1.f); },
        nextScale, settings->flipTime
        );

    world::addTween(flipTween);
}
