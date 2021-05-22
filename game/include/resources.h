#pragma once
#include <map>
#include <string>
#include <sstream>
#include <memory>
#include <SFML/Graphics/Font.hpp>
#include <console.h>
#include <typeindex>
#include <json.hpp>
#include <jsonImporters.h>
#include <fstream>
#include <filesystem>

namespace wok
{
    class res
    {
        static inline std::map<
            std::type_index,
            std::map<std::string, std::shared_ptr<void>>
        > loadedAssets{};

    public:
        template <typename T>
        static std::shared_ptr<T> get(std::string name)
        {
            auto id = std::type_index(typeid(T));
            auto& asset = loadedAssets[id][name];

            auto castedAsset = std::static_pointer_cast<T>(asset);

            if (!castedAsset)
            {
                castedAsset = std::make_shared<T>();
                try
                {
                    create<T>(name, *castedAsset);
                }
                catch (const std::exception& e)
                {
                    console::error(e.what());
                }
                asset = std::static_pointer_cast<void>(castedAsset);
            }

            return castedAsset;
        }

        template <class T>
        static void reloadIfTypeExists()
        {
            auto id = std::type_index(typeid(T));
            auto found = loadedAssets.find(id);
            if (found != loadedAssets.end())
            {
                for (auto& pair : (*found).second)
                {
                    auto castedAsset = std::static_pointer_cast<T>(pair.second);
                    create<T>(pair.first, *castedAsset);
                }
            }
        }

        static void reloadAll();


        template <typename T>
        static void create(const std::string& name, T& asset);

        static void clear()
        {
            loadedAssets.clear();
        }

    private:
        static nlohmann::json loadJsonFile(std::string name)
        {
            std::string path = (std::stringstream()
                << "assets/" << name << ".jsonc"
                ).str();

            if (!std::filesystem::exists(path))
            {
                console::error("Cannot find specified file: ", path);
                abort();
            }

            std::ifstream levelFile(path);

            return nlohmann::json::parse(levelFile, nullptr, true, true);
        }
    };
}
