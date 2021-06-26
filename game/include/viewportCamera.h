#pragma once

#include <SFML/Graphics.hpp>

namespace wok
{
    class ViewportCamera
    {
    public:
        virtual sf::View getView(sf::Vector2f screenSize) const = 0;

        sf::Vector2f viewportToWorld(sf::Vector2f pos) const
        {
            sf::Vector2f centre = lastView.getCenter();
            sf::Vector2f size = lastView.getSize();

            // We bring it from 0..1 to -0.5..0.5 for easier calculation
            pos -= sf::Vector2f(0.5f, 0.5f);

            return centre + m::scale(pos, size);
        }

    protected:
        mutable sf::View lastView;
    };

    class DefaultViewportCamera : public ViewportCamera
    {
    public:
        virtual sf::View getView(sf::Vector2f screenSize) const override
        {
            lastView = sf::View({ 0.f, 0.f, screenSize.x, screenSize.y });
            return lastView;
        }
    };
}
