#include <actors/cactus.h>
#include <world.h>

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
	auto animation = std::make_shared<SineTweener<float>>(
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
