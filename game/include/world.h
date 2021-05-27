#pragma once

#include <map>
#include <memory>
#include <actorSystem/actorHandle.h>
#include <actor.h>
#include <gameClock.h>
#include <exception>

#include <console.h>
#include <tweener.h>
#include <cassert>

#include <physics/interactions.h>
#include <physics/collisions.h>

namespace wok
{
    class world
    {
        // Objects in world
        static inline std::unordered_map<actor_id, std::unique_ptr<Actor>> actors;
        static inline std::vector<std::shared_ptr<Tweener>> tweeners;

        // One-Frame Cache
        static inline std::vector<Actor*> actorsToCallStartOn;
        static inline std::vector<Actor*> actorsToAddToCache;
        static inline std::vector<actor_id> actorsToRemove;

        // Long Cache
        static inline std::vector<Drawable*> drawables;
        static inline std::vector<Tickable*> tickables;
        static inline std::vector<Hittable*> hittables;
        static inline std::vector<Collideable*> collideables;


        static inline actor_id nextFreeID = 0;

    public:
        static inline bool shouldLog = false;
        static inline bool shouldDrawActors = true;
        static inline bool shouldDrawGizmos = false;

        template< class T, class... Args >
        static ActorHandle<T> createNamedActor(std::string_view name, Args&&... args)
        {
            actor_id id = nextFreeID++;

            auto result = actors.emplace(
                id,
                std::make_unique<T>(args...)
            );
            Actor* actor = result.first->second.get();
            actor->name = name;
            actor->handle = { id };

            actorsToCallStartOn.push_back(actor);

            if (shouldLog)
            {
                console::log("WORLD: ", "Creating actor: ", name, " [", id, "]");
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

            if (shouldLog)
            {
                console::log("WORLD: ", "Added tween ", tweener->name);
            }
        }

        static void update(const GameClock& time);
        static void draw(sf::RenderTarget& target, sf::RenderStates& states);


        template< class T >
        static bool isActorAliveAndMatchesType(const actor_id& id)
        {
            auto pair = actors.find(id);
            if (pair == actors.end())
            {
                return false;
            }

            return dynamic_cast<T*>(pair->second.get()) != nullptr;
        }


        template< class T >
        static T* getActorPointer(actor_id id)
        {
            auto pair = actors.find(id);
            if (pair == actors.end())
            {
                return nullptr;
            }

            return dynamic_cast<T*>(pair->second.get());
        }

        template< class T >
        static T& getActor(actor_id id)
        {
            auto pair = actors.find(id);
            if (pair == actors.end())
            {
                throw std::out_of_range("There is no actor with given id");
            }

            return dynamic_cast<T&>(*pair->second.get());
        }

        static void destroyActor(actor_id id)
        {
            if (!isActorAlive(id))
                return;

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

        static physics::RaycastResult raycastAgainstHitboxes(const m::Ray& ray, float maxDist = -1);
        static void checkForCollisions(const sf::FloatRect& rect, std::function<void(collide::Reaction)> callback);

        static void dumpActors(bool details = false);
        static void onAssetsReloaded();
        static void clear();

    private:
        static void drawActors(sf::RenderTarget& target, sf::RenderStates& states);
        static void drawCollisionGizmos(sf::RenderTarget& target, sf::RenderStates& states);
        static void drawGizmos(sf::RenderTarget& target, sf::RenderStates& states);
        static void updateActors(const GameClock& time);
        static void updateTweeners(const GameClock& time);
        static void removeDeadTweens();
        static void removeDeadActors();
        static void fillCacheIfNeeded();
        static void addActorToCache(Actor*);
        static void clearActorFromCache(Actor*);

        static bool isActorAlive(actor_id id)
        {
            return actors.find(id) != actors.end();
        }
    };
}
