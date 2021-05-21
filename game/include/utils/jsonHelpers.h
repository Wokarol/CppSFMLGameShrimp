#pragma once

#include <json.hpp>

namespace wok
{
    template <typename Key>
    static bool tryGetString(nlohmann::json& json, Key key, std::string& value)
    {
        auto j = json[key];
        if (j.is_string())
        {
            value = j;
            return true;
        }
        return false;
    }
}
