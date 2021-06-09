#pragma once

#include <actor.h>
#include <assets/enemySettings.h>

namespace wok
{
    class BasicEnemy : public Actor2D
    {
    public:
        BasicEnemy(std::shared_ptr<EnemySettings> settings) :
            settings(settings)
        {}

        void setActorPosition(sf::Vector2f p) {}
        virtual auto getActorPosition() -> sf::Vector2f override { return {}; }

    private:
        const std::shared_ptr<EnemySettings> settings;
    };
}