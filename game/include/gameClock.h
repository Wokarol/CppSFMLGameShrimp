#pragma once

#include <SFML/Graphics.hpp>

class GameClock
{
	sf::Clock clock;

public:
	float delta = 0.f;
	float absolute = 0.f;

	void Tick()
	{
		delta = clock.restart().asSeconds();
		absolute += delta;
	}
};