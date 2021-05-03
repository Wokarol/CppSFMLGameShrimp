#include <fstream>

#include <SFML/Window.hpp>
#include <SFML/Graphics.hpp>

#include <console.h>
#include <shaker.h>
#include <mathUtils.h>
#include "shapes/line.h"

int main()
{
	// Initialization
	cs::HideConsole();

	sf::Color background(0x333333FF);
	sf::Color white(0xFFFFFFFF);
	sf::Color blue(0x90CBFBFF);
	sf::Color darkBlue(0x110066FF);
	sf::Color yellow(0xFFD432FF);

	auto settings = sf::ContextSettings::ContextSettings(0, 0, 8);
	sf::RenderWindow& window = sf::RenderWindow(sf::VideoMode(600, 600), "Shrimp Cactus Mate", 7U, settings);

	sf::View mainCamera(sf::Vector2f(0.f, 0.f), (sf::Vector2f)window.getSize());
	window.setView(mainCamera);

	// Objects
	sf::RectangleShape box(sf::Vector2f(200, 100));
	box.setOrigin(box.getSize() / 2.f);
	box.setFillColor(sf::Color(0));
	box.setOutlineColor(blue);
	box.setOutlineThickness(4);

	sf::CircleShape caster(10.f);
	caster.setOrigin(sf::Vector2f(1.f, 1.f) * caster.getRadius());
	caster.setFillColor(white);
	caster.setPosition(-200, 150);


	sf::LineShape line(3);

	// Loop
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

		float rotationVelocity = 0;
		sf::Vector2f casterVelocity;
		// Input
		if (sf::Keyboard::isKeyPressed(sf::Keyboard::Q))
		{
			rotationVelocity -= 1.f;
		}
		if (sf::Keyboard::isKeyPressed(sf::Keyboard::E))
		{
			rotationVelocity += 1.f;
		}

		if (sf::Keyboard::isKeyPressed(sf::Keyboard::D))
		{
			casterVelocity.x += 1.f;
		}
		if (sf::Keyboard::isKeyPressed(sf::Keyboard::A))
		{
			casterVelocity.x -= 1.f;
		}
		if (sf::Keyboard::isKeyPressed(sf::Keyboard::S))
		{
			casterVelocity.y += 1.f;
		}
		if (sf::Keyboard::isKeyPressed(sf::Keyboard::W))
		{
			casterVelocity.y -= 1.f;
		}

		float dt = clock.restart().asSeconds();
		t += dt;

		// Logic
		caster.move(casterVelocity * dt * 100.f);
		caster.rotate(rotationVelocity * dt * 90.f);

		line.setColor(m::lerp(white, yellow, std::sin(t * 5.f) * 0.5f + 0.5f));
		line.setThickness(m::lerp(1.f, 3.f, std::sin((t * 5.f) + 216.37f) * 0.5f + 0.5f));

		line.setStart(caster.getPosition());
		line.setEnd(caster.getPosition() + m::rotate(sf::Vector2f(10000.f, 0.f), caster.getRotation()));

		// Drawing
		window.clear(background);

		window.draw(box);
		window.draw(line);
		window.draw(caster);

		window.display();
	}
}