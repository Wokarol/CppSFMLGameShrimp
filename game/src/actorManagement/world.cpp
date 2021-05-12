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

void World::dumpActors() const
{
	std::map<std::shared_ptr<Group>, std::vector<Actor*>> actorsByGroups;

	for (auto& pair : actors)
	{
		auto actor = pair.second.get();

		actorsByGroups[actor->group].push_back(actor);
	}

	cs::Print("---------------- Actor Dump ----------------");
	for (auto& group : actorsByGroups)
	{
		if (group.first)
		{
			cs::Print(group.first->getName());
		}
		else
		{
			cs::Print("[global]");
		}

		for (auto& actor : group.second)
		{
			cs::Print("    ", actor->name, " [", actor->handle.id, "]");
		}
	}
	cs::Print("--------------------------------------------");
}
