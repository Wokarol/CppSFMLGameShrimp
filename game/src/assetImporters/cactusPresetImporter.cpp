#include <resources.h>
#include <assets/cactusPreset.h>
#include <json.hpp>
#include <jsonImporters.h>
#include <sstream>
#include <fstream>

template<>
static void wok::res::create(const std::string& name, std::shared_ptr<CactusPreset>& asset)
{
    std::string tilesetPath = (std::stringstream()
        << "assets/" << name << ".jsonc"
        ).str();

    std::ifstream levelFile(tilesetPath);

    nlohmann::json data = nlohmann::json::parse(levelFile, nullptr, true, true);

    asset->textureName = data.at("texture_name").get<std::string>();
    asset->textureRect = data.at("texture_rect").get<sf::IntRect>();

    asset->animationScale = data.at("animation_scale").get<float>();
    asset->startingHealth = data.at("starting_health").get<float>();

    for (auto& fracture : data.at("fractures"))
    {
        asset->fractures.push_back(fracture.get<sf::IntRect>());
    }
}
