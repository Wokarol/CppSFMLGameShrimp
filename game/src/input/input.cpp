#include <input.h>
#include <SFML/Graphics.hpp>
#include <console.h>

sf::Vector2f wok::input::movement = { 0, 0 };
sf::Vector2f wok::input::mousePositionInWorld = { 0, 0 };
wok::input::key wok::input::attack;

struct DirectionKey
{
	bool pressed;
	sf::Keyboard::Key code;
	sf::Vector2f direction;

	DirectionKey(sf::Keyboard::Key key, sf::Vector2f direction) :
		pressed(false),
		code(key), direction(direction)
	{}
};

std::vector<DirectionKey> directionKeys{
	{ sf::Keyboard::A, sf::Vector2f(-1, 0) },
	{ sf::Keyboard::D, sf::Vector2f( 1, 0) },
	{ sf::Keyboard::W, sf::Vector2f(0, -1) },
	{ sf::Keyboard::S, sf::Vector2f(0,  1) },
};

void wok::input::handleInputKeysPressed(const sf::Event& event)
{
	for (auto& key : directionKeys)
	{
		if (!key.pressed && event.key.code == key.code)
		{
			movement += key.direction;
			key.pressed = true;
		}
	}
}

void wok::input::handleInputKeysReleased(const sf::Event& event)
{
	for (auto& key : directionKeys)
	{
		if (key.pressed && event.key.code == key.code)
		{
			movement -= key.direction;
			key.pressed = false;
		}
	}
}

void wok::input::handleMouseButtonsPressed(const sf::Event& event)
{
	if (event.mouseButton.button == sf::Mouse::Left)
	{
		input::attack.wasPressedThisFrame = true;
		input::attack.isPressed = true;
	}
}

void wok::input::handleMouseButtonsReleased(const sf::Event& event)
{
	if (event.mouseButton.button == sf::Mouse::Left)
	{
		input::attack.wasReleasedThisFrame = true;
		input::attack.isPressed = false;
	}
}
