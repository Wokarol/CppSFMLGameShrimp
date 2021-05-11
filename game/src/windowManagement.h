#pragma once

#include <SFML/Graphics.hpp>
#include <console.h>

namespace
{
	void initializeBoilerplate()
	{
		cs::HideConsole();
	}

	sf::RenderWindow createWindow()
	{
		auto settings = sf::ContextSettings::ContextSettings(0, 0, 8);
		return sf::RenderWindow(sf::VideoMode(600, 600), "Surviving The Heat", 7U, settings);
	}

	void handleEvents(sf::Window& window)
	{
		sf::Event event;
		while (window.pollEvent(event))
		{
			if (event.type == sf::Event::Closed)
			{
				window.close();
			}
		}
	}
}