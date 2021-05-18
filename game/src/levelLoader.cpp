#include <levelLoader.h>

#include <filesystem>
#include <world.h>
#include <actors.h>
#include <json.hpp>
#include <fstream>
#include <console.h>

#include <tweeners.h>
#include <resources.h>

constexpr auto ppu = 16;

void createTilemap(nlohmann::json& json, std::string_view name, std::shared_ptr<Group>& group)
{
	// TODO: FIX HARDCODING

	auto& ground = res::get<sf::Texture>("tilesets/desert_tiles");

	auto& tilemap = *world::createNamedActor<Tilemap>(name, ground, ppu);
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

void createActors(nlohmann::json& json, std::shared_ptr<Group>& group)
{
	auto& cactiTexture = res::get<sf::Texture>("actors/cacti");
	auto& playerTexture = res::get<sf::Texture>("actors/shrimp");

	nlohmann::json& cacti = json["Cacti"];
	if (cacti.is_array())
	{
		for (auto& cactus : cacti)
		{
			auto& propHandle = world::createNamedActor<Cactus>("Cactus", 
				cactiTexture, sf::IntRect(0, 16, 16, 16), 5.f);
			auto& prop = *propHandle;
			prop.group = group;

			nlohmann::json posJ = cactus["pos"];

			sf::Vector2f pos(posJ[0], posJ[1]);
			prop.setPosition(pos.x * ppu, pos.y * ppu);
		}
	}

	nlohmann::json& tall_cacti = json["Tall_Cacti"];
	if (tall_cacti.is_array())
	{
		for (auto& tall_cactus : tall_cacti)
		{
			auto& propHandle = world::createNamedActor<Cactus>("Tall Cactus", 
				cactiTexture, sf::IntRect(16, 0, 16, 32), 4.f);
			auto& prop = *propHandle;
			prop.group = group;

			nlohmann::json posJ = tall_cactus["pos"];

			sf::Vector2f pos(posJ[0], posJ[1]);
			prop.setPosition(pos.x * ppu, pos.y * ppu);
		}
	}

	nlohmann::json& player = json["Player"];
	if (player.is_object())
	{
		auto& playerActor = *world::createNamedActor<Player>("Player",
			playerTexture, 
			sf::IntRect(0, 0, 13, 14), sf::IntRect(0, 16, 6, 4), 
			sf::Vector2f(8, 10), sf::Vector2f(1, 1)
		);
		playerActor.group = group;

		nlohmann::json posJ = player["pos"];

		sf::Vector2f pos(posJ[0], posJ[1]);

		playerActor.setPosition(pos.x * ppu, pos.y * ppu);
	}
}

void levels::load(std::string_view levelPath)
{

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

	std::stringstream groupName;
	groupName << "Level " << level["Title"].get<std::string>();

	auto group = Group::create(groupName.str());

	loadedLevels.push_back(group);

	try
	{
		createTilemap(level["Ground"], "Ground Tilemap", group);
		createTilemap(level["Tiles"], "Free Tile Tilemap", group);
		createActors(level["Actors"], group);
	}
	catch (const std::exception& e)
	{
		cs::Print(e.what());
	}
}
