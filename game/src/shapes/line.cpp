#include <customShapes/line.h>
#include <utils/mathUtils.h>

wok::csf::LineShape::LineShape(float thickness)
{
	body.setFillColor(sf::Color::White);
	start = sf::Vector2f(0, 0);
	end = sf::Vector2f(100, 0);
	this->thickness = thickness;

	isDirty = true;
}

void wok::csf::LineShape::setStart(sf::Vector2f v)
{
	start = v;
	isDirty = true;
}

sf::Vector2f wok::csf::LineShape::getStart() const
{
	return start;
}

void wok::csf::LineShape::setEnd(sf::Vector2f v)
{
	end = v;
	isDirty = true;
}

sf::Vector2f wok::csf::LineShape::getEnd() const
{
	return end;
}

void wok::csf::LineShape::setThickness(float t)
{
	thickness = t;
	isDirty = true;
}
float wok::csf::LineShape::getThickness() const
{
	return thickness;
}

void wok::csf::LineShape::setColor(sf::Color speedX)
{
	body.setFillColor(speedX);
}
sf::Color wok::csf::LineShape::getColor() const
{
	return body.getFillColor();
}

void wok::csf::LineShape::draw(sf::RenderTarget& target, sf::RenderStates states) const
{
	if (isDirty)
	{
		updateShape();
	}
	target.draw(body, states);
}

void wok::csf::LineShape::updateShape() const
{
	isDirty = false;

	auto size = sf::Vector2f(m::dist(start, end), thickness);
	auto angle = m::angle(end - start);

	body.setSize(size);
	body.setOrigin(size / 2.f);
	body.setPosition(m::lerp(start, end, 0.5f));
	body.setRotation(angle);
}
