#pragma once

#include <SFML/Graphics.hpp>
#include <console.h>
#include <world.h>
#include <input.h>

#include <actors.h>

static void initializeBoilerplate()
{
	cs::HideConsole();
}

static sf::RenderWindow createWindow()
{
	auto settings = sf::ContextSettings::ContextSettings(0, 0, 8);
	return sf::RenderWindow(sf::VideoMode(25 * 32, 16 * 32), "Surviving The Heat", 7U, settings);
}

static void centreCamera(sf::RenderTarget& target)
{
	sf::View view(sf::Vector2f(0, 0), (sf::Vector2f)target.getSize());
	target.setView(view);
}

static void setCornerCam(sf::RenderTarget& target)
{
	sf::Vector2f targetSize = (sf::Vector2f)target.getSize();
	float ratio = targetSize.x / targetSize.y;

	float height = 16 * 16;
	float width = ratio * height;
	sf::View view = sf::View(sf::FloatRect(0, 0, width, height));
	target.setView(view);
}

static void handleDebugKeys(const sf::Event& event)
{
	if (event.key.code == sf::Keyboard::F10)
	{
		world::dumpActors(event.key.shift);
		DebugPopup::create("Actors dumped");
	}
	if (event.key.code == sf::Keyboard::F2)
	{
		if (cs::IsConsoleVisible())
		{
			cs::HideConsole();
			cs::enableAutoShow = false;
			DebugPopup::create("Console locked");
		}
		else
		{
			cs::enableAutoShow = true;
			DebugPopup::create("Console unlocked");
		}
	}
	if (event.key.code == sf::Keyboard::F3)
	{
		if (cs::IsConsoleVisible())
		{
			cs::HideConsole();
			DebugPopup::create("Console hidden");
		}
		else
		{
			cs::ShowConsole();
			DebugPopup::create("Console shown");
			if (!cs::enableAutoShow)
			{
				cs::enableAutoShow = true;
				DebugPopup::create("Console unlocked");
			}
		}
	}

	//if (event.key.code == sf::Keyboard::F5)
	//{
	//	DebugPopup::create("Hello world!");
	//}
}

static void handleEventsAndInput(sf::RenderWindow& window)
{
	input::attack.wasPressedThisFrame = false;
	input::attack.wasReleasedThisFrame = false;

	sf::Event event;
	while (window.pollEvent(event))
	{
		if (event.type == sf::Event::Closed)
		{
			window.close();
		}
		if (event.type == sf::Event::KeyPressed)
		{
			handleDebugKeys(event);
			input::handleInputKeysPressed(event);
		}	
		if (event.type == sf::Event::MouseButtonPressed)
		{
			input::handleMouseButtonsPressed(event);
		}
		if (event.type == sf::Event::KeyReleased)
		{
			input::handleInputKeysReleased(event);
		}
		if (event.type == sf::Event::MouseButtonReleased)
		{
			input::handleMouseButtonsReleased(event);
		}
		if (event.type == sf::Event::MouseMoved)
		{
			sf::Vector2i mousePos;
			mousePos.x = event.mouseMove.x;
			mousePos.y = event.mouseMove.y;
			input::mousePositionInWorld = window.mapPixelToCoords(mousePos);
		}
	}
}
