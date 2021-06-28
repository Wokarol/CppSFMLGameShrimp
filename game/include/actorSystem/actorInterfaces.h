#pragma once

#include <SFML/Graphics.hpp>
#include <gameClock.h>
#include <functional>
#include <physics/hitboxes.h>

namespace wok
{
    class Drawable
    {
    public:
        virtual void draw(sf::RenderTarget& target, sf::RenderStates& states) = 0;
        virtual int getSortingOrder() { return 0; }
        virtual float getSortingYPos() { return 0; }
        virtual bool shouldDrawAlways() { return false; } // This overrites "Gizmo Only" mode
    };

    class Tickable
    {
    public:
        virtual void update(const GameClock& time) = 0;
    };

    class Hittable
    {
    public:
        struct HitData
        {
            const sf::Vector2f direction;
            const int damage;

            HitData(sf::Vector2f direction, int damage) : direction(direction), damage(damage) {}
        };

        virtual void reactToHit(HitData) {};
    };

    class Collideable
    {
    public:
        struct CollisionContext
        {

            enum class SourceType
            {
                Player, Enemy, Debug, Enviroment, NONE
            } sourceType;
            bool shouldHitTransparent = true;
            bool shouldHitDestructible = true;

            CollisionContext(SourceType sourceType)
                : sourceType(sourceType)
            {}

        };

        inline static const CollisionContext DebuggingContext = CollisionContext(CollisionContext::SourceType::Debug);

        virtual void getColliders(const CollisionContext&, const std::function<void(sf::FloatRect)>) { };
        virtual void getHitboxes(const CollisionContext& ctx, const std::function<void(const physics::Hitbox&)> yield)
        {
            getColliders(ctx, [&yield](auto rect)
                {
                    yield(physics::AABB(rect));
                });
        }
    };

    class Clickable
    {
    public:
        enum class MouseEventType
        {
            Pressed, Released, Moved
        };

        struct MouseEvent
        {
            const sf::Vector2f mousePosition;
            const MouseEventType eventType;
            bool consumed;

            MouseEvent(sf::Vector2f mousePosition, MouseEventType eventType) :
                mousePosition(mousePosition), eventType(eventType),
                consumed(false)
            {}
        };

        virtual void processMouseEvent(MouseEvent& ctx) = 0;
    };
}
