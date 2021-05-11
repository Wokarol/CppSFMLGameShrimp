#pragma once

#include <actor.h>
#include <cmath>

class Box : public Actor, public Drawable
{
	sf::RectangleShape body;

	float offsetX, offsetY, speedX, speedY;

public:
	Box(float size, sf::Vector2f position, sf::Color fill, sf::Color outline)
	{
		body = sf::RectangleShape(sf::Vector2f(size, size));
		body.setFillColor(sf::Color::White);
		body.setOrigin(body.getSize() / 2.f);
		body.setPosition(position);
		body.setFillColor(fill);
		body.setOutlineColor(outline);
		body.setOutlineThickness(-2);

		offsetX = rand() % 200;
		offsetY = rand() % 200;
		speedX = (rand() % 1000) / 500.f + 1.f;
		speedY = (rand() % 1000) / 500.f + 1.f;
	}

	virtual void update(const GameClock& time) override
	{
		body.move(
			std::cos(time.absolute * speedX + offsetX) * 50.f * time.delta,
			std::cos(time.absolute * speedY + offsetY) * 50.f * time.delta
		);
	}

	virtual void draw(sf::RenderTarget& target) override
	{
		target.draw(body);
	}
};