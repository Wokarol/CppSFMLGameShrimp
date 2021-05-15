#pragma once

#include <SFML/Graphics.hpp>

struct input
{
	static sf::Vector2f movement;
	static sf::Vector2f mousePositionInWorld;

	static void handleInputKeysPressed(const sf::Event& event);
	static void handleInputKeysReleased(const sf::Event& event);
};