#pragma once

#include <actor.h>
#include <world.h>
#include <sstream>
#include <string>

class DebugPopup : public Actor, public Drawable, public Tickable
{
	std::shared_ptr<sf::Font> font;
	sf::Text text;
	int lastPos = -1;

public:
	DebugPopup(std::string message);

	virtual void start() override;
	virtual void update(const GameClock& time) override;

	virtual void draw(sf::RenderTarget& target) override;
	virtual int getSortingOrder() override;

	static void create(std::string message)
	{
		std::string name = (std::stringstream() << message.substr(0, 10) << "...").str();
		world::createNamedActor<DebugPopup>(name, message);
	}

	virtual ~DebugPopup();
};