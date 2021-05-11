#pragma once

#include <map>
#include <memory>
#include <actorHandle.h>
#include <actor.h>
#include <gameClock.h>
#include <exception>

#include <console.h>

class World
{
	std::map<actor_id, std::unique_ptr<Actor>> actors;
	std::vector<actor_id> actorsToRemove;
	actor_id nextID = 0;

public:
	bool logging = false;

	template< class T, class... Args >
	ActorHandle<T> createNamedActor(std::string name, Args&&... args)
	{
		actor_id id = nextID++;

		auto& result = actors.emplace(
			id,
			std::make_unique<T>(args...)
		);
		Actor& actor = *result.first->second;
		actor.name = name;
		actor.handle = { id, this };

		if (logging)
		{
			cs::Print("Creating actor: ", name, " [", id, "]");
		}

		return { id, this };
	}

	template< class T, class... Args >
	ActorHandle<T> createActor(Args&&... args)
	{
		return createNamedActor<T>("", args...);
	}

	void update(const GameClock& time);
	void draw(sf::RenderTarget& target);


	template< class T >
	bool isActorAliveAndMatchesType(const actor_id& id) const
	{
		auto& pair = actors.find(id);
		if (pair == actors.end())
		{
			return false;
		}

		return dynamic_cast<T*>(pair->second.get()) != nullptr;
	}


	template< class T >
	T* getActorPointer(actor_id id) const
	{
		auto& pair = actors.find(id);
		if (pair == actors.end())
		{
			throw std::out_of_range("There is no actor with given id");
		}

		return dynamic_cast<T*>(pair->second.get());
	}

	template< class T >
	T& getActor(actor_id id) const
	{
		auto& pair = actors.find(id);
		if (pair == actors.end())
		{
			throw std::out_of_range("There is no actor with given id");
		}

		return dynamic_cast<T&>(*pair->second.get());
	}


	template< class T >
	T& getActor(actor_id id)
	{
		return const_cast<T&>(const_cast<const World*>(this)->getActor<T>(id));
	}

	void destroyActor(actor_id id)
	{
		actorsToRemove.push_back(id);
	}

	void destroyGroup(std::shared_ptr<Group>& group)
	{
		for (auto& actor : actors)
		{
			auto actorGroup = actor.second->group;
			if (actorGroup == group)
			{
				destroyActor(actor.first);
			}
		}
	}

	void dumpActors() const;
};
