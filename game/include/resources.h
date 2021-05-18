#pragma once
#include <map>
#include <string>
#include <sstream>
#include <memory>
#include <SFML/Graphics/Font.hpp>
#include <console.h>
#include <typeindex>

namespace res
{
	inline std::map<
		std::type_index, 
		std::map<std::string, std::shared_ptr<void>>
	> loadedAssets;

	template <typename T>
	inline std::shared_ptr<T> get(std::string name)
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
	inline void create(const std::string& name, std::shared_ptr<T>& asset)
	{
		std::string fontPath = (std::stringstream()
			<< "assets/fonts/" << name << ".ttf"
			).str();

		asset->loadFromFile(fontPath);
	}

	inline void clear()
	{
		loadedAssets.clear();
	}
}