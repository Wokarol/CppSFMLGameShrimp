#pragma once
#include <map>
#include <string>
#include <sstream>
#include <memory>
#include <SFML/Graphics/Font.hpp>
#include <console.h>

namespace fonts
{
	inline std::map<std::string, std::shared_ptr<sf::Font>> loadedFonts;

	inline std::shared_ptr<sf::Font> get(std::string name)
	{
		auto& font = loadedFonts[name];

		if (font)
		{
			return font;
		}
		else
		{
			std::string fontPath = (std::stringstream()
				<< "assets/fonts/" << name << ".ttf"
			).str();
			font = std::make_shared<sf::Font>();

			font->loadFromFile(fontPath);

			return font;
		}
	}

	inline void clear()
	{
		loadedFonts.clear();
	}
}