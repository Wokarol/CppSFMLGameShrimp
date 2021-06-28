#pragma once

#include <string>
#include <SFML/Graphics/Rect.hpp>

#include <actors/components/settings/movementSettings.h>
#include <actors/components/settings/gunSettings.h>

namespace wok
{
    struct EnemySettings
    {
        MovementSettings movement;
        GunSettings gun;

        int maxHealth;

        std::string textureName;
        sf::IntRect textureRect;
        std::vector<sf::IntRect> fractures;
    };
}
