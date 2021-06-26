#include <fstream>

#include <gameClock.h>
#include <console.h>

#include <filesystem>
#include <json.hpp>
#include <utils/jsonHelpers.h>

#include <windowManagement.h>
#include <world.h>
#include <actors/staticBox.h>

#include <sceneLoader.h>
#include <resources.h>
#include <projectSettings.h>
#include <gameState.h>
#include <fader.h>

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
    game::fader = wok::Fader();

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
        if (game::awaitsClosing())
        {
            window.close();
            break;
        }

        handleEventsAndInput(window);
        time.Tick();

        sf::Vector2f screenSize = (sf::Vector2f)window.getSize();
        window.setView(game::getCurrentCamera().getView(screenSize));

        wok::world::update(time);
        game::fader.update(time);

        window.clear(colors.background);
        auto states = sf::RenderStates();
        wok::world::draw(window, states);

        game::fader.draw(window, screenSize);

        window.display();
    }

    wok::world::clear();
    wok::res::clear();
    game::fader = {};
}
