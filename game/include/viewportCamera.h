#pragma once

#include <SFML/Graphics.hpp>

namespace wok
{
    class ViewportCamera
    {
    public:
        virtual sf::View getView(sf::Vector2f screenSize) const = 0;
    };

    class DefaultViewportCamera : public ViewportCamera
    {
    public:
        virtual sf::View getView(sf::Vector2f screenSize) const override
        {
            return sf::View({ 0.f, 0.f, screenSize.x, screenSize.y });
        }
    };
}
