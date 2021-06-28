#include <sceneLoader.h>

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
#include <gameState.h>

#include <assets/tilesetData.h>
#include <assets/cactusPreset.h>
#include <assets/playerSettings.h>
#include <assets/enemySettings.h>
#include <assets/spawnerSettings.h>
#include <assets/dummySettings.h>

#include <utils/rectUtils.h>d

using namespace wok;

void createTilemapAndAddRect(nlohmann::json& json, std::string_view name,
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
    sf::FloatRect tilemapRect;
    for (auto& tile : tiles)
    {
        sf::Vector2f pos = tile["pos"];
        sf::Vector2f tPos = tile["tPos"];

        auto tileRect = tilemap.add_tile(pos, tPos);
        tilemapRect = ru::combineRects(tileRect, tilemapRect);
    }

    game::mapRect = ru::combineRects(game::mapRect, tilemapRect);
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

void addEnemySpawners(nlohmann::json& json, EnemySpawner& spawner)
{
    if (json.is_array())
    {
        for (auto& enemy : json)
        {
            sf::Vector2f pos = enemy.at("pos");
            spawner.addSpawnPoint(pos * project::ppu);
        }
    }
}

void createActors(nlohmann::json& json, std::shared_ptr<Group>& group)
{
    auto smallCactus = res::get<CactusPreset>(project::actorPaths["small_cactus"]);
    generateCacti(json["Cacti"], group, smallCactus, "Cactus");


    auto bigCactus = res::get<CactusPreset>(project::actorPaths["tall_cactus"]);
    generateCacti(json["Tall_Cacti"], group, bigCactus, "Tall Cactus");

    auto spawnerSettings = res::get<SpawnerSettings>(project::actorPaths["spawner"]);
    auto spawner = world::createNamedActor<EnemySpawner>("Enemy Spawner", spawnerSettings);
    spawner->group = group;
    addEnemySpawners(json["Enemy"], *spawner);

    nlohmann::json& player = json["Player"];
    if (player.is_object())
    {
        auto settings = res::get<PlayerSettings>(project::actorPaths["player"]);
        auto playerHandle = world::createNamedActor<Player>("Player", settings);
        auto& playerActor = *playerHandle;
        playerActor.group = group;

        sf::Vector2f pos = player["pos"];
        playerActor.setActorPosition(pos * project::ppu);

        game::player = playerHandle;
    }

    nlohmann::json& dummy = json["Dummy"];
    if (dummy.is_object())
    {
        auto settings = res::get<DummySettings>(project::actorPaths["dummy"]);
        auto dummyHandle = world::createNamedActor<Dummy>("Training Dummy", settings, dummy.at("message"));
        dummyHandle->group = group;

        sf::Vector2f pos = dummy["pos"];
        dummyHandle->setActorPosition(pos * project::ppu);
    }
}

void wok::scenes::loadMenu()
{
    auto group = Group::create("Menu");
    loadedGroups.push_back(group);

    auto font = res::get<sf::Font>("Hard Western");

    sf::Color buttonBackgroundColor(0x684c3cff);
    sf::Color buttonTextColor(0xFFFFFFFF);

    world::createNamedActor<ui::Background>("Background", sf::Color(0xFEF9DBFF))->withGroup(group);
    world::createNamedActor<ui::Title>("Title",
        sf::Color::Black, 20.f, font,
        "Surviving the Heat", 80u
        )->withGroup(group);

    auto startButton = world::createNamedActor<ui::Button>("Start Button", "START", font, 60u,
        sf::Vector2f(40, -160), sf::Vector2f(0.f, 1.f),
        sf::Vector2f(300, 100), buttonBackgroundColor, buttonTextColor);
    startButton->withGroup(group);

    auto quitButton = world::createNamedActor<ui::Button>("Quit Button", "QUIT", font, 60u,
        sf::Vector2f(40, -40), sf::Vector2f(0.f, 1.f),
        sf::Vector2f(300, 100), buttonBackgroundColor, buttonTextColor);
    quitButton->withGroup(group);

    startButton->setOnClick([]() { switchToScene(project::firstLevelScenePath); });
    quitButton->setOnClick([]() { game::close(); });
}

void wok::scenes::loadScene(std::string_view levelPath)
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

    loadedGroups.push_back(group);

    try
    {
        game::mapRect = {};

        auto groundTileset = res::get<TilesetData>(project::actorPaths["desert"]);
        auto wallTileset = res::get<TilesetData>(project::actorPaths["walls"]);
        createTilemapAndAddRect(level["Ground"], "Ground Tilemap", groundTileset, group, -110, false);
        createTilemapAndAddRect(level["Tiles"], "Free Tile Tilemap", groundTileset, group, -100, false);
        createTilemapAndAddRect(level["Walls"], "Walls Tilemap", wallTileset, group, -90, true);
        createActors(level["Actors"], group);
    }
    catch (const std::exception& e)
    {
        console::error(e.what());
    }
}

void wok::scenes::switchToScene(std::string_view name)
{
    game::fader.fade([=]()
        {
            for (auto& group : loadedGroups)
            {
                world::destroyGroup(group);
            }
            loadedGroups.clear();

            loadScene(name);
        });
}

void wok::scenes::switchToMenu()
{
    game::fader.fade([=]()
        {
            for (auto& group : loadedGroups)
            {
                world::destroyGroup(group);
            }
            loadedGroups.clear();

            loadMenu();
        });
}
