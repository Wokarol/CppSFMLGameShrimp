#pragma once

#include <SFML/System/Vector2.hpp>
#include <SFML/Graphics/Rect.hpp>
#include <string>

namespace wok
{
    struct GunSettings
    {
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

        std::string bulletPath;
    };
}