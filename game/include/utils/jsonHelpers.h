#pragma once

#include <json.hpp>
#include <console.h>

namespace wok
{
    template <typename Key>
    inline bool tryGetString(nlohmann::json& json, Key key, std::string& value)
    {
        auto j = json[key];
        if (j.is_string())
        {
            value = j;
            return true;
        }
        return false;
    }

    /// Gets path taking into consideration relatives. 
    /// Calling getAssetPath("foo/bar", "./target") will return "foo/target"
    inline std::string getAssetPath(std::string parentAssetPath, std::string pathFromJson)
    {
        bool pathIsRelative = pathFromJson[0] == '.';
        if (!pathIsRelative)
        {
            return pathFromJson;
        }

        std::string parentFolderPath = parentAssetPath.substr(0, parentAssetPath.find_last_of('/'));
        return parentFolderPath + pathFromJson.substr(1); // [original/folder] + [/json/path]
    }
}
