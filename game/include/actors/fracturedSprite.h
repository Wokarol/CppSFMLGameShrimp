#pragma once


#include <world.h>
#include <actor.h>

namespace wok
{
    class FracturedSprite : public Actor2D, public Drawable, public Tickable
    {
        struct Fracture
        {
            sf::Sprite sprite;
            sf::Vector2f velocity;
            float angularVelocity;
            float lifetime = 1.f;
            float lifetimeDropRate;

            Fracture(sf::Sprite sprite, sf::Vector2f direction, float rotation, float lifetimeDropRate) :
                sprite(sprite), velocity(direction), angularVelocity(rotation), lifetimeDropRate(lifetimeDropRate)
            {}
        };

    public:
        FracturedSprite(const sf::Sprite& original, std::shared_ptr<sf::Texture> texture, std::vector<sf::IntRect> rects, float direction);

        virtual void update(const GameClock& time) override;
        virtual void draw(sf::RenderTarget& target, sf::RenderStates& states) override;
        virtual float getSortingYPos() { return pos.y; }

        virtual sf::Vector2f getActorPosition() override { return pos; }

    private:
        const std::shared_ptr<sf::Texture> texture;
        std::vector<Fracture> fractures;
        sf::Vector2f pos;
    };
}
