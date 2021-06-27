#include <resources.h>
#include <assets/dummySettings.h>
#include <json.hpp>
#include <utils/jsonHelpers.h>
#include <jsonImporters.h>

template<>
static void wok::res::create(const std::string& name, DummySettings& asset)
{
    auto data = loadJsonFile(name);

    asset.textureName = getAssetPath(name, data.at("texture_path"));
    data.at("texture_rect").get_to(asset.textureRect);

    data.at("fractures").get_to(asset.fractures);
}