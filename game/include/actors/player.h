#pragma once

#include <actor.h>
#include <memory>
#include <tweeners.h>

#include <customShapes/line.h>
#include <assets/playerSettings.h>

#include <actors/components/movement2D.h>
#include <actors/components/gun.h>

#include <actors/iconBar.h>

namespace wok
{
    class Player : public Actor2D, public wok::Drawable, public Tickable, public Collideable, public Hittable
    {
    public:
        Player(std::shared_ptr<PlayerSettings> settings);

        // Callbacks
        virtual void start() override;
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
        //std::pair<sf::Vector2f, float> updateGunPositionAndRotation(sf::Vector2f mousePosition);
        //m::Ray getGunRay();
        //void updateShootingLogic(sf::Vector2f globalGunPosition, m::Ray gunRay, const GameClock& time);
        //void shoot(sf::Vector2f globalGunPosition, m::Ray gunRay);

    private:
        // Components
        Movement2D movement;
        Gun gun;

        const std::shared_ptr<PlayerSettings> settings;
        std::shared_ptr<sf::Texture> texture;

        sf::Sprite body;

        float invincibilityCooldown = -1.f;
        float invincibilityLength = 0.2f;

        int health;
        ActorHandle<IconBar> healthBar;
    };
}
