#pragma once

#include <gameClock.h>

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
	virtual void draw() {};
};
