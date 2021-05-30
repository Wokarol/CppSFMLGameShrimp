#include <resources.h>
#include <assets/bulletSettings.h>

#include <json.hpp>
#include <utils/jsonHelpers.h>
#include <jsonImporters.h>
#include <sstream>
#include <fstream>

template<>
static void wok::res::create(const std::string& name, BulletSettings& asset)
{
    auto j = loadJsonFile(name);

    j.at("texture_rect").get_to(asset.textureRect);
    asset.textureName = getAssetPath(name, j.at("texture_name"));

    j.at("damage").get_to(asset.damage);
    j.at("velocity").get_to(asset.velocity);
    j.at("lifespan").get_to(asset.lifespan);
}
