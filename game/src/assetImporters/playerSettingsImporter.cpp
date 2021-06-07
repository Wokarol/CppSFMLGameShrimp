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

    j.at("movement").get_to(asset.movement);
    from_json_with_paths(j.at("gun"), asset.gun, name);

    asset.healthBarName = getAssetPath(name, j.at("health_bar_name"));
    j.at("max_health").get_to(asset.maxHealth);

    asset.textureName = getAssetPath(name, j.at("texture_name"));
    j.at("body_texture_rect").get_to(asset.bodyTextureRect);
}
