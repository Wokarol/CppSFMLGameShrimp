#pragma once

#pragma once

#include <actors/components/settings/gunSettings.h>

#include <json.hpp>
#include <utils/jsonHelpers.h>
#include "sfml.h"

namespace wok
{
    inline void from_json_with_paths(const nlohmann::json& j, wok::GunSettings& settings, std::string parentPath)
    {
        //j.at("movement_speed").get_to(settings.movementSpeed);
        //j.at("flip_time").get_to(settings.flipTime);


        j.at("shoot_interval").get_to(settings.shootInterval);
        j.at("bullet_spread").get_to(settings.bulletSpread);
        j.at("muzzle_flash_time").get_to(settings.muzzleFlashTime);

        settings.bulletPath = getAssetPath(parentPath, j.at("bullet_path"));

        j.at("gun_texture_rect").get_to(settings.gunTextureRect);
        j.at("muzzle_flash_texture_rect").get_to(settings.muzzleFlashTextureRect);

        j.at("gun_offset").get_to(settings.gunOffset);
        j.at("gun_origin").get_to(settings.gunOrigin);
        j.at("muzzle_flash_offset").get_to(settings.muzzleFlashOffset);
    }
}