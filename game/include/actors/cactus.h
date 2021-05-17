#pragma once

#include <actor.h>
#include <SFML/Graphics.hpp>
#include <memory>
#include <tweeners.h>

class Cactus : public sf::Sprite, public Actor, public Drawable
{
	std::shared_ptr<sf::Texture> texture;
	float sizeScale;

public:
	Cactus(std::shared_ptr<sf::Texture> texture, sf::IntRect rect, float sizeScale) :
		texture(texture),
		Sprite(*texture, rect),
		sizeScale(sizeScale)
	{
		setOrigin(
			(float)(rect.width / 2),
			(float)(rect.height)
		);
	}

	virtual void start() override
	{
		auto animation = std::make_shared<SineTweener<float>>(
			handle,
			[this](float v) { setRotation(v); },
			-sizeScale, sizeScale, sizeScale
			);
		animation->addTimeOffset((rand() / (float)RAND_MAX) * 20.f);

		world::addTween(animation);
	}

	virtual void draw(sf::RenderTarget& target, sf::RenderStates& states) override
	{
		target.draw(*this);
	};

	virtual float getSortingYPos() override
	{
		return getPosition().y;
	}
};
