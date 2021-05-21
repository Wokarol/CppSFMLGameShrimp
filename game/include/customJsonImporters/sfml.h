#pragma once

#include <SFML/Graphics.hpp>
#include <json.hpp>

namespace sf
{
    template <class T>
    inline void from_json(const nlohmann::json& j, sf::Rect<T>& rect)
    {
        rect.left = j[0];
        rect.top = j[1];
        rect.width = j[2];
        rect.height = j[3];
    }

    template <class T>
    inline void from_json(const nlohmann::json& j, sf::Vector2<T>& rect)
    {
        rect.x = j[0];
        rect.y = j[1];
    }

    template <class T>
    inline void from_json(const nlohmann::json& j, sf::Vector3<T>& rect)
    {
        rect.x = j[0];
        rect.y = j[1];
        rect.z = j[2];
    }
}