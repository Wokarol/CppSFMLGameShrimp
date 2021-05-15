#pragma once

#include <actor.h>
#include <memory>

class Player : public Actor, public Drawable
{
	std::shared_ptr<sf::Texture> texture;
	sf::Sprite body;
	sf::Sprite gun;
	sf::Vector2f gunPivot;

public:
	Player(std::shared_ptr<sf::Texture> texture, 
		sf::IntRect playerSpriteRect, sf::IntRect gunSpriteRect, sf::Vector2f gunPivot):
		texture(texture),
		body(*texture, playerSpriteRect),
		gun(*texture, gunSpriteRect)
	{
		sf::Vector2f pivot(playerSpriteRect.width / 2, playerSpriteRect.height);
		this->gunPivot = gunPivot - pivot;
		body.setOrigin(pivot);
	}

	void setPosition(float x, float y);
	void setPosition(sf::Vector2f v);
	virtual void update(const GameClock& time) override;
	virtual void draw(sf::RenderTarget& target) override;
};