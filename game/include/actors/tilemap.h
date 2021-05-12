#pragma once

#include <actor.h>
#include <SFML/Graphics.hpp>

class Tilemap : public Actor, public Drawable
{
	sf::Texture tileset;
	int ppu;

	std::vector<sf::Sprite> tiles;

public:
	Tilemap(sf::Texture tileset_, int ppu_) :
		tileset(tileset_),
		ppu(ppu_)
	{}

	void add_tile(sf::Vector2f tilePos, sf::Vector2f tilesetCoord);

	virtual void draw(sf::RenderTarget& target) override;
};