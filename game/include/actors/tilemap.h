#pragma once

#include <actor.h>
#include <SFML/Graphics.hpp>

class Tilemap : public Actor, public Drawable
{
	std::shared_ptr<sf::Texture> tileset;
	int ppu;

	std::vector<sf::Sprite> tiles;

public:
	Tilemap(std::shared_ptr<sf::Texture> tileset_, int ppu_) :
		tileset(tileset_),
		ppu(ppu_)
	{}

	void add_tile(sf::Vector2f tilePos, sf::Vector2f tilesetCoord);

	virtual void draw(sf::RenderTarget& target, sf::RenderStates& states) override;

	virtual int getSortingOrder() override;
};