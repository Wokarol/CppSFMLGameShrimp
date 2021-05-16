#pragma once

#include "gameClock.h"
#include <actorHandle.h>
#include <functional>
#include <console.h>

class world;
class Actor;

class Tweener
{
protected:
	bool isRunning = true;
	ActorHandle<Actor> actor;
	std::function<void()> afterKilled = []() {};

public:
	Tweener(ActorHandle<Actor> actor) :
		actor(actor)
	{}

	virtual void tween(const GameClock& time) = 0;
	bool isTweensActorAlive() { return actor.isValid(); }
	bool getIsRunning() { return isRunning && isTweensActorAlive(); }

	void getAfterKilled(std::function<void()> callback)
	{
		this->afterKilled = callback;
	}

	void kill()
	{
		isRunning = false;
	}

	virtual ~Tweener() = default;

	friend world;
};
