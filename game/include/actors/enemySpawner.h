#pragma once

#include <world.h>
#include <actor.h>
#include <SFML/Graphics.hpp>

#include <assets/spawnerSettings.h>

#include <actors/basicEnemy.h>

namespace wok
{
    class EnemySpawner : public Actor, public Tickable
    {
    public:
        EnemySpawner(std::shared_ptr<SpawnerSettings> settings);
        void addSpawnPoint(sf::Vector2f);

        virtual void update(const GameClock& time) override;

    private:
        void spawnEnemy(std::string path);

    private:
        std::shared_ptr<SpawnerSettings> settings;
        std::vector<sf::Vector2f> spawnPoints;

        int currentWave = 0;
        int currentEnemyInWave = 0;

        float timeRemainingUntilEnemySpawned = 0;

        std::vector<ActorHandle<BasicEnemy>> spawnedEnemies;
    };
}
