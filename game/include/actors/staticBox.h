#pragma once


#include <actor.h>

namespace wok
{
    class StaticBox : public Actor2D, public sf::RectangleShape, public wok::Drawable, public Collideable
    {
    private:
        std::shared_ptr<sf::Texture> texture;

    public:
        StaticBox(sf::Vector2f pos, sf::Vector2f size) : RectangleShape(size)
        {
            setOrigin(size / 2.f);
            setPosition(pos);
            setFillColor(sf::Color(255, 0, 0, 120));
        }

        virtual void draw(sf::RenderTarget& target, sf::RenderStates& states) override
        {
            target.draw(*this, states);
        };

        virtual float getSortingYPos() override
        {
            return -10;
        }

        virtual sf::Vector2f getActorPosition() override { return getPosition(); }

        virtual void getColliders(const std::function<void(sf::FloatRect)> yield) override
        {
            yield(getGlobalBounds());
        }
    };
}