#pragma once

#include <SFML/System/Vector2.hpp>

#include <actor.h>
#include <actors/player.h>
#include <actors/camera.h>

#include <fader.h>
#include <viewportCamera.h>

namespace game
{
    namespace implementation
    {
        inline bool awaitsClosing = false;
        inline wok::ActorHandle<wok::Camera> camera;
    }

    inline sf::Vector2f screenSize;
    inline wok::ActorHandle<wok::Player> player;
    inline wok::Fader fader;
    inline bool dummyKilled;

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
}
