#include <levelLoader.h>

#include <filesystem>
#include <world.h>
#include <actors.h>
#include <json.hpp>
#include <jsonImporters.h>
#include <fstream>
#include <console.h>

#include <tweeners.h>
#include <resources.h>

#include <assets/tilesetData.h>
#include <assets/cactusPreset.h>
#include <assets/playerSettings.h>

constexpr auto ppu = 16;

using namespace wok;

void createTilemap(nlohmann::json& json, std::string_view name,
    TilesetData tileset, std::shared_ptr<Group>& group)
{
    auto& tilemap = *world::createNamedActor<Tilemap>(name, tileset);
    tilemap.group = group;

    if (!json.is_array())
    {
        console::error("Ground key in .level is not an array");
        return;
    }

    std::vector<nlohmann::json> tiles = json;

    for (auto& tile : tiles)
    {
        sf::Vector2f pos = tile["pos"];
        sf::Vector2f tPos = tile["tPos"];

        tilemap.add_tile(pos, tPos);
    }
}

void generateCacti(nlohmann::json& json, std::shared_ptr<Group>& group, const CactusPreset& preset, std::string_view name)
{
    if (json.is_array())
    {
        for (auto& cactus : json)
        {
            auto& prop = *world::createNamedActor<Cactus>(name, preset);
            prop.group = group;

            sf::Vector2f pos = cactus["pos"];

            prop.setPosition(pos.x * ppu, pos.y * ppu);
        }
    }
}

void createActors(nlohmann::json& json, std::shared_ptr<Group>& group)
{
    CactusPreset smallCactus = *res::get<CactusPreset>("actors/smallCactus");
    generateCacti(json["Cacti"], group, smallCactus, "Cactus");


    CactusPreset bigCactus = *res::get<CactusPreset>("actors/tallCactus");
    generateCacti(json["Tall_Cacti"], group, bigCactus, "Tall Cactus");

    nlohmann::json& player = json["Player"];
    if (player.is_object())
    {
        PlayerSettings settings = *res::get<PlayerSettings>("actors/player");
        auto& playerActor = *world::createNamedActor<Player>("Player", settings);
        playerActor.group = group;

        sf::Vector2f pos = player["pos"];
        playerActor.setPosition({ pos.x * ppu, pos.y * ppu });
    }
}

void wok::levels::load(std::string_view levelPath)
{

    std::stringstream pathStream;
    pathStream << "assets/" << levelPath << ".level";
    std::string path = pathStream.str();
    if (!std::filesystem::exists(path))
    {
        console::error(path, " cannot be found in assets folder");
        return;
    }

    nlohmann::json level;
    {
        std::ifstream levelFile(path);
        levelFile >> level;
    }

    std::stringstream groupName;
    groupName << "Level " << level["Title"].get<std::string>();

    auto group = Group::create(groupName.str());

    loadedLevels.push_back(group);

    try
    {
        auto groundTileset = res::get<TilesetData>("actors/tilesets/desert");
        createTilemap(level["Ground"], "Ground Tilemap", *groundTileset, group);
        createTilemap(level["Tiles"], "Free Tile Tilemap", *groundTileset, group);
        createActors(level["Actors"], group);
    }
    catch (const std::exception& e)
    {
        console::error(e.what());
    }
}
