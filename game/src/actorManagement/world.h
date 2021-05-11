#pragma once

#include <map>
#include <memory>
#include <actor.h>
#include <gameClock.h>

class World
{
	typedef uint32_t id;

	std::map<id, std::unique_ptr<Actor>> actors;
	id lastId = 0;

public:
	template< class T, class... Args >
	void emplace(Args&&... args)
	{
		actors.emplace(
			++lastId,
			std::make_unique<T>(args...)
		);
	}

	void update(const GameClock& time);
	void draw(sf::RenderTarget& target);
};
