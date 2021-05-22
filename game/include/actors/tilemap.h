#pragma once

#include <actor.h>
#include <SFML/Graphics.hpp>

#include <resources.h>
#include <assets/tilesetData.h>

namespace wok
{
    class Tilemap : public Actor, public wok::Drawable
    {
    public:
        Tilemap(const std::shared_ptr<TilesetData>& tileset) :
            tileset(res::get<sf::Texture>(tileset->path)),
            data(tileset)
        {}

        void add_tile(sf::Vector2f tilePos, sf::Vector2f tilesetCoord);

        virtual void draw(sf::RenderTarget& target, sf::RenderStates& states) override;

        virtual int getSortingOrder() override;


    private:
        std::shared_ptr<sf::Texture> tileset;
        std::vector<sf::Sprite> tiles;
        std::shared_ptr<TilesetData> data;
    };
}
