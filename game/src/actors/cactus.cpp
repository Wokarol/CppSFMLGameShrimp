#include <actors/cactus.h>
#include <world.h>
#include "..\..\include\actors\cactus.h"

wok::Cactus::Cactus(std::shared_ptr<sf::Texture> texture, sf::IntRect rect, float sizeScale) :
	texture(texture),
	Sprite(*texture, rect),
	sizeScale(sizeScale)
{
	setOrigin(
		(float)(rect.width / 2),
		(float)(rect.height)
	);
}

void wok::Cactus::start()
{
	animation = std::make_shared<SineTweener<float>>(
		handle,
		[this](float v) { setRotation(v); },
		-sizeScale, sizeScale, sizeScale
		);
	animation->addTimeOffset((rand() / (float)RAND_MAX) * 20.f);

	world::addTween(animation);
}

void wok::Cactus::draw(sf::RenderTarget& target, sf::RenderStates& states)
{
	target.draw(*this);
}

wok::intersect::Intersection wok::Cactus::getClosestHit(const m::Ray& ray)
{
	auto& bounds = getGlobalBounds();
	sf::RectangleShape collider({ bounds.width, bounds.height });
	collider.setPosition(bounds.left, bounds.top);

	return intersect::rayWithAABB(ray, collider);
}

void wok::Cactus::reactToHit(const intersect::Intersection& intersection)
{
	float dir = 1;
	if (intersection.ray.direction.x < 0)
	{
		dir = -1;
	}

	animation->paused = true;
	auto hit = std::make_shared<LerpTweener<float>>(handle,
		[this]() { return getRotation(); }, [this](float v) { setRotation(v); },
		getRotation() + 5.f * dir * sizeScale, 2.f
		);

	hit->setAfterKilled([=]() { animation->paused = false; });
	hit->setEasing([](float t) { return std::sin(t * 7 * 3.1415f) * pow(2.7182f, -5 * t); });

	world::addTween(hit);
}
