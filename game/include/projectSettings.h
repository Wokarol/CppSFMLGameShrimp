#pragma once

#include <json.hpp>
#include <map>
#include <string>

namespace wok::project
{
    inline std::unordered_map<std::string, std::string> actorPaths;
    inline std::string fontsPath;

    inline void init(nlohmann::json j)
    {
        j["actor_paths"].get_to(project::actorPaths);
        j["fonts_path"].get_to(project::fontsPath);
    }
}
