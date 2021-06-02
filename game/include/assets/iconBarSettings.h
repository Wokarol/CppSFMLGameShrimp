#pragma once

#include <string>
#include <SFML/Graphics.hpp>

namespace wok
{
    struct IconBarSettings
    {
        sf::Vector2i spriteRectFull;
        sf::Vector2i spriteRectEmpty;
        std::vector<sf::Vector2i> animationStripGained;
        std::vector<sf::Vector2i> animationStripLost;
        sf::Vector2i iconSize;
        std::string iconPath;
        sf::Vector2f offset;
        float spacing = 0;
        float animationFramerate = 0;
    };
}