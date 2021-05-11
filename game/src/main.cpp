#include <fstream>

#include <gameClock.h>
#include <console.h>
#include "shapes/line.h"
#include "windowManagement.h"

#include "actorHandle.h"
#include "actorManagement/world.h"
#include "actors.h"

struct Pallete
{
	sf::Color background = sf::Color(0x333333FF);
	sf::Color white = sf::Color(0xFFFFFFFF);
	sf::Color blue = sf::Color(0x90CBFBFF);
	sf::Color darkBlue = sf::Color(0x110066FF);
	sf::Color yellow = sf::Color(0xFFD432FF);
};


int main()
{
	initializeBoilerplate();

	auto& window = createWindow();
	centreCamera(window);

	Pallete colors;
	GameClock time;
	World world;

	//world.logging = true;

	auto logger = world.createActor<LoggerActor>();
	auto box = logger.as<Box>();
	//auto box = world.createNamedActor<Box>("The Box", 50);

	while (window.isOpen())
	{
		handleEvents(window);
		time.Tick();

		if (sf::Keyboard::isKeyPressed(sf::Keyboard::Space))
		{
			if(logger.isValid())
			{
				logger.destroy();
			}
		}

		if (sf::Keyboard::isKeyPressed(sf::Keyboard::P))
		{
			if (box.isValid())
			{
				box.destroy();
			}
		}

		world.update(time);

		window.clear(sf::Color::Black);
		world.draw(window);
		window.display();
	}
}