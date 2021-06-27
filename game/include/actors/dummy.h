#pragma once

#include <actor.h>
#include <SFML/Graphics.hpp>
#include <memory>
#include <tweeners.h>
#include <assets/dummySettings.h>

namespace wok
{
    class Dummy :
        public Actor2D, public wok::Drawable, public Tickable, public Hittable, public Collideable
    {
    public:
        Dummy(std::shared_ptr<DummySettings> settings, std::string message);

        virtual void assetsReloaded() override;

        virtual void update(const GameClock& time) override;
        virtual void draw(sf::RenderTarget& target, sf::RenderStates& states) override;

        virtual float getSortingYPos() override { return body.getPosition().y; }

        virtual void reactToHit(HitData) override;
        virtual void getHitboxes(const CollisionContext&, const std::function<void(const physics::Hitbox&)> yield) override;

        virtual sf::Vector2f getActorPosition() override { return body.getPosition(); }
        void setActorPosition(sf::Vector2f v) { return body.setPosition(v); }

    private:
        std::shared_ptr<DummySettings> settings;
        std::shared_ptr<sf::Texture> texture;
        std::shared_ptr<sf::Font> font;
        std::string message;

        // Model
        sf::Sprite body;
        sf::Text messageText;
    };
}
