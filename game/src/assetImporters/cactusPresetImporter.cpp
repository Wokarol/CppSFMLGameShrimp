#include <resources.h>
#include <assets/cactusPreset.h>
#include <json.hpp>
#include <utils/jsonHelpers.h>
#include <jsonImporters.h>
#include <sstream>
#include <fstream>

template<>
static void wok::res::create(const std::string& name, CactusPreset& asset)
{
    auto data = loadJsonFile(name);

    asset.textureName = getAssetPath(name, data.at("texture_name"));
    data.at("texture_rect").get_to(asset.textureRect);

    data.at("animation_scale").get_to(asset.animationScale);
    data.at("starting_health").get_to(asset.startingHealth);

    data.at("fractures").get_to(asset.fractures);
}
