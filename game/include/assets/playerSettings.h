#pragma once

#include <string>
#include <SFML/Graphics.hpp>

#include <actors/components/settings/movementSettings.h>

namespace wok
{
    struct PlayerSettings
    {
        MovementSettings movement;

        std::string bulletName;

        std::string healthBarName;
        int maxHealth;

        std::string textureName;
        sf::IntRect bodyTextureRect;
        sf::IntRect gunTextureRect;
        sf::IntRect muzzleFlashTextureRect;

        /// In texture space of the player
        sf::Vector2f gunOffset;
        /// In texture space of the gun
        sf::Vector2f gunOrigin;
        /// In relation to a gun
        sf::Vector2f muzzleFlashOffset;

        float shootInterval = 0.f;
        float bulletSpread = 0.f;
        float muzzleFlashTime = 0.f;
    };
}