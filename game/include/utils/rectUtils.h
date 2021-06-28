#pragma once

#include <SFML/Graphics/Rect.hpp>

namespace ru
{
    template <class T>
    sf::Rect<T> combineRects(sf::Rect<T> a, sf::Rect<T> b)
    {
        float aRight = a.left + a.width;
        float aBottom = a.top + a.height;
        float bRight = b.left + b.width;
        float bBottom = b.top + b.height;

        sf::Rect<T> rect;
        rect.left = std::min(a.left, b.left);
        rect.top = std::min(a.top, b.top);

        rect.width = std::max(aRight, bRight) - rect.left;
        rect.height = std::max(aBottom, bBottom) - rect.top;

        return rect;
    }

    template <class T>
    sf::Rect<T> addMargin(sf::Rect<T> rect, sf::Vector2<T> margin)
    {
        rect.left += margin.x;
        rect.top += margin.y;
        rect.width -= margin.x * (T)2;
        rect.height -= margin.y * (T)2;
        return rect;
    }

    template <class T>
    sf::Rect<T> addMargin(sf::Rect<T> rect, T margin)
    {
        return addMargin<T>(rect, { margin, margin });
    }
}