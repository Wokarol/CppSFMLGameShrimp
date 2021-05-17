#pragma once

#include <actor.h>
#include <cmath>

class Box : public Actor, public Drawable, public Tickable
{
	sf::RectangleShape body;

	float offsetX, offsetY, speedX, speedY;

public:
	Box(float size, sf::Vector2f position, sf::Color fill, sf::Color outline);

	virtual void update(const GameClock& time) override;

	virtual void draw(sf::RenderTarget& target) override;
};