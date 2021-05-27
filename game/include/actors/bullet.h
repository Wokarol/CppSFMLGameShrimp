#pragma once

#include <actor.h>
#include <assets/bulletSettings.h>

namespace wok
{
    class Bullet : public sf::Sprite, public Actor2D, public Drawable, public Tickable
    {
    public:
        Bullet(sf::Vector2f position, sf::Vector2f direction, std::shared_ptr<BulletSettings> settings);
        virtual void assetsReloaded() override;
        virtual void update(const GameClock& time) override;
        virtual void draw(sf::RenderTarget& target, sf::RenderStates& states) override;
        virtual void drawGizmos(sf::RenderTarget& target, sf::RenderStates& states) override;

        virtual float getSortingYPos() { return getPosition().y; }

        virtual sf::Vector2f getActorPosition() override { return getPosition(); }

    private:
        const std::shared_ptr<BulletSettings> settings;

        sf::Vector2f direction;
        std::shared_ptr<sf::Texture> texture;
        float timeSinceStart = 0;
    };
}
