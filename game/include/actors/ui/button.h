#pragma once

#include <actor.h>

namespace wok::ui
{
    class Button : public Actor, public wok::Drawable
    {
    public:
        Button(std::string content, std::shared_ptr<sf::Font> font, unsigned int fontSize,
            sf::Vector2f offsetFromAnchor, sf::Vector2f anchor,
            sf::Vector2f size, sf::Color backgroundColor, sf::Color textColor
        ) :
            content(content), font(font), fontSize(fontSize),
            offsetFromAnchor(offsetFromAnchor), anchor(anchor),
            size(size), backgroundColor(backgroundColor), textColor(textColor)
        {}

        virtual void draw(sf::RenderTarget& target, sf::RenderStates& states) override;

    private:
        std::string content;
        std::shared_ptr<sf::Font> font;
        unsigned int fontSize;
        sf::Vector2f offsetFromAnchor;
        sf::Vector2f anchor;
        sf::Vector2f size;
        sf::Color backgroundColor;
        sf::Color textColor;
    };
}
