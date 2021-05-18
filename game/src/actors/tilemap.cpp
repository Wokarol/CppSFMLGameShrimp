#include <actors/tilemap.h>

#include <SFML/Graphics.hpp>
#include <console.h>

void Tilemap::add_tile(sf::Vector2f tilePos, sf::Vector2f tilesetCoord)
{
	sf::IntRect tileRect(
		(int)(tilesetCoord.x * ppu),
		(int)(tilesetCoord.y * ppu),
		ppu,
		ppu
	);

	sf::Sprite& tile = sf::Sprite(*tileset, sf::IntRect(tileRect));
	tile.setPosition(tilePos * (float)ppu);

	tiles.push_back(tile);
}

void Tilemap::draw(sf::RenderTarget& target, sf::RenderStates& states)
{
	for (auto& tile : tiles)
	{
		target.draw(tile);
	}
}

int Tilemap::getSortingOrder()
{
	return -100;
}
