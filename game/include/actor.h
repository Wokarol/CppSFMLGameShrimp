#pragma once

#include <gameClock.h>
#include <SFML/Graphics.hpp>

class World;

class Actor
{
	bool startCalled = false;

public:
	virtual void start() {};
	virtual void update(const GameClock& time) {};

	friend World;
};

class Drawable
{
public:
	virtual void draw(sf::RenderTarget& target) = 0;
};
