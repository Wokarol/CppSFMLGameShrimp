#include <actors/player.h>
#include <input.h>
#include <utils/mathUtils.h>

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
	auto mousePosition = input::mousePositionInWorld;

	body.move(input::movement * time.delta * 30.f);
	if (mousePosition.x < body.getPosition().x)
	{
		body.setScale(-1, 1);
	}
	else
	{
		body.setScale(1, 1);
	}

	auto globalGunPos = body.getPosition() + m::scale(gunOffset, body.getScale());
	auto rightDirection = sf::Vector2f(body.getScale().x, 0);

	float angleOfGun = m::angle(rightDirection, mousePosition - globalGunPos);

	gun.setPosition(globalGunPos);
	gun.setRotation(angleOfGun);
	gun.setScale(body.getScale());
}

void Player::draw(sf::RenderTarget& target)
{
	target.draw(body);
	target.draw(gun);
}
