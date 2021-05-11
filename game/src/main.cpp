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
	srand((unsigned int)time(NULL));

	initializeBoilerplate();

	auto& window = createWindow();
	centreCamera(window);

	Pallete colors;
	GameClock time;
	World world;

	auto& rightBoxes = Group::create("Right boxes");
	auto& leftBoxes = Group::create("Left boxes");
	for (size_t i = 0; i < 30; i++)
	{
		float x = rand() % 500 - 250;
		float y = rand() % 500 - 250;
		auto& handle = world.createActor<Box>(
			50, 
			sf::Vector2f(x, y),
			colors.darkBlue,
			colors.blue
		);
		handle->group = x > 0 ? rightBoxes : leftBoxes;
	}

	while (window.isOpen())
	{
		handleEvents(window);
		time.Tick();

		if (sf::Keyboard::isKeyPressed(sf::Keyboard::Right))
		{
			world.destroyGroup(rightBoxes);
			rightBoxes = {};
		}
		if (sf::Keyboard::isKeyPressed(sf::Keyboard::Left))
		{
			world.destroyGroup(leftBoxes);
			leftBoxes = {};
		}

		world.update(time);

		window.clear(colors.background);
		world.draw(window);
		window.display();
	}
}