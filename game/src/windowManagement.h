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
		wok::world::dumpActors(event.key.shift);
		wok::DebugPopup::create("Actors dumped");
	}
	if (event.key.code == sf::Keyboard::F2)
	{
		if (cs::IsConsoleVisible())
		{
			cs::HideConsole();
			cs::enableLock = true;
			wok::DebugPopup::create("Console locked");
		}
		else
		{
			cs::enableLock = false;
			wok::DebugPopup::create("Console unlocked");
		}
	}
	if (event.key.code == sf::Keyboard::F3)
	{
		if (cs::IsConsoleVisible())
		{
			cs::HideConsole();
			wok::DebugPopup::create("Console hidden");
		}
		else
		{
			if (cs::enableLock)
			{
				cs::enableLock = false;
				wok::DebugPopup::create("Console unlocked");
			}
			cs::ShowConsole();
			wok::DebugPopup::create("Console shown");
		}
	}

	//if (event.key.code == sf::Keyboard::F5)
	//{
	//	DebugPopup::create("Hello world!");
	//}
}

static void handleEventsAndInput(sf::RenderWindow& window)
{
	wok::input::attack.wasPressedThisFrame = false;
	wok::input::attack.wasReleasedThisFrame = false;

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
			wok::input::handleInputKeysPressed(event);
		}	
		if (event.type == sf::Event::MouseButtonPressed)
		{
			wok::input::handleMouseButtonsPressed(event);
		}
		if (event.type == sf::Event::KeyReleased)
		{
			wok::input::handleInputKeysReleased(event);
		}
		if (event.type == sf::Event::MouseButtonReleased)
		{
			wok::input::handleMouseButtonsReleased(event);
		}
		if (event.type == sf::Event::MouseMoved)
		{
			sf::Vector2i mousePos;
			mousePos.x = event.mouseMove.x;
			mousePos.y = event.mouseMove.y;
			wok::input::mousePositionInWorld = window.mapPixelToCoords(mousePos);
		}
	}
}
