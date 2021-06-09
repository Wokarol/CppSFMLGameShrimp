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
#include <projectSettings.h>

#include <assets/tilesetData.h>
#include <assets/cactusPreset.h>
#include <assets/playerSettings.h>
#include <assets/enemySettings.h>

using namespace wok;

void createTilemap(nlohmann::json& json, std::string_view name,
    std::shared_ptr<TilesetData> tileset, std::shared_ptr<Group>& group, int sortingOrder, bool shouldCollide)
{
    auto& tilemap = *world::createNamedActor<Tilemap>(name, tileset, sortingOrder, shouldCollide);
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

void generateCacti(nlohmann::json& json, std::shared_ptr<Group>& group, const std::shared_ptr<CactusPreset>& preset, std::string_view name)
{
    if (json.is_array())
    {
        for (auto& cactus : json)
        {
            auto& prop = *world::createNamedActor<Cactus>(name, preset);
            prop.group = group;

            sf::Vector2f pos = cactus["pos"];

            prop.setPosition(pos * project::ppu);
        }
    }
}

void spawnEnemies(nlohmann::json& json, std::shared_ptr<Group>& group, const std::shared_ptr<EnemySettings>& settings)
{
    if (json.is_array())
    {
        for (auto& enemy : json)
        {
            auto& basicEnemy = *world::createNamedActor<BasicEnemy>("Basic Enemy", settings);
            basicEnemy.group = group;

            sf::Vector2f pos = enemy.at("pos");
            basicEnemy.setActorPosition(pos * project::ppu);
        }
    }
}

void createActors(nlohmann::json& json, std::shared_ptr<Group>& group)
{
    auto smallCactus = res::get<CactusPreset>(project::actorPaths["small_cactus"]);
    generateCacti(json["Cacti"], group, smallCactus, "Cactus");


    auto bigCactus = res::get<CactusPreset>(project::actorPaths["tall_cactus"]);
    generateCacti(json["Tall_Cacti"], group, bigCactus, "Tall Cactus");

    auto basicEnemy = res::get<EnemySettings>(project::actorPaths["basic_enemy"]);
    spawnEnemies(json.at("Enemy"), group, basicEnemy);

    nlohmann::json& player = json["Player"];
    if (player.is_object())
    {
        auto settings = res::get<PlayerSettings>(project::actorPaths["player"]);
        auto& playerActor = *world::createNamedActor<Player>("Player", settings);
        playerActor.group = group;

        sf::Vector2f pos = player["pos"];
        playerActor.setActorPosition(pos * project::ppu);
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
        auto groundTileset = res::get<TilesetData>(project::actorPaths["desert"]);
        auto wallTileset = res::get<TilesetData>(project::actorPaths["walls"]);
        createTilemap(level["Ground"], "Ground Tilemap", groundTileset, group, -110, false);
        createTilemap(level["Tiles"], "Free Tile Tilemap", groundTileset, group, -100, false);
        createTilemap(level["Walls"], "Walls Tilemap", wallTileset, group, -90, true);
        createActors(level["Actors"], group);
    }
    catch (const std::exception& e)
    {
        console::error(e.what());
    }
}
