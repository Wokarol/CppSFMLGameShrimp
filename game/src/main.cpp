#include <fstream>

#include <gameClock.h>
#include <console.h>
#include <customShapes/line.h>

#include <actorHandle.h>
#include <actors.h>

#include "windowManagement.h"
#include "actorManagement/world.h"

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



	while (window.isOpen())
	{
		handleEvents(window, world);
		time.Tick();

		world.update(time);

		window.clear(colors.background);
		world.draw(window);
		window.display();
	}
}