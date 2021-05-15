#include <actors/player.h>

void Player::setPosition(float x, float y)
{
	setPosition(sf::Vector2f(x, y));
}

void Player::setPosition(sf::Vector2f v)
{
	body.setPosition(v);
}

void Player::update(const GameClock& time)
{
	gun.setPosition(body.getPosition() + gunPivot);
}

void Player::draw(sf::RenderTarget& target)
{
	target.draw(body);
	target.draw(gun);
}
