#pragma once

#include <string>
#include <SFML/Graphics.hpp>

namespace wok
{
    struct DummySettings
    {
        std::string textureName;
        sf::IntRect textureRect;
        std::vector<sf::IntRect> fractures;
    };
}
