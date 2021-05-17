#pragma once

#include <gameClock.h>
#include <SFML/Graphics.hpp>
#include <string>
#include <actorHandle.h>
#include <console.h>

class world;

class Group
{
	std::string name;

public:
	Group(std::string name_) :
		name(name_)
	{
		cs::Print("Created group ", name);
	}

	~Group()
	{
		cs::Print("Removed group ", name);
	}

	std::string getName() const
	{
		return name;
	}

	static std::shared_ptr<Group> create(std::string name_)
	{
		return std::make_shared<Group>(name_);
	}
};

class Actor
{
protected:
	ActorHandle<Actor> handle;

public:
	std::shared_ptr<Group> group;
	std::string name;

	virtual void start() {};

	virtual ~Actor() = default;

	ActorHandle<Actor> getHandle() { return handle; }

	friend world;
};

class Drawable
{
public:
	virtual void draw(sf::RenderTarget& target) = 0;
	virtual int getSortingOrder() { return 0; }
	virtual float getSortingYPos() { return 0; }
};

class Tickable
{
public:
	virtual void update(const GameClock& time) {};
};
