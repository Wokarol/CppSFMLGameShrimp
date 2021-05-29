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

    j["strip"].get_to<std::vector<sf::Vector2i>>(asset.animationStrip);
    j["icon_size"].get_to(asset.iconSize);
    asset.iconPath = getAssetPath(name, j["icon_path"]);
    j["offset"].get_to(asset.offset);
    j["spacing"].get_to(asset.spacing);
    j["animation_framerate"].get_to(asset.animationFramerate);
}
