#pragma once

#include <actor.h>
#include <memory>

class StaticProp : public Actor, public sf::Sprite, public Drawable
{
	std::shared_ptr<sf::Texture> texture;

public:
	StaticProp(std::shared_ptr<sf::Texture> texture, sf::IntRect textureRect) :
		Sprite(*texture, textureRect),
		texture(texture)
	{
		setOrigin(textureRect.width / 2, textureRect.height);
	}

	virtual void draw(sf::RenderTarget& target) override
	{
		target.draw(*this);
	};
};