#include <resources.h>
#include <assets/playerSettings.h>

#include <json.hpp>
#include <jsonImporters.h>
#include <utils/jsonHelpers.h>
#include <sstream>
#include <fstream>

template<>
static void wok::res::create(const std::string& name, PlayerSettings& asset)
{
    auto j = loadJsonFile(name);

    j.at("shoot_interval").get_to(asset.shootInterval);
    j.at("bullet_spread").get_to(asset.bulletSpread);
    j.at("muzzle_flash_time").get_to(asset.muzzleFlashTime);

    j.at("flip_time").get_to(asset.flipTime);

    j.at("movement_speed").get_to(asset.movementSpeed);

    asset.bulletName = getAssetPath(name, j.at("bullet_name"));
    asset.healthBarName = getAssetPath(name, j.at("health_bar_name"));
    j.at("max_health").get_to(asset.maxHealth);

    asset.textureName = getAssetPath(name, j.at("texture_name"));
    j.at("body_texture_rect").get_to(asset.bodyTextureRect);
    j.at("gun_texture_rect").get_to(asset.gunTextureRect);
    j.at("muzzle_flash_texture_rect").get_to(asset.muzzleFlashTextureRect);

    j.at("gun_offset").get_to(asset.gunOffset);
    j.at("gun_origin").get_to(asset.gunOrigin);
    j.at("muzzle_flash_offset").get_to(asset.muzzleFlashOffset);
}
