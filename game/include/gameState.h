#pragma once

#include <SFML/System/Vector2.hpp>

#include <actor.h>
#include <actors/player.h>

namespace game
{
    namespace implementation
    {
        inline bool awaitsClosing = false;
    }

    inline sf::Vector2f screenSize;
    inline wok::ActorHandle<wok::Player> player;

    inline bool awaitsClosing()
    {
        return implementation::awaitsClosing;
    }

    inline void close()
    {
        implementation::awaitsClosing = true;
    }
}