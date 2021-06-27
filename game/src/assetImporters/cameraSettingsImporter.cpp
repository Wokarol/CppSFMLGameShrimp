#include <resources.h>
#include <assets/cameraSettings.h>

#include <json.hpp>
#include <utils/jsonHelpers.h>
#include <jsonImporters.h>

template<>
static void wok::res::create(const std::string& name, CameraSettings& asset)
{
    auto j = loadJsonFile(name);

    j.at("height").get_to(asset.height);
    j.at("offset").get_to(asset.offset);
}