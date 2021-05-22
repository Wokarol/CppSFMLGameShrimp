#pragma once

#include <string_view>
#include <actor.h>
#include <sstream>
#include <world.h>
#include <map>

namespace wok::levels
{
    inline std::vector<std::shared_ptr<Group>> loadedLevels = {};

    void load(std::string_view name);
}
