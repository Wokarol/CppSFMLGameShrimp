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

    //j.at("strip").get_to(asset.animationStrip);
    //j.at("icon_size").get_to(asset.iconSize);
    //asset.iconPath = getAssetPath(name, j.at("icon_path"));
    //j.at("offset").get_to(asset.offset);
    //j.at("spacing").get_to(asset.spacing);
    //j.at("animation_framerate").get_to(asset.animationFramerate);

    J_LOAD("full", asset.spriteRectFull);
    J_LOAD("empty", asset.spriteRectEmpty);
    J_LOAD("strip_gained", asset.animationStripGained);
    J_LOAD("strip_lost", asset.animationStripLost);
    J_LOAD("icon_size", asset.iconSize);
    J_PATH_LOAD("icon_path", asset.iconPath);
    J_LOAD("offset", asset.offset);
    J_LOAD("spacing", asset.spacing);
    J_LOAD("animation_framerate", asset.animationFramerate);
}
