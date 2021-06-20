#pragma once

#include <SFML/System/Vector2.hpp>

#include <actor.h>
#include <actors/player.h>

namespace game
{
    inline sf::Vector2f screenSize;
    inline wok::ActorHandle<wok::Player> player;
}