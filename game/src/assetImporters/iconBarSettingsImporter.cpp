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

    j.at("strip").get_to(asset.animationStrip);
    j.at("icon_size").get_to(asset.iconSize);
    asset.iconPath = getAssetPath(name, j.at("icon_path"));
    j.at("offset").get_to(asset.offset);
    j.at("spacing").get_to(asset.spacing);
    j.at("animation_framerate").get_to(asset.animationFramerate);
}
