#include <actors/player.h>
#include <input.h>
#include <utils/mathUtils.h>
#include <world.h>
#include <tweeners.h>

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

	body.move(m::normalize(input::movement) * time.delta * 50.f);
	if (mousePosition.x < body.getPosition().x == facingRight)
	{
		facingRight = !facingRight;

		float nextScale = facingRight ? 1.f : -1.f;

		if (flipTween)
			flipTween->kill();

		flipTween = std::make_shared<LerpTweener<float>>(handle,
			[this]() { return body.getScale().x; }, [this](float v) { body.setScale(v, 1); },
			nextScale, 0.1f
		);

		world::addTween(flipTween);
	}

	auto globalGunPos = body.getPosition() + m::scale(gunOffset, body.getScale());
	auto rightDirection = sf::Vector2f(body.getScale().x, 0);

	float angleOfGun = m::angle(rightDirection, mousePosition - globalGunPos);

	gun.setPosition(globalGunPos);
	gun.setRotation(angleOfGun);
	gun.setScale(body.getScale());
}

void Player::draw(sf::RenderTarget& target, sf::RenderStates& states)
{
	target.draw(body);
	target.draw(gun);
}
