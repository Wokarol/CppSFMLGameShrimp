#pragma once

#include <actor.h>

class Box : public Actor, public Drawable
{
	sf::RectangleShape body;

public:
	Box(int size)
	{
		body = sf::RectangleShape(sf::Vector2f(size, size));
		body.setFillColor(sf::Color::White);
		body.setPosition(sf::Vector2f(0, 0));
	}

	virtual void draw(sf::RenderTarget& target) override
	{
		target.draw(body);
	}
};