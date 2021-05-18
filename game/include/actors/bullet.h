#pragma once

#include <actor.h>

namespace wok
{
	class Bullet : public sf::Sprite, public Actor, public Drawable, public Tickable 
	{
		const float lifeSpan = 2.f;
		const int damage = 1;
		const sf::IntRect textureRect = { 0, 23, 4, 3 };
		const std::string textureName = "actors/shrimp";
		const float velocity = 300;

	public:
		Bullet(sf::Vector2f position, sf::Vector2f direction);
		virtual void update(const GameClock& time) override;
		virtual void draw(sf::RenderTarget& target, sf::RenderStates& states) override;
		virtual float getSortingYPos() { return getPosition().y; }

	private:
		sf::Vector2f direction;
		std::shared_ptr<sf::Texture> texture;
		float timeSinceStart = 0;
	};
}