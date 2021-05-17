#include <actors/box.h>

Box::Box(float size, sf::Vector2f position, sf::Color fill, sf::Color outline)
{
	body = sf::RectangleShape(sf::Vector2f(size, size));
	body.setFillColor(sf::Color::White);
	body.setOrigin(body.getSize() / 2.f);
	body.setPosition(position);
	body.setFillColor(fill);
	body.setOutlineColor(outline);
	body.setOutlineThickness(-2);

	offsetX = (float)(rand() % 200);
	offsetY = (float)(rand() % 200);
	speedX = (rand() % 1000) / 500.f + 1.f;
	speedY = (rand() % 1000) / 500.f + 1.f;
}

void Box::update(const GameClock& time)
{
	body.move(
		std::cos(time.absolute * speedX + offsetX) * 50.f * time.delta,
		std::cos(time.absolute * speedY + offsetY) * 50.f * time.delta
	);
}

void Box::draw(sf::RenderTarget& target, sf::RenderStates& states)
{
	target.draw(body);
}
