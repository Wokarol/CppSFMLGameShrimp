#pragma once

#include <string_view>
#include <actor.h>
#include <sstream>
#include <world.h>
#include <map>

namespace wok::levels
{
    inline std::vector<std::shared_ptr<Group>> loadedLevels = {};
    inline std::unordered_map<std::string, std::string> actorPaths;

    inline void init(std::unordered_map<std::string, std::string> actorPaths)
    {
        levels::actorPaths = actorPaths;
    }
    void load(std::string_view name);
}
