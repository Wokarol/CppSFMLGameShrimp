#pragma once

#include <SFML/Graphics.hpp>
#include <console.h>
#include <resources.h>
#include <world.h>
#include <input.h>

#include <actors.h>

#include <gameState.h>

inline void initializeBoilerplate()
{
    console::hide();
}

inline sf::RenderWindow createWindow()
{
    auto settings = sf::ContextSettings::ContextSettings(0, 0, 8);
    return sf::RenderWindow(sf::VideoMode(25 * 32, 16 * 32), "Surviving The Heat", 7U, settings);
}

inline void centreCamera(sf::RenderTarget& target)
{
    sf::View view(sf::Vector2f(0, 0), (sf::Vector2f)target.getSize());
    target.setView(view);
}

inline void setCornerCam(sf::RenderTarget& target)
{
    sf::Vector2f targetSize = (sf::Vector2f)target.getSize();
    float ratio = targetSize.x / targetSize.y;

    float height = 16 * 16;
    float width = ratio * height;
    sf::View view = sf::View(sf::FloatRect(0, 0, width, height));
    target.setView(view);

    gameState::screenSize = view.getSize();
}

inline void handleDebugKeys(const sf::Event& event)
{
    if (event.key.code == sf::Keyboard::F2)
    {
        if (console::isVisible())
        {
            console::hide();
            console::enableLock = true;
            wok::DebugPopup::create("Console locked");
        }
        else
        {
            console::enableLock = false;
            wok::DebugPopup::create("Console unlocked");
        }
    }
    if (event.key.code == sf::Keyboard::F3)
    {
        if (console::isVisible())
        {
            console::hide();
            wok::DebugPopup::create("Console hidden");
        }
        else
        {
            if (console::enableLock)
            {
                console::enableLock = false;
                wok::DebugPopup::create("Console unlocked");
            }
            if (event.key.shift)
            {
                console::showImportant();
            }
            else
            {
                console::show();
            }
            wok::DebugPopup::create("Console shown");
        }
    }
    if (event.key.code == sf::Keyboard::F4)
    {
        wok::input::slowMode ^= true;
        wok::DebugPopup::create("Slow mode ", (wok::input::slowMode ? "ON" : "OFF"));
    }
    if (event.key.code == sf::Keyboard::F5)
    {
        wok::res::reloadAll();
        wok::DebugPopup::create("Reloaded assets");
    }
    if (event.key.code == sf::Keyboard::F6)
    {
        bool& actors = wok::world::shouldDrawActors;
        bool& gizmos = wok::world::shouldDrawGizmos;

        if (actors && !gizmos)
        {
            gizmos = true;
            wok::DebugPopup::create("Gizmos and Actors");
        }
        else if (actors && gizmos)
        {
            actors = false;
            wok::DebugPopup::create("Just Gizmos");
        }
        else if (!actors)
        {
            actors = true;
            gizmos = false;
            wok::DebugPopup::create("Just Actors");
        }
    }

    if (event.key.code == sf::Keyboard::F10)
    {
        wok::world::dumpActors(!event.key.shift);
        wok::DebugPopup::create("Actors dumped");
    }
}

inline void handleEventsAndInput(sf::RenderWindow& window)
{
    wok::input::resetKeyStateBeforeTheFrame();

    sf::Event event;
    while (window.pollEvent(event))
    {
        if (event.type == sf::Event::Closed)
        {
            window.close();
        }
        if (event.type == sf::Event::KeyPressed)
        {
            handleDebugKeys(event);
            wok::input::handleInputKeysPressed(event);
        }
        if (event.type == sf::Event::MouseButtonPressed)
        {
            wok::input::handleMouseButtonsPressed(event);
        }
        if (event.type == sf::Event::KeyReleased)
        {
            wok::input::handleInputKeysReleased(event);
        }
        if (event.type == sf::Event::MouseButtonReleased)
        {
            wok::input::handleMouseButtonsReleased(event);
        }
        if (event.type == sf::Event::MouseMoved)
        {
            sf::Vector2i mousePos;
            mousePos.x = event.mouseMove.x;
            mousePos.y = event.mouseMove.y;
            wok::input::mousePositionInWorld = window.mapPixelToCoords(mousePos);
        }
    }
}
