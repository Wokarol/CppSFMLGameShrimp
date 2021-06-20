#pragma once

#include <actor.h>
#include <memory>
#include <tweeners.h>

#include <customShapes/line.h>
#include <assets/playerSettings.h>

#include <actors/components/movement2D.h>
#include <actors/components/gun.h>
#include <actors/components/health.h>

namespace wok
{
    class Player : public Actor2D, public wok::Drawable, public Tickable, public Collideable, public Hittable
    {
    public:
        Player(std::shared_ptr<PlayerSettings> settings);

        // Callbacks
        virtual void start(const GameClock&) override;
        virtual void update(const GameClock& time) override;
        virtual void assetsReloaded() override;
        virtual void draw(sf::RenderTarget& target, sf::RenderStates& states) override;
        virtual void drawGizmos(sf::RenderTarget& target, sf::RenderStates& states) override;
        virtual void getHitboxes(const std::function<void(const physics::Hitbox&)> yield) override;
        virtual void reactToHit(HitData) override;

        // Getters Setters
        void setActorPosition(sf::Vector2f v) { body.setPosition(v); }
        virtual auto getSortingYPos() -> float override { return body.getPosition().y; }
        virtual auto getActorPosition() -> sf::Vector2f override { return body.getPosition(); }

    private:
        void onDeath(HitData);

    private:
        // Components
        Movement2D movement;
        Gun gun;
        Health health;

        const std::shared_ptr<PlayerSettings> settings;
        std::shared_ptr<sf::Texture> texture;

        sf::Sprite body;

        float invincibilityCooldown = -1.f;
        float invincibilityLength = 0.2f;
    };
}
