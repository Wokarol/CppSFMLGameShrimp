#include "world.h"
#include <console.h>
#include <iomanip>
#include <sstream>
#include <ios>
#include <algorithm>
#include <cassert>

std::map<actor_id, std::unique_ptr<Actor>> world::actors = {};
std::vector<std::shared_ptr<Tweener>> world::tweeners = {};
std::vector<actor_id> world::actorsToRemove = {};
actor_id world::nextID = 0;

bool world::logging = false;

void world::update(const GameClock& time)
{
	// Updating actors
	for (auto& pairs : actors)
	{
		auto& actor = pairs.second;

		if (!actor->startCalled)
		{
			if (logging)
			{
				cs::Print("WORLD: ", "Starting ", actor->name, " [", pairs.first, "]");
			}
			actor->startCalled = true;
			actor->start();
		}
		actor->update(time);
	}

	// Updating tweeners
	for (auto& tweener : tweeners)
	{
		assert(tweener);
		if (tweener->getIsRunning())
		{
			tweener->tween(time);
		}
	}

	// Removing dead tweens
	if (tweeners.size() > 0)
	{
		auto toErase = std::remove_if(tweeners.begin(), tweeners.end(), [](const auto& t)
			{
				bool noLongerRunning = !t->getIsRunning();
				if (noLongerRunning && logging)
				{
					cs::Print("WORLD: ", "Tween for ", t->name, " is no longer running");
				}
				if (noLongerRunning)
					t->afterKilled();
				return noLongerRunning;
			});

		tweeners.erase(toErase, tweeners.end());

		for (auto& tweener : tweeners)
		{
			assert(tweener);
		}
	}

	// Removing dead actors
	for (actor_id& id : actorsToRemove)
	{
		if (logging)
		{
			cs::Print("WORLD: ", "Destroying actor: ", getActor<Actor>(id).name, " [", id, "]");
		}

		actors.erase(id);
	}
	actorsToRemove.clear();
}

void world::draw(sf::RenderTarget& target)
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

void world::dumpActors()
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

		size_t maxNameLength = std::max_element(namedActors.begin(), namedActors.end(), [](const auto& a, const auto& b)
			{
				return a.second.size() < b.second.size();
			})->second.size();
		std::streamsize padding = maxNameLength + 3;

		for (auto& namedActor : namedActors)
		{
			cs::Print("    ", std::left, std::setw(padding), namedActor.second, typeid(*namedActor.first).name());
		}
	}
	cs::Print("--------------------------------------------");
}

void world::clear()
{
	actors.clear();
	tweeners.clear();
	actorsToRemove.clear();
	nextID = 0;
}
