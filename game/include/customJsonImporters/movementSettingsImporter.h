#pragma once

#include <actors/components/settings/movementSettings.h>

#include <SFML/System/Vector2.hpp>
#include <json.hpp>

namespace wok
{
    inline void from_json(const nlohmann::json& j, wok::MovementSettings& settings)
    {
        j.at("movement_speed").get_to(settings.movementSpeed);
        j.at("flip_time").get_to(settings.flipTime);
    }
}