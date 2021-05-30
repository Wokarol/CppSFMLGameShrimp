#include <resources.h>
#include <world.h>

#include "SFML/Graphics.hpp"
#include "assets/cactusPreset.h"
#include "assets/playerSettings.h"
#include "assets/bulletSettings.h"
#include "assets/tilesetData.h"
#include "assets/iconBarSettings.h"


#define reload(type) reloadIfTypeExists<type>(); console::log("Reloaded: ", #type);

void wok::res::reloadAll()
{
    res::clearUnused();

    reload(sf::Texture);
    reload(sf::Font);

    reload(wok::CactusPreset);
    reload(wok::PlayerSettings);
    reload(wok::BulletSettings);
    reload(wok::TilesetData);
    reload(wok::IconBarSettings);

    world::onAssetsReloaded();
}
