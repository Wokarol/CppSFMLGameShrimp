#pragma once

#include <string>
#include <SFML/Graphics.hpp>

namespace wok
{
    struct IconBarSettings
    {
        std::vector<sf::Vector2i> animationStrip;
        sf::Vector2i iconSize;
        std::string iconPath;
        sf::Vector2f offset;
        float spacing = 0;
        float animationFramerate = 0;
    };
}