#pragma once

#include <actor.h>
#include <console.h>

class LoggerActor : public Actor
{
	float timer = 0;
	float interval = 1.f;

	int logsSpitOut = 0;

public:
	LoggerActor()
	{
		cs::Print("Constructed logger");
	}

	~LoggerActor()
	{
		cs::Print("Destructed logger");
	}

	virtual void start() override
	{
		cs::Print("Started logger!");
	}

	virtual void update(const GameClock& time) override
	{
		timer += time.delta;

		if (timer >= interval)
		{
			timer -= interval;

			logsSpitOut++;
			cs::Print("Boop!");
		}
	}

	int getLogCount() const
	{
		return logsSpitOut;
	}
};