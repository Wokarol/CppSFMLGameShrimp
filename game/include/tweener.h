#pragma once

#include "gameClock.h"
#include <actorHandle.h>
#include <functional>
#include <console.h>

namespace wok
{
	class world;
	class Actor;

	class Tweener
	{
	protected:
		bool isRunning = true;
		ActorHandle<Actor> actor;
		std::function<void()> afterKilled = []() {};
		std::string name;

	public:
		bool paused = false;

		Tweener(ActorHandle<Actor> actor) :
			actor(actor)
		{
			name = actor->name;
		}

		virtual void tween(const GameClock& time) = 0;
		bool isTweensActorAlive() { return actor.isValid(); }
		bool getIsRunning() { return isRunning && isTweensActorAlive(); }

		void setAfterKilled(std::function<void()> callback)
		{
			this->afterKilled = callback;
		}

		void kill()
		{
			isRunning = false;
		}

		virtual ~Tweener() = default;

		friend world;
	};

}