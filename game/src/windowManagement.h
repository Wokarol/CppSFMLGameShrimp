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

	void centreCamera(sf::RenderTarget& target)
	{
		sf::View view(sf::Vector2f(0, 0), (sf::Vector2f)target.getSize());
		target.setView(view);
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