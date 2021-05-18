#pragma once

#include <map>
#include <memory>
#include <actorHandle.h>
#include <actor.h>
#include <gameClock.h>
#include <exception>

#include <console.h>
#include <tweener.h>
#include <cassert>

#include <physics/intersections.h>
#include <physics/interactions.h>

namespace wok
{
	class world
	{
		static std::map<actor_id, std::unique_ptr<Actor>> actors;
		static std::vector<Actor*> actorsToCallStartOn;
		static std::vector<Drawable*> drawables;
		static std::vector<Tickable*> tickables;
		static std::vector<Hittable*> hittables;

		static std::vector<Actor*> actorsToAddToCache;

		static std::vector<std::shared_ptr<Tweener>> tweeners;
		static std::vector<actor_id> actorsToRemove;
		static actor_id nextID;

	public:
		static bool logging;

		template< class T, class... Args >
		static ActorHandle<T> createNamedActor(std::string_view name, Args&&... args)
		{
			actor_id id = nextID++;

			auto& result = actors.emplace(
				id,
				std::make_unique<T>(args...)
			);
			Actor* actor = result.first->second.get();
			actor->name = name;
			actor->handle = { id };

			actorsToCallStartOn.push_back(actor);

			if (logging)
			{
				cs::Print("WORLD: ", "Creating actor: ", name, " [", id, "]");
			}
			actorsToAddToCache.push_back(actor);

			return { id };
		}

		template< class T, class... Args >
		static ActorHandle<T> createActor(Args&&... args)
		{
			return createNamedActor<T>("", args...);
		}

		template <typename T>
		static void addTween(std::shared_ptr<T> tweener)
		{
			assert(tweener);

			tweeners.push_back(tweener);

			if (logging)
			{
				cs::Print("WORLD: ", "Added tween ", tweener->name);
			}
		}

		static void update(const GameClock& time);
		static void draw(sf::RenderTarget& target, sf::RenderStates& states);


		template< class T >
		static bool isActorAliveAndMatchesType(const actor_id& id)
		{
			auto& pair = actors.find(id);
			if (pair == actors.end())
			{
				return false;
			}

			return dynamic_cast<T*>(pair->second.get()) != nullptr;
		}


		template< class T >
		static T* getActorPointer(actor_id id)
		{
			auto& pair = actors.find(id);
			if (pair == actors.end())
			{
				return nullptr;
			}

			return dynamic_cast<T*>(pair->second.get());
		}

		template< class T >
		static T& getActor(actor_id id)
		{
			auto& pair = actors.find(id);
			if (pair == actors.end())
			{
				throw std::out_of_range("There is no actor with given id");
			}

			return dynamic_cast<T&>(*pair->second.get());
		}

		static void destroyActor(actor_id id)
		{
			actorsToRemove.push_back(id);
		}

		static void destroyGroup(std::shared_ptr<Group>& group)
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

		static physics::RaycastResult raycast(const m::Ray& ray);

		static void dumpActors(bool details = false);
		static void clear();


	private:
		static void updateActors(const GameClock& time);
		static void updateTweeners(const GameClock& time);
		static void removeDeadTweens();
		static void removeDeadActors();
		static void fillCacheIfNeeded();
		static void addActorToCache(Actor*);
		static void clearActorFromCache(Actor*);
	};
}
