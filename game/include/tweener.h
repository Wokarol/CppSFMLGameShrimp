#pragma once

#include <gameClock.h>
#include <actorSystem/actorHandle.h>
#include <functional>
#include <console.h>

namespace wok
{
    class world;
    class Actor;

    class Tweener
    {
    public:
        bool paused = false;

        Tweener(ActorHandle<> actor) :
            actor(actor)
        {
            name = actor->name;
        }

        virtual void tween(const GameClock& time) = 0;

        bool isTweensActorAlive() { return actor.isValid(); }
        bool getIsRunning() { return isRunning && isTweensActorAlive(); }

        void setAfterKilled(std::function<void()> callback) { this->afterKilled = callback; }

        void kill()
        {
            isRunning = false;
        }

        virtual ~Tweener() = default;

    protected:
        bool isRunning = true;
        ActorHandle<> actor;
        std::function<void()> afterKilled = []() {};
        std::string name;

        friend world;
    };
}
