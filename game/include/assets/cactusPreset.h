#pragma once

#include <string>
#include <SFML/Graphics.hpp>

namespace wok
{
    struct CactusPreset
    {
        std::string textureName = "actors/cacti";
        sf::IntRect textureRect{};
        float animationScale = 1.f;
        int startingHealth = 3;
        std::vector<sf::IntRect> fractures;
    };
}
