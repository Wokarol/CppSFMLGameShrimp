#include <fstream>

#include <gameClock.h>
#include <console.h>

#include <filesystem>
#include <json.hpp>
#include <utils/jsonHelpers.h>

#include "windowManagement.h"
#include <world.h>
#include <actors/staticBox.h>

#include <levelLoader.h>
#include <resources.h>
#include <projectSettings.h>

struct Pallete
{
    sf::Color background = sf::Color(0x333333FF);
    sf::Color white = sf::Color(0xFFFFFFFF);
    sf::Color blue = sf::Color(0x90CBFBFF);
    sf::Color darkBlue = sf::Color(0x110066FF);
    sf::Color yellow = sf::Color(0xFFD432FF);
};

bool startGame()
{
    std::string configPath = "assets/start.config";
    if (!std::filesystem::exists(configPath))
    {
        console::error("start.config cannot be found in assets folder");
        return false;
    }

    nlohmann::json config;
    {
        std::ifstream configFile(configPath);
        configFile >> config;
    }

    try
    {
        std::string levelToLoad;
        if (!wok::tryGetString(config, "start_level", levelToLoad))
        {
            console::error("Key 'start_level' was not found");
            return false;
        }

        wok::project::init(config);

        wok::levels::load(levelToLoad);
    }
    catch (const std::exception& e)
    {
        console::error(e.what());
        return false;
    }
    return true;
}

int main()
{
    srand((unsigned int)time(NULL));

    initializeBoilerplate();

    auto window = createWindow();
    //centreCamera(window);
    setCornerCam(window);

    Pallete colors;
    wok::GameClock time;

    wok::world::shouldLog = false;

    if (!startGame())
    {
        window.close();
        std::cout << std::endl;
        system("pause");
    }

    wok::world::createNamedActor<wok::StaticBox>("Box 1",
        sf::Vector2f(100, 100),
        sf::Vector2f(36, 36));


    wok::world::createNamedActor<wok::StaticBox>("Box 3",
        sf::Vector2f(120, 120),
        sf::Vector2f(36, 36));

    wok::world::createNamedActor<wok::StaticBox>("Box 4",
        sf::Vector2f(156, 156),
        sf::Vector2f(36, 36));

    while (window.isOpen())
    {
        handleEventsAndInput(window);
        time.Tick();

        wok::world::update(time);

        window.clear(colors.background);
        auto states = sf::RenderStates();
        wok::world::draw(window, states);

        sf::View view = window.getView();
        window.setView(window.getDefaultView());

        sf::Color pressed(0xFF0000FF);
        sf::Color notPressed(0xFF000066);
        sf::Vector2f bottomLeft(30.f, window.getSize().y - 30.f);
        sf::Vector2f cellSize(50, 50);
        float spacing = 10;

        sf::RectangleShape rect(cellSize);
        rect.setOrigin(0, cellSize.y);

        rect.setPosition(bottomLeft.x, bottomLeft.y);
        rect.setFillColor(sf::Keyboard::isKeyPressed(sf::Keyboard::A) ? pressed : notPressed);
        window.draw(rect);

        rect.setPosition(bottomLeft.x + (spacing + cellSize.x) * 2.f, bottomLeft.y);
        rect.setFillColor(sf::Keyboard::isKeyPressed(sf::Keyboard::D) ? pressed : notPressed);
        window.draw(rect);

        rect.setPosition(bottomLeft.x + spacing + cellSize.x, bottomLeft.y - spacing - cellSize.y);
        rect.setFillColor(sf::Keyboard::isKeyPressed(sf::Keyboard::W) ? pressed : notPressed);
        window.draw(rect);

        rect.setPosition(bottomLeft.x + spacing + cellSize.x, bottomLeft.y);
        rect.setFillColor(sf::Keyboard::isKeyPressed(sf::Keyboard::S) ? pressed : notPressed);
        window.draw(rect);

        window.setView(view);
        window.display();
    }

    wok::world::clear();
    wok::res::clear();
}
