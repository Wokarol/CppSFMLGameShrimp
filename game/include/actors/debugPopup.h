#pragma once

#include <actor.h>
#include <world.h>
#include <sstream>
#include <string>

class DebugPopup : public Actor, public Drawable
{
	std::shared_ptr<sf::Font> font;
	sf::Text text;

public:
	DebugPopup(std::string message);

	virtual void start() override;

	virtual void draw(sf::RenderTarget& target) override;

	static void create(std::string message)
	{
		std::string name = (std::stringstream() << message.substr(0, 10) << "...").str();
		world::createNamedActor<DebugPopup>(name, message);
	}
};