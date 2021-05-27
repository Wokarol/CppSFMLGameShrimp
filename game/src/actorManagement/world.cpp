#include <world.h>
#include <console.h>
#include <iomanip>
#include <sstream>
#include <ios>
#include <algorithm>
#include <cassert>
#include <physics/interactions.h>

using namespace wok;

void world::updateActors(const GameClock& time)
{
    for (auto& actor : actorsToCallStartOn)
    {
        if (shouldLog) console::log("WORLD: ", "Starting ", actor->name, " [", actor->handle.id, "]");

        actor->start();
    }
    actorsToCallStartOn.clear();

    for (auto& actor : tickables)
    {
        assert(actor);
        actor->update(time);
    }
}

void wok::world::onAssetsReloaded()
{
    for (auto& actor : actors)
    {
        actor.second->assetsReloaded();
    }
}

void world::updateTweeners(const GameClock& time)
{
    for (auto& tweener : tweeners)
    {
        assert(tweener);
        if (tweener->getIsRunning() && !tweener->paused)
        {
            tweener->tween(time);
        }
    }
}

void world::removeDeadTweens()
{
    if (tweeners.size() <= 0)
        return;

    auto toErase = std::remove_if(tweeners.begin(), tweeners.end(), [](const auto& t)
        {
            if (t->getIsRunning())
                return false;

            if (shouldLog) console::log("WORLD: ", "Tween for ", t->name, " is no longer running");
            t->afterKilled();
            return true;
        });

    tweeners.erase(toErase, tweeners.end());
}

void world::removeDeadActors()
{
    std::sort(actorsToRemove.begin(), actorsToRemove.end());
    auto end = std::unique(actorsToRemove.begin(), actorsToRemove.end());

    for (auto it = actorsToRemove.begin(); it != end; it++)
    {
        auto id = *it;
        Actor* actor = getActorPointer<Actor>(id);
        assert(actor);

        if (shouldLog) console::log("WORLD: ", "Destroying actor: ", actor->name, " [", id, "]");

        clearActorFromCache(actor);
        actors.erase(id);
    }
    actorsToRemove.clear();
}

void wok::world::fillCacheIfNeeded()
{
    for (auto& actor : actorsToAddToCache)
    {
        addActorToCache(actor);
    }
    actorsToAddToCache.clear();
}

void wok::world::addActorToCache(Actor* actor)
{
    assert(actor);

    if (shouldLog) console::log("WORLD: ", "Adding: ", actor->name, " [", actor->handle.id, "] to cache");

    if (auto tickable = dynamic_cast<Tickable*>(actor))
    {
        if (shouldLog) console::log("    ", "Actor is tickable");
        tickables.push_back(tickable);
    }

    if (auto drawable = dynamic_cast<Drawable*>(actor))
    {
        if (shouldLog) console::log("    ", "Actor is drawable");
        drawables.push_back(drawable);
    }

    if (auto hittable = dynamic_cast<Hittable*>(actor))
    {
        if (shouldLog) console::log("    ", "Actor is hittable");
        hittables.push_back(hittable);
    }

    if (auto collideable = dynamic_cast<Collideable*>(actor))
    {
        if (shouldLog) console::log("    ", "Actor is collideable");
        collideables.push_back(collideable);
    }
}

void wok::world::clearActorFromCache(Actor* actor)
{
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


    if (auto collideable = dynamic_cast<Collideable*>(actor))
    {
        collideables.erase(std::find(collideables.begin(), collideables.end(), collideable));
    }
}

void world::update(const GameClock& time)
{
    fillCacheIfNeeded();

    updateActors(time);
    fillCacheIfNeeded();

    updateTweeners(time);
    removeDeadTweens();
    removeDeadActors();
}

void world::draw(sf::RenderTarget& target, sf::RenderStates& states)
{
    drawActors(target, states);

    if (shouldDrawGizmos)
    {
        drawGizmos(target, states);
    }
}

