#include <fstream>

#include <gameClock.h>
#include "shapes/line.h"
#include "windowManagement.h"

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

	Pallete colors;
	auto& window = createWindow();
	GameClock time;
	World world;

	world.emplace<LoggerActor>();

	while (window.isOpen())
	{
		handleEvents(window);
		time.Tick();

		world.update(time);
	}
}