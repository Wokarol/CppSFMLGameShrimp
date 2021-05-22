#include <resources.h>
#include <assets/tilesetData.h>
#include <json.hpp>
#include <utils/jsonHelpers.h>
#include <sstream>
#include <fstream>

#include <regex>

template<>
static void wok::res::create(const std::string& name, std::shared_ptr<TilesetData>& asset)
{
    auto data = loadJsonFile(name);

    asset->path = getAssetPath(name, data["path"]);
    asset->tileSize = data.at("tile_size").get<int>();
}
