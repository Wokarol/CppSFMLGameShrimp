#pragma once
#include <map>
#include <string>
#include <sstream>
#include <memory>
#include <SFML/Graphics/Font.hpp>
#include <console.h>
#include <typeindex>

class res
{
	static inline std::map<
		std::type_index,
		std::map<std::string, std::shared_ptr<void>>
	> loadedAssets{};

public:
	template <typename T>
	static inline std::shared_ptr<T> get(std::string name)
	{
		auto id = std::type_index(typeid(T));
		auto& asset = loadedAssets[id][name];

		auto castedAsset = std::static_pointer_cast<T>(asset);

		if (!castedAsset)
		{
			castedAsset = std::make_shared<T>();
			create<T>(name, castedAsset);
			asset = std::static_pointer_cast<void>(castedAsset);
		}

		return castedAsset;
	}


	template <typename T>
	static void create(const std::string& name, std::shared_ptr<T>& asset);

	static inline void clear()
	{
		loadedAssets.clear();
	}
};
