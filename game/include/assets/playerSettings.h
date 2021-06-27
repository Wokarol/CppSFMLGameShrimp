#pragma once

#include <string>
#include <SFML/Graphics.hpp>

#include <actors/components/settings/movementSettings.h>
#include <actors/components/settings/gunSettings.h>

namespace wok
{
    struct PlayerSettings
    {
        MovementSettings movement;
        GunSettings gun;

        std::string cameraPath;

        std::string healthBarName;
        int maxHealth;

        std::string textureName;
        sf::IntRect bodyTextureRect;
    };
}