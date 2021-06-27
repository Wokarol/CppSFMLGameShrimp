#pragma once

#include <string>
#include <vector>

namespace wok
{
    struct EnemyWave
    {
        int count;
        std::string enemyPath;
    };

    struct SpawnerSettings
    {
        std::vector<EnemyWave> waves;
        float timeBetweenSpawns;
        float spawnDuration;
    };
}