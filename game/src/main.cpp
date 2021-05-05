#include <fstream>

#include <SFML/Window.hpp>
#include <SFML/Graphics.hpp>

#include <console.h>
#include <shaker.h>
#include <mathUtils.h>
#include <intersections.h>
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
	mainCamera.zoom(0.5);
	window.setView(mainCamera);

	// Objects
	//sf::RectangleShape box(sf::Vector2f(80, 60));
	//box.setPosition(0, 0);
	//box.setOrigin(box.getSize() / 2.f);
	//box.setFillColor(sf::Color(0));
	//box.setOutlineColor(blue);
	//box.setOutlineThickness(1);

	////box.rotate(45);

	//sf::RectangleShape boxCollider(box.getSize() + sf::Vector2f(2.f, 2.f) * box.getOutlineThickness());
	//boxCollider.setPosition(box.getPosition());
	//boxCollider.setOrigin(boxCollider.getSize() / 2.f);
	//boxCollider.setRotation(box.getRotation());

	sf::CircleShape circle(30.f);
	circle.setPosition(50, 30);
	circle.setOrigin(sf::Vector2f(1.f, 1.f) * circle.getRadius());
	circle.setFillColor(sf::Color(0));
	circle.setOutlineColor(blue);
	circle.setOutlineThickness(1);

	sf::CircleShape circleCollider(circle.getRadius() + circle.getOutlineThickness());
	circleCollider.setPosition(circle.getPosition() - sf::Vector2f(1.f, 1.f));
	circleCollider.setOrigin(sf::Vector2f(1.f, 1.f) * circleCollider.getRadius());
	circleCollider.setRotation(circle.getRotation());

	sf::CircleShape caster(5.f);
	caster.setOrigin(sf::Vector2f(1.f, 1.f) * caster.getRadius());
	caster.setFillColor(white);
	caster.setPosition(-60, 0);

	sf::CircleShape hit(2.f);
	hit.setOrigin(sf::Vector2f(1.f, 1.f) * hit.getRadius());
	hit.setFillColor(blue);
	hit.setPosition(-200, 150);


	sf::LineShape line(3);
	sf::LineShape reflection(3);

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

		// Input
		sf::Vector2f casterVelocity;

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

		//// Logic
		//box.rotate(10 * dt);
		//box.setPosition(
		//	std::sin(t * 2.f) * 20.f,
		//	std::sin(t * 1.f + 2134.f) * 15.f
		//);
		//boxCollider.setPosition(box.getPosition());
		//boxCollider.setRotation(box.getRotation());

		circle.setPosition(
			std::sin(t * 1.f) * 10.f + 40.f,
			std::sin(t * 2.f + 223.6f) * 5.f + 50.f
		);
		circleCollider.setPosition(circle.getPosition());

		caster.move(casterVelocity * dt * 200.f);
		
		sf::Vector2f m = window.mapPixelToCoords(sf::Mouse::getPosition(window));
		caster.setRotation(m::angle(m - caster.getPosition()));

		auto ray = m::Ray(caster.getPosition(), sf::Vector2f(1.f, 0.f));
		ray.rotate(caster.getRotation());

		auto intersect = inter::rayCircle(ray, circleCollider);
		sf::Vector2f point;
		if (intersect.hit)
		{
			point = ray.getPoint(intersect.dist);
			float dirAngle = m::angle(-ray.dir, intersect.normal);

			reflection.setStart(point);
			reflection.setEnd(point + m::rotate(intersect.normal * 50.f, dirAngle));
		}
		else
		{
			point = ray.getPoint(10000.f);
		}

		hit.setPosition(point);

		line.setColor(m::lerp(white, yellow, std::sin(t * 5.f) * 0.5f + 0.5f));
		line.setThickness(m::lerp(0.5f, 2.f, std::sin((t * 6.f) + 216.37f) * 0.5f + 0.5f));

		reflection.setColor(line.getColor());
		reflection.setThickness(line.getThickness());

		line.setStart(ray.origin);
		line.setEnd(point);

		// Drawing
		window.clear(background);

		//window.draw(box);
		//window.draw(boxCollider);

		window.draw(circle);
		//window.draw(circleCollider);

		window.draw(line);
		if (intersect.hit)
		{
			window.draw(reflection);
		}
		window.draw(caster);
		//window.draw(hit);

		window.display();
	}
}