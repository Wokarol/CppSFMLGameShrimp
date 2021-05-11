#pragma once

#include <gameClock.h>
#include <SFML/Graphics.hpp>
#include <string>
#include <actorHandle.h>

class World;

class Actor
{
public:
	bool startCalled = false;
	ActorHandle<Actor> handle;

public:
	std::string name;

	virtual void start() {};
	virtual void update(const GameClock& time) {};

	virtual ~Actor() {};

	ActorHandle<Actor> getHandle() { return handle; }

	friend World;
};

class Drawable
{
public:
	virtual void draw(sf::RenderTarget& target) = 0;
};
