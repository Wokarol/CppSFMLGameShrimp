#pragma once

#include "gameClock.h"
#include <actorHandle.h>

class World;
class Actor;

class Tweener
{
protected:
	bool isRunning = true;
	ActorHandle<Actor> actor;

public:
	virtual void tween(const GameClock& time) = 0;
	bool isTweensActorAlive() { return actor.isValid(); }
	bool getIsRunning() { return isRunning && isTweensActorAlive(); }

	virtual ~Tweener() = default;

	friend World;
};
