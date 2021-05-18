#include <resources.h>
#include <assets/tilesetData.h>
#include <json.hpp>
#include <sstream>
#include <fstream>

template<>
static void res::create(const std::string& name, std::shared_ptr<TilesetData>& asset)
{
	std::string tilesetPath = (std::stringstream()
		<< "assets/" << name << ".tileset"
		).str();

	nlohmann::json data;
	{
		std::ifstream levelFile(tilesetPath);
		levelFile >> data;
	}

	asset->path = data["path"];
	asset->tileSize = data["tile_size"];
}
