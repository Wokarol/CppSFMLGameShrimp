#include <resources.h>
#include <assets/bulletSettings.h>

#include <json.hpp>
#include <utils/jsonHelpers.h>
#include <jsonImporters.h>
#include <sstream>
#include <fstream>

template<>
static void wok::res::create(const std::string& name, std::shared_ptr<BulletSettings>& asset)
{
    std::string tilesetPath = (std::stringstream()
        << "assets/" << name << ".jsonc"
        ).str();

    std::ifstream levelFile(tilesetPath);

    nlohmann::json j = nlohmann::json::parse(levelFile, nullptr, true, true);

    asset->textureRect = j["texture_rect"];
    asset->textureName = getAssetPath(name, j["texture_name"]);

    asset->damage = j["damage"];
    asset->velocity = j["velocity"];
    asset->lifespan = j["lifespan"];
}
