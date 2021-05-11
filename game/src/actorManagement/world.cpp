#include "world.h"
#include <console.h>

void World::update(const GameClock& time)
{
	for (auto& pairs : actors)
	{
		auto& actor = pairs.second;

		if (!actor->startCalled)
		{
			if (logging)
			{
				cs::Print("Starting ", actor->name, " [", pairs.first, "]");
			}
			actor->startCalled = true;
			actor->start();
		}
		actor->update(time);
	}

	for (actor_id& id : actorsToRemove)
	{
		if (logging)
		{
			cs::Print("Destroying actor: ", getActor<Actor>(id).name, " [", id, "]");
		}

		actors.erase(id);
	}
	actorsToRemove.clear();
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
