#pragma once

#include <actor.h>
#include <SFML/Graphics.hpp>

#include <resources.h>
#include <assets/tilesetData.h>

namespace wok
{
    class Tilemap : public Actor, public wok::Drawable, public Collideable
    {
    public:
        Tilemap(const std::shared_ptr<TilesetData>& tileset, int sortingOrder, bool shouldCollide) :
            tileset(res::get<sf::Texture>(tileset->path)),
            data(tileset), sortingOrder(sortingOrder), shouldCollide(shouldCollide)
        {}

        void add_tile(sf::Vector2f tilePos, sf::Vector2f tilesetCoord);

        virtual void draw(sf::RenderTarget& target, sf::RenderStates& states) override;

        virtual int getSortingOrder() override;
        virtual void getColliders(const std::function<void(sf::FloatRect)> yield) override;


    private:
        int sortingOrder;
        bool shouldCollide;
        std::shared_ptr<sf::Texture> tileset;
        std::vector<sf::Sprite> tiles;
        std::shared_ptr<TilesetData> data;
    };
}
