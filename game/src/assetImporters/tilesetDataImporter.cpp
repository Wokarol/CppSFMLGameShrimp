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
    std::string tilesetPath = (std::stringstream()
        << "assets/" << name << ".jsonc"
        ).str();

    std::ifstream levelFile(tilesetPath);

    nlohmann::json data = nlohmann::json::parse(levelFile, nullptr, true, true);

    asset->path = getAssetPath(name, data["path"]);
    asset->tileSize = data.at("tile_size").get<int>();
}
