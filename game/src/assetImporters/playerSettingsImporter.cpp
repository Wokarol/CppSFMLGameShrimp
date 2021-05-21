#include <resources.h>
#include <assets/playerSettings.h>

#include <json.hpp>
#include <jsonImporters.h>
#include <sstream>
#include <fstream>

template<>
static void wok::res::create(const std::string& name, std::shared_ptr<PlayerSettings>& asset)
{
    std::string tilesetPath = (std::stringstream()
        << "assets/" << name << ".jsonc"
        ).str();

    std::ifstream levelFile(tilesetPath);

    nlohmann::json j = nlohmann::json::parse(levelFile, nullptr, true, true);

    asset->shootInterval = j["shoot_interval"];
    asset->bulletSpread = j["bullet_spread"];
    asset->muzzleFlashTime = j["muzzle_flash_time"];

    asset->flipTime = j["flip_time"];

    asset->movementSpeed = j["movement_speed"];

    asset->bulletName = j["bullet_name"];

    asset->textureName = j["texture_name"];
    asset->bodyTextureRect = j["body_texture_rect"];
    asset->gunTextureRect = j["gun_texture_rect"];
    asset->muzzleFlashTextureRect = j["muzzle_flash_texture_rect"];

    asset->gunOffset = j["gun_offset"];
    asset->gunOrigin = j["gun_origin"];
    asset->muzzleFlashOffset = j["muzzle_flash_offset"];
}
