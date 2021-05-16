#pragma once

#include <stdint.h>

class world;

typedef uint32_t actor_id;

template <class T>
class ActorHandle
{
	actor_id id;

public:
	ActorHandle() :
		id(-1)
	{}

	ActorHandle(actor_id id_) :
		id(id_)
	{}

	bool isValid() const
	{
		if (id == -1)
			return false;

		return world::isActorAliveAndMatchesType<T>(id);
	}

	operator T* () const
	{ return world::getActorPointer<T>(id); }

	T& operator*() const
	{ return world::getActor<T>(id); }

	T* operator->() const
	{ return &world::getActor<T>(id); }

	template <class NewT>
	ActorHandle<NewT> as()
	{
		return ActorHandle<NewT>(id);
	}

	void destroy() const
	{
		world::destroyActor(id);
	}

	friend world;
};