#pragma once

#include <SFML/Graphics.hpp>
#include <actors/components/settings/movementSettings.h>
#include <functional>
#include <actor.h>
#include <tweeners.h>

namespace wok
{
    class Movement2D
    {
    public:
        Movement2D() : settings(nullptr) {}
        Movement2D(ActorHandle<Actor> owner, MovementSettings& settings, std::function<sf::FloatRect()> colliderGetter)
            : owner(owner), settings(&settings), colliderGetter(colliderGetter)
        {}

        void moveBy(sf::Transformable& transform, sf::Vector2f input, float delta);
        void setOrientation(sf::Transformable& transform, sf::Vector2f target);
        void applyPushback(sf::Vector2f force);

    private:
        void applyInputToVelocity(sf::Vector2f input, float dt);
        void moveWithReaction(sf::Transformable& transform, float dt);
        void flip(sf::Transformable& transform);

    private:
        ActorHandle<Actor> owner;
        MovementSettings* settings;

        bool isFacingRight = true;
        std::shared_ptr<LerpTweener<float>> flipTween;

        sf::Vector2f velocity;
        std::function<sf::FloatRect()> colliderGetter; // TODO: Maybe change that into "collider" parameter
    };
}