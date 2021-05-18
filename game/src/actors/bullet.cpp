#include <actors/bullet.h>
#include <resources.h>
#include <utils/mathUtils.h>
#include <world.h>

wok::Bullet::Bullet(sf::Vector2f position, sf::Vector2f direction) :
	direction(m::normalize(direction)), texture(res::get<sf::Texture>(textureName))
{
	setPosition(position);
	setRotation(m::angle(direction));

	setTexture(*texture);
	setTextureRect(textureRect);
	setOrigin(getTextureRect().width / 2.f, getTextureRect().width / 2.f);
}

void wok::Bullet::update(const GameClock& time)
{
	timeSinceStart += time.delta;
	if (timeSinceStart > lifeSpan)
	{
		handle.destroy();
		return;
	}


	float distanceDelta = velocity * time.delta;

	auto& raycastResult = world::raycast(m::Ray(getPosition(), direction), distanceDelta);
	if (raycastResult.hitActor.isValid())
	{
		raycastResult.hitActor->reactToHit(raycastResult.intersection, damage);
		handle.destroy();
	}

	move(direction * distanceDelta);
}

void wok::Bullet::draw(sf::RenderTarget& target, sf::RenderStates& states)
{
	target.draw(*this, states);
}
