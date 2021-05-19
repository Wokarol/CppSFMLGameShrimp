#pragma once

#include <actor.h>
#include <memory>

namespace wok
{
	class StaticProp : public Actor, public sf::Sprite, public wok::Drawable
	{
		std::shared_ptr<sf::Texture> texture;

	public:
		StaticProp(std::shared_ptr<sf::Texture> texture, sf::IntRect textureRect) :
			Sprite(*texture, textureRect),
			texture(texture)
		{
			setOrigin(
				(float)(textureRect.width / 2),
				(float)(textureRect.height)
			);
		}

		virtual void draw(sf::RenderTarget& target, sf::RenderStates& states) override
		{
			target.draw(*this, states);
		};

		virtual float getSortingYPos() override
		{
			return getPosition().y;
		}
	};
}