#pragma once

#include <string>
#include <SFML/Graphics.hpp>

namespace wok
{
    struct BulletSettings
    {
        float lifespan = 0.f;
        int damage = 0;
        sf::IntRect textureRect;
        std::string textureName;
        float velocity = 0;
    };
}