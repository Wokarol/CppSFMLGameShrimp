#pragma once

#include "gameClock.h"
#include <actorHandle.h>
#include <functional>

class World;
class Actor;

class Tweener
{
protected:
	bool isRunning = true;
	ActorHandle<Actor> actor;

public:
	Tweener(ActorHandle<Actor> actor) :
		actor(actor)
	{}

	virtual void tween(const GameClock& time) = 0;
	bool isTweensActorAlive() { return actor.isValid(); }
	bool getIsRunning() { return isRunning && isTweensActorAlive(); }

	void kill()
	{
		isRunning = false;
	}

	virtual ~Tweener() = default;

	friend World;
};
