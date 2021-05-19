#pragma once

#include <SFML/Graphics.hpp>

namespace wok
{
	struct input
	{
		struct key
		{
			bool wasPressedThisFrame = false;
			bool wasReleasedThisFrame = false;
			bool isPressed = false;
		};

		static sf::Vector2f movement;
		static sf::Vector2f mousePositionInWorld;

		static key attack;
		static bool slowMode;

		static void handleInputKeysPressed(const sf::Event& event);
		static void handleInputKeysReleased(const sf::Event& event);
		static void handleMouseButtonsPressed(const sf::Event& event);
		static void handleMouseButtonsReleased(const sf::Event& event);
	};
}