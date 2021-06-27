#include <resources.h>
#include <assets/spawnerSettings.h>

#include <json.hpp>
#include <utils/jsonHelpers.h>
#include <jsonImporters.h>

template<>
static void wok::res::create(const std::string& name, SpawnerSettings& asset)
{
    auto j = loadJsonFile(name);

    j.at("time_between_spawns").get_to(asset.timeBetweenSpawns);

    auto waves = j.at("waves");
    if (!waves.is_array()) return;

    for (auto& wave : waves)
    {
        EnemyWave enemyWave;
        wave.at("count").get_to(enemyWave.count);
        enemyWave.enemyPath = getAssetPath(name, wave.at("path"));

        asset.waves.push_back(enemyWave);
    }
}
