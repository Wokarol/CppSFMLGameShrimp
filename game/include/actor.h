#pragma once

#include <gameClock.h>
#include <SFML/Graphics.hpp>
#include <string>

class World;

class Actor
{
	bool startCalled = false;

public:
	std::string name;

	virtual void start() {};
	virtual void update(const GameClock& time) {};

	virtual ~Actor() {};

	friend World;
};

class Drawable
{
public:
	virtual void draw(sf::RenderTarget& target) = 0;
};
