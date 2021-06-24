#include <fstream>

#include <gameClock.h>
#include <console.h>

#include <filesystem>
#include <json.hpp>
#include <utils/jsonHelpers.h>

#include "windowManagement.h"
#include <world.h>
#include <actors/staticBox.h>

#include <sceneLoader.h>
#include <resources.h>
#include <projectSettings.h>
#include <gameState.h>

struct Pallete
{
    sf::Color background = sf::Color(0x333333FF);
    sf::Color white = sf::Color(0xFFFFFFFF);
    sf::Color blue = sf::Color(0x90CBFBFF);
    sf::Color darkBlue = sf::Color(0x110066FF);
    sf::Color yellow = sf::Color(0xFFD432FF);
};

bool loadGameConfig()
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
        wok::project::init(config);
    }
    catch (const std::exception& e)
    {
        console::error(e.what());
        return false;
    }
    return true;
}

bool startGame()
{

    wok::scenes::loadMenu();
    return true;
}

int main()
{
    srand((unsigned int)time(NULL));

    initializeBoilerplate();

    auto window = createWindow();
    //centreCamera(window);
    //setCornerCam(window);

    Pallete colors;
    wok::GameClock time;

    wok::world::shouldLog = false;

    bool configLoadedSuccesfully = loadGameConfig();
    if (!configLoadedSuccesfully)
    {
        window.close();
        console::pause();
    }

    bool gameStartedSuccesfully = startGame();
    if (!gameStartedSuccesfully)
    {
        window.close();
        console::pause();
    }

    while (window.isOpen())
    {
        handleEventsAndInput(window);
        time.Tick();

        sf::Vector2f screenSize = (sf::Vector2f)window.getSize();
        window.setView(sf::View({ 0.f, 0.f, screenSize.x, screenSize.y }));

        wok::world::update(time);

        window.clear(colors.background);
        auto states = sf::RenderStates();
        wok::world::draw(window, states);

        //sf::View view = window.getView();
        //window.setView(window.getDefaultView());

        //sf::Color pressed(0xFF0000FF);
        //sf::Color notPressed(0xFF000066);
        //sf::Vector2f bottomLeft(30.f, window.getSize().y - 30.f);
        //sf::Vector2f cellSize(50, 50);
        //float spacing = 10;

        //sf::RectangleShape rect(cellSize);
        //rect.setOrigin(0, cellSize.y);

        //rect.setPosition(bottomLeft.x, bottomLeft.y);
        //rect.setFillColor(sf::Keyboard::isKeyPressed(sf::Keyboard::A) ? pressed : notPressed);
        //window.draw(rect);

        //rect.setPosition(bottomLeft.x + (spacing + cellSize.x) * 2.f, bottomLeft.y);
        //rect.setFillColor(sf::Keyboard::isKeyPressed(sf::Keyboard::D) ? pressed : notPressed);
        //window.draw(rect);

        //rect.setPosition(bottomLeft.x + spacing + cellSize.x, bottomLeft.y - spacing - cellSize.y);
        //rect.setFillColor(sf::Keyboard::isKeyPressed(sf::Keyboard::W) ? pressed : notPressed);
        //window.draw(rect);

        //rect.setPosition(bottomLeft.x + spacing + cellSize.x, bottomLeft.y);
        //rect.setFillColor(sf::Keyboard::isKeyPressed(sf::Keyboard::S) ? pressed : notPressed);
        //window.draw(rect);

        //window.setView(view);
        window.display();
    }

    wok::world::clear();
    wok::res::clear();
}
