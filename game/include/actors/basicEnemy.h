#pragma once

#include <actor.h>
#include <assets/enemySettings.h>

namespace wok
{
    class BasicEnemy : public Actor2D, public wok::Drawable, public Tickable, public Collideable, public Hittable
    {
    public:
        BasicEnemy(std::shared_ptr<EnemySettings> settings) :
            settings(settings)
        {}

        virtual void update(const GameClock& time) override {};
        virtual void draw(sf::RenderTarget& target, sf::RenderStates& states) override {};
        //virtual void getHitboxes(const std::function<void(const physics::Hitbox&)> yield) override;
        virtual void reactToHit(HitData) override {};

        void setActorPosition(sf::Vector2f p) {}
        virtual auto getActorPosition() -> sf::Vector2f override { return {}; }

    private:
        // Components
        Movement2D movement;

        const std::shared_ptr<EnemySettings> settings;
        std::shared_ptr<sf::Texture> texture;

        sf::Sprite body;

        int health;
    };
}