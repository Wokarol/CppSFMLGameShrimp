#pragma once

#include <actor.h>
#include <assets/enemySettings.h>
#include <actors/components/gun.h>
#include <actors/components/movement2D.h>
#include <actors/components/health.h>

#include <gates.h>

namespace wok
{
    class BasicEnemy : public Actor2D, public wok::Drawable, public Tickable, public Collideable, public Hittable
    {
    public:
        BasicEnemy(std::shared_ptr<EnemySettings> settings);

        virtual void start(const GameClock&) override;
        virtual void assetsReloaded() override;

        virtual void update(const GameClock& time) override;
        virtual void draw(sf::RenderTarget& target, sf::RenderStates& states) override;
        virtual void getHitboxes(const std::function<void(const physics::Hitbox&)> yield) override;
        virtual void reactToHit(HitData) override;

        void setActorPosition(sf::Vector2f p) { body.setPosition(p); }
        virtual auto getActorPosition() -> sf::Vector2f override { return body.getPosition(); }

    private:
        void onDeath(HitData);

    private:
        // Components
        Movement2D movement;
        Gun gun;
        Health health;

        const std::shared_ptr<EnemySettings> settings;
        std::shared_ptr<sf::Texture> texture;

        sf::Sprite body;

        sf::Vector2f chosenDirection = {};
        float randomStartTimeForShooting;

        // Gates
        AfterDelayGate directionChangedGate;
    };
}