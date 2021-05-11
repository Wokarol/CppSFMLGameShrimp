#pragma once

#include <stdint.h>

class World;

typedef uint32_t actor_id;

template <class T>
class ActorHandle
{
	actor_id id;
	World* world = nullptr;

public:
	ActorHandle() :
		id(-1),
		world(nullptr)
	{}

	ActorHandle(actor_id id_, World* world_) :
		id(id_),
		world(world_)
	{}

	bool isValid() const
	{
		if (world == nullptr)
			return false;

		return world->isActorAliveAndMatchesType<T>(id);
	}

	operator T* () const
	{
		return world->getActorPointer<T>(id);
	}

	T& operator*() const
	{
		return world->getActor<T>(id);
	}

	T* operator->() const
	{
		return &world->getActor<T>(id);
	}

	template <class NewT>
	ActorHandle<NewT> as()
	{
		return ActorHandle<NewT>(id, world);
	}

	void destroy() const
	{
		world->destroyActor(id);
	}

	friend World;
};