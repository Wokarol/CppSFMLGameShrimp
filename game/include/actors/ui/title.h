#pragma once

#include <actor.h>
#include <SFML/Graphics.hpp>

#include <resources.h>

namespace wok::ui
{
    class Title : public Actor, public wok::Drawable
    {
    public:
        Title(sf::Color fillColor, float positionFromTop, std::string fontName, std::string title, unsigned int characterSize) :
            fillColor(fillColor), positionFromTop(positionFromTop),
            font(res::get<sf::Font>(fontName)), title(title), characterSize(characterSize)
        {}

        virtual void draw(sf::RenderTarget& target, sf::RenderStates& states) override;
        virtual int getSortingOrder() override { return -100; }

    private:
        sf::Color fillColor;
        float positionFromTop;
        std::shared_ptr<sf::Font> font;
        std::string title;
        unsigned int characterSize;
    };
}