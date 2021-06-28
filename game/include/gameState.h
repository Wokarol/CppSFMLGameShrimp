#pragma once

#include <SFML/System/Vector2.hpp>

#include <actor.h>
#include <actors/player.h>
#include <actors/camera.h>

#include <fader.h>
#include <viewportCamera.h>
#include <scoreCounter.h>

#include <json.hpp>
#include <fstream>
#include <filesystem>

namespace game
{
    namespace implementation
    {
        inline bool awaitsClosing = false;
        inline wok::ActorHandle<wok::Camera> camera;
    }

    inline wok::ActorHandle<wok::Player> player;
    inline wok::Fader fader;
    inline bool dummyKilled;
    inline sf::FloatRect mapRect;
    inline wok::ScoreCounter score;

    inline void setActiveCamera(wok::ActorHandle<wok::Camera> camera)
    {
        implementation::camera = camera;
    }

    inline const wok::ViewportCamera& getCurrentCamera()
    {
        if (implementation::camera.isValid())
        {
            return *implementation::camera;
        }
        else
        {
            return wok::DefaultViewportCamera();
        }
    }

    inline bool awaitsClosing()
    {
        return implementation::awaitsClosing;
    }

    inline void close()
    {
        implementation::awaitsClosing = true;
    }

    inline void saveState()
    {
        nlohmann::json gameData;
        gameData["highscore"] = score.getHighscore();

        {
            std::ofstream saveFile("save.sav");
            saveFile << gameData.dump();
        }
    }

    inline void loadState()
    {
        if (std::filesystem::exists("save.sav"))
        {
            nlohmann::json gameData;
            std::ifstream saveFile("save.sav");
            saveFile >> gameData;

            console::log("Found file to load!");
            score = wok::ScoreCounter(gameData.at("highscore"));
        }
        else
        {
            console::log("No file to load!");
            score = wok::ScoreCounter(0);
        }
    }
}
