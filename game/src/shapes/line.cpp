#include <customShapes/line.h>
#include <utils/mathUtils.h>

csf::LineShape::LineShape(float thickness)
{
	body.setFillColor(sf::Color::White);
	start = sf::Vector2f(0, 0);
	end = sf::Vector2f(100, 0);
	this->thickness = thickness;

	isDirty = true;
}

void csf::LineShape::setStart(sf::Vector2f v)
{
	start = v;
	isDirty = true;
}

sf::Vector2f csf::LineShape::getStart() const
{
	return start;
}

void csf::LineShape::setEnd(sf::Vector2f v)
{
	end = v;
	isDirty = true;
}

sf::Vector2f csf::LineShape::getEnd() const
{
	return end;
}

void csf::LineShape::setThickness(float t)
{
	thickness = t;
	isDirty = true;
}
float csf::LineShape::getThickness() const
{
	return thickness;
}

void csf::LineShape::setColor(sf::Color speedX)
{
	body.setFillColor(speedX);
}
sf::Color csf::LineShape::getColor() const
{
	return body.getFillColor();
}

void csf::LineShape::draw(sf::RenderTarget& target, sf::RenderStates states) const
{
	if (isDirty)
	{
		updateShape();
	}
	target.draw(body, states);
}

void csf::LineShape::updateShape() const
{
	isDirty = false;

	auto size = sf::Vector2f(m::dist(start, end), thickness);
	auto angle = m::angle(end - start);

	body.setSize(size);
	body.setOrigin(size / 2.f);
	body.setPosition(m::lerp(start, end, 0.5f));
	body.setRotation(angle);
}
