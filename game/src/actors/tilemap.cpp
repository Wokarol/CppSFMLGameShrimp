#include <actors/tilemap.h>

#include <SFML/Graphics.hpp>
#include <console.h>

void wok::Tilemap::add_tile(sf::Vector2f tilePos, sf::Vector2f tilesetCoord)
{
    sf::IntRect tileRect(
        (int)(tilesetCoord.x * data->tileSize),
        (int)(tilesetCoord.y * data->tileSize),
        data->tileSize,
        data->tileSize
    );

    // We use basic sprites for now, could be changed to mesh generation later
    sf::Sprite tile = sf::Sprite(*tileset, sf::IntRect(tileRect));
    tile.setPosition(tilePos * (float)data->tileSize);

    tiles.push_back(tile);
}

void wok::Tilemap::draw(sf::RenderTarget& target, sf::RenderStates& states)
{
    for (auto& tile : tiles)
    {
        target.draw(tile, states);
    }
}

int wok::Tilemap::getSortingOrder()
{
    // Tilemap should be rendered below everything else 
    // Should be changed to parameter later
    return -100;
}
