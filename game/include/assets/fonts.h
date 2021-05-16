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
			cs::Print("Reusing font asset!");
			return font;
		}
		else
		{
			cs::Print("Loading [", name, ".ttf] from memory!");

			std::string fontPath = (std::stringstream()
				<< "assets/fonts/" << name << ".ttf"
			).str();
			font = std::make_shared<sf::Font>();
			cs::Print("Loading font from ", fontPath);

			font->loadFromFile(fontPath);

			return font;
		}
	}

	inline void clear()
	{
		loadedFonts.clear();
	}
}