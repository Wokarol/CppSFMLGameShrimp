#include <resources.h>
#include <assets/enemySettings.h>

#include <json.hpp>
#include <jsonImporters.h>
#include <utils/jsonHelpers.h>

template<>
static void wok::res::create(const std::string& name, EnemySettings& asset)
{
    auto j = loadJsonFile(name);

    j.at("movement").get_to(asset.movement);
    from_json_with_paths(j.at("gun"), asset.gun, name);

    j.at("max_health").get_to(asset.maxHealth);

    asset.textureName = getAssetPath(name, j.at("texture_name"));
    j.at("texture_rect").get_to(asset.textureRect);

    j.at("fractures").get_to(asset.fractures);
}
