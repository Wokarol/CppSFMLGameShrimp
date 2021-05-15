#include "world.h"
#include <console.h>
#include <iomanip>
#include <sstream>
#include <ios>
#include <algorithm>

void World::update(const GameClock& time)
{
	// Updating actors
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

	// Updating tweeners
	for (auto& tweener : tweeners)
	{
		if (tweener->isTweensActorAlive())
		{
			tweener->tween(time);
		}
	}

	// Removing dead actors
	for (actor_id& id : actorsToRemove)
	{
		if (logging)
		{
			cs::Print("Destroying actor: ", getActor<Actor>(id).name, " [", id, "]");
		}

		actors.erase(id);
	}
	actorsToRemove.clear();

	// Removing dead tweens
	if (tweeners.size() > 0)
	{
		auto& toErase = std::remove_if(tweeners.begin(), tweeners.end(), [](const std::unique_ptr<Tweener>& t)
			{
				return !t->getIsRunning();
			});
		if (toErase != tweeners.end())
		{
			tweeners.erase(toErase);
		}
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

		std::vector<std::pair<Actor*, std::string>> namedActors;

		for (auto& actor : group.second)
		{
			std::stringstream s;
			s << " [" << std::setw(3) << actor->handle.id << "] " << actor->name;

			namedActors.emplace_back(actor, s.str());
		}

		int maxNameLength = std::max_element(namedActors.begin(), namedActors.end(), [](const auto& a, const auto& b)
			{
				return a.second.size() < b.second.size();
			})->second.size();
		int padding = maxNameLength + 3;

		for (auto& namedActor : namedActors)
		{
			cs::Print("    ", std::left, std::setw(padding), namedActor.second, typeid(*namedActor.first).name());
		}
	}
	cs::Print("--------------------------------------------");
}
