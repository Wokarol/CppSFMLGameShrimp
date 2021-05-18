#include <actors/bullet.h>
#include <resources.h>
#include <utils/mathUtils.h>

wok::Bullet::Bullet(sf::Vector2f position, sf::Vector2f direction) :
	direction(direction), texture(res::get<sf::Texture>("actors/shrimp")),
	velocity(100)
{
	setPosition(position);
	setRotation(m::angle(direction));

	setTexture(*texture);
	setTextureRect(sf::IntRect(0, 23, 4, 3));
}

void wok::Bullet::update(const GameClock& time)
{
	move(direction * velocity * time.delta);
}

void wok::Bullet::draw(sf::RenderTarget& target, sf::RenderStates& states)
{
	target.draw(*this, states);
}
