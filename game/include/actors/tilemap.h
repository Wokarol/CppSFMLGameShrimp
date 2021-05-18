#pragma once

#include <actor.h>
#include <SFML/Graphics.hpp>

#include <resources.h>
#include <assets/tilesetData.h>

class Tilemap : public Actor, public Drawable
{
	std::shared_ptr<sf::Texture> tileset;
	int ppu;

	std::vector<sf::Sprite> tiles;

public:
	Tilemap(const TilesetData& tileset) :
		tileset(res::get<sf::Texture>(tileset.path)),
		ppu(tileset.tileSize)
	{}

	void add_tile(sf::Vector2f tilePos, sf::Vector2f tilesetCoord);

	virtual void draw(sf::RenderTarget& target, sf::RenderStates& states) override;

	virtual int getSortingOrder() override;
};