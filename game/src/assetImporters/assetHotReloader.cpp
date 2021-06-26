#include <resources.h>
#include <world.h>

#include "SFML/Graphics.hpp"

#include "assets/cactusPreset.h"
#include "assets/playerSettings.h"
#include "assets/bulletSettings.h"
#include "assets/tilesetData.h"
#include "assets/iconBarSettings.h"
#include "assets/enemySettings.h"
#include "assets/cameraSettings.h"


#define reload(type) reloadIfTypeExists<type>(); console::log("Reloaded: ", #type);

void wok::res::reloadAll()
{
    res::clearUnused();

    reload(sf::Texture);
    reload(sf::Font);
    reload(sf::Shader);

    reload(wok::CactusPreset);
    reload(wok::PlayerSettings);
    reload(wok::BulletSettings);
    reload(wok::TilesetData);
    reload(wok::IconBarSettings);
    reload(wok::IconBarSettings);
    reload(wok::EnemySettings);
    reload(wok::CameraSettings);

    world::onAssetsReloaded();
}
