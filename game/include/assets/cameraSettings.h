#pragma once

#include <SFML/System/Vector2.hpp>

namespace wok
{
    struct CameraSettings
    {
        float height = 0.f;
        sf::Vector2f offset;
    };
}