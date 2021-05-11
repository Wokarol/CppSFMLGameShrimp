#pragma once

#include <map>
#include <memory>
#include <actor.h>
#include <gameClock.h>
#include <exception>

typedef uint32_t actor_id;

class World;

template <class T>
class ActorHandle
{
	actor_id id;
	World* world;

	ActorHandle(actor_id id_, World* world_) :
		id(id_),
		world(world_)
	{}

public:
	bool isAlive() const
	{
		return world->isActorAlive(id);
	}

	T& operator*() const
	{
		return world->getActor<T>(id);
	}

	T* operator->() const
	{
		return &world->getActor<T>(id);
	}

	void destroy() const
	{
		world->destroyActor(id);
	}

	friend World;
};

class World
{

	std::map<actor_id, std::unique_ptr<Actor>> actors;
	actor_id lastId = 0;

public:
	template< class T, class... Args >
	ActorHandle<T> emplace(Args&&... args)
	{
		actor_id id = ++lastId;

		actors.emplace(
			id,
			std::make_unique<T>(args...)
		);

		return { id, this };
	}

	void update(const GameClock& time);
	void draw(sf::RenderTarget& target);

	bool isActorAlive(const actor_id& id) const
	{
		return actors.find(id) != actors.end();
	}

	template< class T >
	T& getActor(actor_id id) const
	{
		auto& pair = actors.find(id);
		if (pair == actors.end())
		{
			throw std::out_of_range("There is no actor with id " + id);
		}

		return dynamic_cast<T&>(*pair->second.get());
	}

	void destroyActor(actor_id id)
	{
		actors.erase(id);
	}
};
