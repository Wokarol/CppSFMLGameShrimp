#pragma once


#include <actor.h>

namespace wok
{
    class StaticBox : public Actor, public sf::RectangleShape, public wok::Drawable, public Collideable
    {
    private:
        std::shared_ptr<sf::Texture> texture;

    public:
        StaticBox(sf::Vector2f pos, sf::Vector2f size) : RectangleShape(size)
        {
            setOrigin(size / 2.f);
            setPosition(pos);
            setFillColor(sf::Color(0));
            setOutlineColor(sf::Color::Cyan);
            setOutlineThickness(-1.f);
        }

        virtual void draw(sf::RenderTarget& target, sf::RenderStates& states) override
        {
            target.draw(*this, states);
        };

        virtual float getSortingYPos() override
        {
            return -10;
        }

        virtual bool shouldDrawAlways() { return true; }


        virtual void getColliders(const std::function<void(sf::FloatRect)> yield) override
        {
            yield(getGlobalBounds());
        }
    };
}