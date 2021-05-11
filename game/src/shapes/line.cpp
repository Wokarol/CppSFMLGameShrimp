#include "line.h"
#include <mathUtils.h>

sf::LineShape::LineShape(float thickness)
{
	body.setFillColor(Color::White);
	start = sf::Vector2f(0, 0);
	end = sf::Vector2f(100, 0);
	this->thickness = thickness;

	isDirty = true;
}

void sf::LineShape::setStart(Vector2f v)
{
	start = v;
	isDirty = true;
}

sf::Vector2f sf::LineShape::getStart() const
{
	return start;
}

void sf::LineShape::setEnd(Vector2f v)
{
	end = v;
	isDirty = true;
}

sf::Vector2f sf::LineShape::getEnd() const
{
	return end;
}

void sf::LineShape::setThickness(float t)
{
	thickness = t;
	isDirty = true;
}
float sf::LineShape::getThickness() const
{
	return thickness;
}

void sf::LineShape::setColor(sf::Color speedX)
{
	body.setFillColor(speedX);
}
sf::Color sf::LineShape::getColor() const
{
	return body.getFillColor();
}

void sf::LineShape::draw(RenderTarget& target, RenderStates states) const
{
	if (isDirty)
	{
		updateShape();
	}
	target.draw(body, states);
}

void sf::LineShape::updateShape() const
{
	isDirty = false;

	auto size = sf::Vector2f(m::dist(start, end), thickness);
	auto angle = m::angle(end - start);

	body.setSize(size);
	body.setOrigin(size / 2.f);
	body.setPosition(m::lerp(start, end, 0.5f));
	body.setRotation(angle);
}
