#pragma once

#include <actor.h>
#include <assets/bulletSettings.h>

namespace wok
{
    class Bullet : public sf::Sprite, public Actor, public Drawable, public Tickable
    {
    public:
        Bullet(sf::Vector2f position, sf::Vector2f direction, BulletSettings settings);
        virtual void update(const GameClock& time) override;
        virtual void draw(sf::RenderTarget& target, sf::RenderStates& states) override;
        virtual float getSortingYPos() { return getPosition().y; }

    private:
        const BulletSettings settings;

        sf::Vector2f direction;
        std::shared_ptr<sf::Texture> texture;
        float timeSinceStart = 0;
    };
}
