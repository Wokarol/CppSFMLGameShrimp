#pragma once

#include <string>
#include <SFML/Graphics.hpp>

namespace wok
{
    struct IconBarSettings
    {
        std::vector<sf::Vector2i> animationStrip;
        sf::Vector2f iconSize;
        std::string iconPath;
        sf::Vector2i offset;
        float spacing;
        float animationFramerate;
    };
}