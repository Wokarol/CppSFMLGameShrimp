#include <actors/bullet.h>
#include <resources.h>
#include <utils/mathUtils.h>
#include <world.h>

wok::Bullet::Bullet(sf::Vector2f position, sf::Vector2f direction) :
	direction(m::normalize(direction)), texture(res::get<sf::Texture>("actors/shrimp")),
	velocity(300)
{
	setPosition(position);
	setRotation(m::angle(direction));

	setTexture(*texture);
	setTextureRect(sf::IntRect(0, 23, 4, 3));
	setOrigin(getTextureRect().width / 2.f, getTextureRect().width / 2.f);
}

void wok::Bullet::update(const GameClock& time)
{
	float distanceDelta = velocity * time.delta;

	auto& raycastResult = world::raycast(m::Ray(getPosition(), direction), distanceDelta);
	if (raycastResult.hitActor.isValid())
	{
		cs::Print(raycastResult.hitActor.as<Actor>()->name);
		raycastResult.hitActor->reactToHit(raycastResult.intersection, 1);
		handle.destroy();
	}


	move(direction * distanceDelta);
}

void wok::Bullet::draw(sf::RenderTarget& target, sf::RenderStates& states)
{
	target.draw(*this, states);
}
