#include <fstream>

#include <SFML/Window.hpp>
#include <SFML/Graphics.hpp>

#include "console.h"
#include "shaker.h"

int main()
{
	cs::HideConsole();
	//cs::ShowConsole();

	std::vector<sf::Color> palette
	{
		sf::Color(0xbda56cff),
		sf::Color(9,  38,  58),
		sf::Color(24,  95, 140),
		sf::Color(149, 194, 199),
		sf::Color(235, 213, 104),
		sf::Color(241, 113,  65),
	};

	auto settings = sf::ContextSettings::ContextSettings(0, 0, 8);
	sf::RenderWindow& window = sf::RenderWindow(sf::VideoMode(400, 400), "Shrimp Cactus Mate", 7U, settings);
	sf::View mainCamera(sf::Vector2f(0.f, 0.f), sf::Vector2f(16 * 5, 16 * 5));
	window.setView(mainCamera);

	sf::Texture shrimp;
	if (!shrimp.loadFromFile("assets/shrimp.png"))
	{
		cs::ShowConsole();
	}
	sf::Sprite shrimpet(shrimp);
	shrimpet.setOrigin((sf::Vector2f)shrimp.getSize() / 2.f);

	sf::Clock clock;
	float t = 0.f;

	while (window.isOpen())
	{
		sf::Event event;
		while (window.pollEvent(event))
		{
			if (event.type == sf::Event::Closed)
			{
				window.close();
			}
		}

		float dt = clock.restart().asSeconds();
		t += dt;

		window.clear(palette[0]);

		//shrimpet.rotate(90 * dt);
		window.draw(shrimpet);

		window.display();
	}
}