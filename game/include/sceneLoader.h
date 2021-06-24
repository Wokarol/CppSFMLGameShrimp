#pragma once

#include <string_view>
#include <actor.h>
#include <sstream>
#include <world.h>
#include <map>

namespace wok::scenes
{
    inline std::vector<std::shared_ptr<Group>> loadedScenes = {};

    void loadMenu();
    void loadScene(std::string_view name);
}