void world::drawActors(sf::RenderTarget& target, sf::RenderStates& states)
{
    std::sort(drawables.begin(), drawables.end(), [](const auto& a, const auto& b)
        {
            auto orderOfA = std::make_tuple(a->getSortingOrder(), a->getSortingYPos());
            auto orderOfB = std::make_tuple(b->getSortingOrder(), b->getSortingYPos());
            return orderOfA < orderOfB;
        });

    for (auto& drawable : drawables)
    {
        assert(drawable);

        if (!shouldDrawActors && !drawable->shouldDrawAlways())
            continue;

        drawable->draw(target, states);
    }
}

void world::drawGizmos(sf::RenderTarget& target, sf::RenderStates& states)
{
    for (auto& actor : actors)
    {
        assert(actor.second.get());
        actor.second->drawGizmos(target, states);
    }
}

physics::RaycastResult world::raycastAgainstHitboxes(const m::Ray& ray, float maxRaycastDistance)
{
    physics::Intersection closestHit;
    ActorHandle<Actor> hitActorHandle;

    for (auto& collideable : collideables)
    {
        assert(collideable);
        collideable->getHitboxes([&](const physics::Hitbox& hitbox)
            {
                auto intersection = hitbox.getIntersection(ray);

                if (!intersection.hit)
                    return;

                bool thereWasAHitAlread = closestHit.hit;
                bool myHitIsCloser = intersection.distance < closestHit.distance;

                if (!thereWasAHitAlread || myHitIsCloser)
                {
                    auto hitActor = dynamic_cast<Actor*>(collideable);
                    assert(hitActor);

                    closestHit = intersection;
                    hitActorHandle = hitActor->getHandle();
                }
            });
    }

    bool raycastIsInfinite = maxRaycastDistance < 0;
    bool closestHitIsInRange = closestHit.distance <= maxRaycastDistance;

    if (closestHitIsInRange || raycastIsInfinite)
    {
        // We hit something
        return { closestHit, hitActorHandle.as<Collideable>() };
    }
    else
    {
        // We hit nothing
        return { };
    }
}

void wok::world::checkForCollisions(const sf::FloatRect& rect, std::function<void(collide::Reaction)> reactionCallback)
{
    for (auto& col : collideables)
    {
        assert(col);
        col->getColliders([&](sf::FloatRect collider)
            {
                auto reaction = collide::AABBWithAABB(rect, collider);
                reactionCallback(reaction);
            });
    }
}

void world::dumpActors(bool detailed)
{
    std::map<std::shared_ptr<Group>, std::vector<Actor*>> actorsByGroups;

    for (auto& pair : actors)
    {
        auto actor = pair.second.get();

        actorsByGroups[actor->group].push_back(actor);
    }

    console::error("---------------- Actor Dump ----------------");
    for (auto& group : actorsByGroups)
    {
        if (group.first)
        {
            console::error(group.first->getName());
        }
        else
        {
            console::error("[global]");
        }

        std::vector<std::pair<Actor*, std::string>> namedActors;

        for (auto& actor : group.second)
        {
            std::stringstream s;
            s << " [" << std::setw(4) << actor->handle.id << "] " << actor->name;

            namedActors.emplace_back(actor, s.str());
        }

        size_t maxNameLength = std::max_element(namedActors.begin(), namedActors.end(), [](const auto& a, const auto& b)
            {
                return a.second.size() < b.second.size();
            })->second.size();
            std::streamsize padding = maxNameLength + 3;

            for (auto& namedActor : namedActors)
            {
                console::error("    ", std::left, std::setw(padding), namedActor.second, typeid(*namedActor.first).name());
            }
    }
    if (detailed)
    {
        console::error("----------------- Details ------------------");
        console::error("   Hittables: ", hittables.size());
        console::error("   Tickables: ", tickables.size());
        console::error("   Drawables: ", drawables.size());
        console::error("   Collideables:  ", collideables.size());
        console::error("   Tweeners:  ", tweeners.size());
    }
    console::error("--------------------------------------------");
}

void world::clear()
{
    actors.clear();
    tweeners.clear();

    actorsToCallStartOn.clear();
    actorsToAddToCache.clear();
    actorsToRemove.clear();

    drawables.clear();
    tickables.clear();
    hittables.clear();

    nextFreeID = 0;
}
