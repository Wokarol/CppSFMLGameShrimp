#include <actors/enemySpawner.h>

#include <actors/enemySpawnMarker.h>

#include <gameState.h>

#include <rng.h>
#include <resources.h>

wok::EnemySpawner::EnemySpawner(std::shared_ptr<SpawnerSettings> settings) :
    settings(settings)
{ }

void wok::EnemySpawner::addSpawnPoint(sf::Vector2f v)
{
    spawnPoints.push_back(v);
}

void wok::EnemySpawner::update(const GameClock& time)
{
    spawnedEnemies.erase(
        std::remove_if(spawnedEnemies.begin(), spawnedEnemies.end(), [](const ActorHandle<BasicEnemy>& enemy)
            {
                return !enemy.isValid();
            }),
        spawnedEnemies.end());

    if (!game::dummyKilled) return;
    if (game::fader.isFading()) return;

    timeRemainingUntilEnemySpawned -= time.delta;

    if (currentEnemyInWave >= settings->waves[currentWave].count)
    {
        if (spawnedEnemies.size() > 2) return;
        if (awaitedSpawns > 0) return;

        currentEnemyInWave = 0;
        currentWave++;
    }

    if (currentWave >= settings->waves.size())
    {
        currentWave--;
    }

    if (timeRemainingUntilEnemySpawned < 0)
    {
        spawnEnemy(settings->waves[currentWave].enemyPath);
        timeRemainingUntilEnemySpawned = settings->timeBetweenSpawns;
        currentEnemyInWave++;
    }

}

void wok::EnemySpawner::spawnEnemy(std::string path)
{
    auto myHandle = handle;

    int spawnIndex = randomizer::getBetween(0, (int)spawnPoints.size());

    awaitedSpawns++;
    auto marker = world::createNamedActor<EnemySpawnMarker>("Enemy to be spawned", settings->spawnDuration,
        spawnPoints[spawnIndex], [this, path, spawnIndex, myHandle]()
        {
            if (!myHandle.isValid()) return;

            auto enemySettings = res::get<EnemySettings>(path);
            auto enemy = world::createNamedActor<BasicEnemy>("Enemy", enemySettings);
            enemy->group = group;
            enemy->setActorPosition(spawnPoints[spawnIndex]);

            spawnedEnemies.push_back(enemy);
            awaitedSpawns--;
        });
    marker->group = group;
}
