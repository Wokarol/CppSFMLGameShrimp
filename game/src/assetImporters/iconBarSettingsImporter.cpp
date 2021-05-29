#include <resources.h>
#include <assets/iconBarSettings.h>

#include <json.hpp>
#include <utils/jsonHelpers.h>
#include <jsonImporters.h>
#include <fstream>

template<>
static void wok::res::create(const std::string& name, IconBarSettings& asset)
{
    auto j = loadJsonFile(name);

    asset.animationStrip = j["strip"].get<std::vector<sf::Vector2i>>();
    asset.iconSize = j["icon_size"];
    asset.iconPath = getAssetPath(name, j["icon_path"]);
    asset.offset = j["offset"];
    asset.spacing = j["spacing"];
    asset.animationFramerate = j["animation_framerate"];
}
