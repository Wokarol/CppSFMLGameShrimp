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


    for (size_t i = 0; i < 200; i++)
    {
        wok::world::createNamedActor<wok::StaticBox>("Box",
            sf::Vector2f(200, 100),
            sf::Vector2f(20, 20));
    }

    while (window.isOpen())
    {
        handleEventsAndInput(window);
        time.Tick();

        wok::world::update(time);

        window.clear(colors.background);
        auto states = sf::RenderStates();
        wok::world::draw(window, states);

        window.display();
    }

    wok::world::clear();
    wok::res::clear();
}
