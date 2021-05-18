#include "world.h"
#include <console.h>
#include <iomanip>
#include <sstream>
#include <ios>
#include <algorithm>
#include <cassert>

using namespace wok;

std::map<actor_id, std::unique_ptr<Actor>> world::actors = {};
std::vector<Actor*> world::actorsToCallStartOn = {};
std::vector<Drawable*> world::drawables = {};
std::vector<Tickable*> world::tickables = {};
std::vector<Hittable*> world::hittables = {};

std::vector<std::shared_ptr<Tweener>> world::tweeners = {};
std::vector<actor_id> world::actorsToRemove = {};
actor_id world::nextID = 0;

bool world::logging = false;

void world::updateActors(const GameClock& time)
{
	for (auto& actor : actorsToCallStartOn)
	{
		if (logging)
		{
			cs::Print("WORLD: ", "Starting ", actor->name, " [", actor->handle.id, "]");
		}
		actor->start();
	}
	actorsToCallStartOn.clear();

	for (auto& actor : tickables)
	{
		actor->update(time);
	}
}

void world::updateTweeners(const GameClock& time)
{
	for (auto& tweener : tweeners)
	{
		assert(tweener);
		if (tweener->getIsRunning())
		{
			tweener->tween(time);
		}
	}
}

void world::removeDeadTweens()
{
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
}

void world::removeDeadActors()
{
	for (actor_id& id : actorsToRemove)
	{
		Actor* actor = getActorPointer<Actor>(id);
		assert(actor);
		if (logging)
		{
			cs::Print("WORLD: ", "Destroying actor: ", actor->name, " [", id, "]");
		}

		if (auto tickable = dynamic_cast<Tickable*>(actor))
		{
			tickables.erase(std::find(tickables.begin(), tickables.end(), tickable));
		}

		if (auto drawable = dynamic_cast<Drawable*>(actor))
		{
			drawables.erase(std::find(drawables.begin(), drawables.end(), drawable));
		}

		if (auto hittable = dynamic_cast<Hittable*>(actor))
		{
			hittables.erase(std::find(hittables.begin(), hittables.end(), hittable));
		}

		actors.erase(id);
	}
	actorsToRemove.clear();
}

void world::update(const GameClock& time)
{
	updateActors(time);
	updateTweeners(time);
	removeDeadTweens();
	removeDeadActors();
}

void world::draw(sf::RenderTarget& target, sf::RenderStates& states)
{
	std::sort(drawables.begin(), drawables.end(), [](const auto& a, const auto& b)
		{
			auto sa = std::make_tuple(a->getSortingOrder(), a->getSortingYPos());
			auto sb = std::make_tuple(b->getSortingOrder(), b->getSortingYPos());
			return sa < sb;
		});

	for (auto& drawable : drawables)
	{
		assert(drawable);
		drawable->draw(target, states);
	}
}

intersect::Intersection world::raycast(const m::Ray& ray)
{

	intersect::Intersection closestHit;

	for (auto& hittable : hittables)
	{
		assert(hittable);
		auto hit = hittable->getClosestHit(ray);

		if (!closestHit.hit || (hit.hit && hit.distance < closestHit.distance))
		{
			closestHit = hit;
		}
	}

	return closestHit;
}

void world::dumpActors(bool detailed)
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
	if (detailed)
	{
		cs::Print("----------------- Details ------------------");
		cs::Print("   Hittables: ", hittables.size());
		cs::Print("   Tickables: ", tickables.size());
		cs::Print("   Drawables: ", drawables.size());
	}
	cs::Print("--------------------------------------------");
}

void world::clear()
{
	actors.clear();
	tweeners.clear();
	actorsToCallStartOn.clear();
	drawables.clear();
	tickables.clear();
	hittables.clear();
	actorsToRemove.clear();
	nextID = 0;
}
