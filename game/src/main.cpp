#include <iostream>
#include <fstream>

#include <SFML/Window.hpp>
#include <SFML/Graphics.hpp>

#include "console.h"

#include <filesystem>
namespace fs = std::filesystem;

int main()
{
    //HideConsole();
    ShowConsole();

    std::vector<sf::Color> palette
    {
        sf::Color(9,  38,  58),
        sf::Color(24,  95, 140),
        sf::Color(149, 194, 199),
        sf::Color(235, 213, 104),
        sf::Color(241, 113,  65),
    };

    auto settings = sf::ContextSettings::ContextSettings(0, 0, 4);
    sf::RenderWindow& window = sf::RenderWindow(sf::VideoMode(800, 600), "Premake!!!", 7U, settings);

    sf::CircleShape circle(50.0);

    circle.setPosition(400, 300);
    circle.setOrigin(25, 25);
    circle.setFillColor(palette[1]);

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

        window.clear(palette[0]);

        std::ifstream file("assets/foo.data");
        if (!file.good())
        {
            std::cout << "imma die" << std::endl;
        }
        float r = 0;
        file >> r;
        file.close();
        circle.setRadius(r);

        window.draw(circle);

        window.display();
    }
}