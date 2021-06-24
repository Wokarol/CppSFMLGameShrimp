#pragma once

#include <actor.h>

namespace wok::ui
{
    class Background : public Actor, public wok::Drawable
    {
    public:
        Background(sf::Color fillColor) :
            fillColor(fillColor)
        {}

        virtual void draw(sf::RenderTarget& target, sf::RenderStates& states) override;
        virtual int getSortingOrder() override { return -500; }

    private:
        sf::Color fillColor;
    };
}