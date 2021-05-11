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

void World::draw(sf::RenderTarget& target)
{
	for (auto& pairs : actors)
	{
		auto& actor = pairs.second;

		if (auto drawable = dynamic_cast<Drawable*>(actor.get()))
		{
			drawable->draw(target);
		}
	}
}
