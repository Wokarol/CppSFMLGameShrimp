#pragma once

#include <actor.h>
#include <console.h>

class LoggerActor : public Actor
{
	float timer = 0;
	float interval = 1.f;

public:
	virtual void start() override
	{
		cs::Print("Created logger!");
	}

	virtual void update(const GameClock& time) override
	{
		timer += time.delta;

		if (timer >= interval)
		{
			timer -= interval;

			cs::Print("Boop!");
		}
	}
};