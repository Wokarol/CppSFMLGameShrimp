#include <levelLoader.h>


#include <world.h>
#include <actors.h>

void levels::load(std::string_view name, World& world)
{
	std::stringstream groupName;
	groupName << "Level " << name;

	auto group = std::make_shared<Group>(groupName.str());
	for (size_t i = 0; i < name.size(); i++)
	{
		//Box box(50, sf::Vector2f(0, 0), sf::Color::Black, sf::Color::White);
		std::stringstream actorName;
		actorName << "Box " << name[i];
		world.createNamedActor<Box>(actorName.str(), 50, sf::Vector2f(0, 0), sf::Color::Black, sf::Color::White)
			->group = group;
	}
}
