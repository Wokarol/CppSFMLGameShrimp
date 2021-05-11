#include "world.h"

void World::update(const GameClock& time)
{
	for (auto& pairs : actors)
	{
		auto& actor = pairs.second;

		if (!actor->startCalled)
		{
			actor->startCalled = true;
			actor->start();
		}
		actor->update(time);
	}
}
