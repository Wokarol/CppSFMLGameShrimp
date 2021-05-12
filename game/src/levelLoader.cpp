#include <levelLoader.h>

#include <filesystem>
#include <world.h>
#include <actors.h>
#include <json.hpp>
#include <fstream>
#include <console.h>

void createTilemap(nlohmann::json json, std::string_view name, World& world, std::shared_ptr<Group>& group)
{
	// TODO: FIX HARDCODING

	sf::Texture ground;
	if (!ground.loadFromFile("assets/tilesets/desert_tiles.png"))
	{
		cs::ShowConsole();
	}

	auto& tilemap = *world.createNamedActor<Tilemap>(name, ground, 16);
	tilemap.group = group;

	if (!json.is_array())
	{
		cs::Print("Ground key in .level is not an array");
		return;
	}

	std::vector<nlohmann::json> tiles = json;

	for (auto& tile : tiles)
	{
		nlohmann::json posJ = tile["pos"];
		nlohmann::json tPosJ = tile["tPos"];

		sf::Vector2f pos(posJ[0], posJ[1]);
		sf::Vector2f tPos(tPosJ[0], tPosJ[1]);

		tilemap.add_tile(pos, tPos);
	}
}

void levels::load(std::string_view levelPath, World& world)
{
	std::stringstream groupName;
	groupName << "Level " << levelPath;

	auto group = std::make_shared<Group>(groupName.str());

	std::stringstream pathStream;
	pathStream << "assets/" << levelPath << ".level";
	std::string path = pathStream.str();
	if (!std::filesystem::exists(path))
	{
		cs::Print(path, " cannot be found in assets folder");
		return;
	}

	nlohmann::json level;
	{
		std::ifstream levelFile (path);
		levelFile >> level;
	}

	try
	{
		createTilemap(level["Ground"], "Ground Tilemap", world, group);
		createTilemap(level["Tiles"], "Free Tile Tilemap", world, group);

	}
	catch (const std::exception& e)
	{
		cs::Print(e.what());
	}
}
