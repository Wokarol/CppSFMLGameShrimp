#pragma once

#include <string_view>
#include <actor.h>
#include <sstream>
#include <world.h>
#include <map>

namespace wok::scenes
{
    inline std::vector<std::shared_ptr<Group>> loadedGroups = {};

    void loadMenu();
    void loadScene(std::string_view name);

    void switchToScene(std::string_view name);
    void switchToMenu();
}
