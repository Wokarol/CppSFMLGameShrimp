#pragma once

#include <cmath>
#include <SFML/Graphics.hpp>

namespace m
{
    constexpr auto PI = 3.14159f;

    inline sf::Vector2f normalize(const sf::Vector2f& v)
    {
        float length = (float)(v.x * v.x + v.y * v.y);
        if (length == 0.f)
            return v;
        length = sqrt(1.f / length);

        return sf::Vector2f(
            v.x * length,
            v.y * length
        );
    }

    inline sf::Vector2f rotate(const sf::Vector2f& v, float degrees)
    {
        degrees *= PI / 180.f;
        const float cs = std::cos(degrees);
        const float sn = std::sin(degrees);

        return sf::Vector2f(
            v.x * cs - v.y * sn,
            v.x * sn + v.y * cs
        );
    }

    inline float angle(const sf::Vector2f& v)
    {
        if (v.y == 0.f)
            return v.x < 0.f ? 180.f : 0.f;
        else if (v.x == 0.f)
            return v.y < 0.f ? 270.f : 90.f;

        if (v.y > 0.f)
            if (v.x > 0.f)
                return atan(v.y / v.x) * 180.f / PI;
            else
                return 180.f - atan(v.y / -v.x) * 180.f / PI;
        else
            if (v.x > 0)
                return 360.f - atan(-v.y / v.x) * 180.f / PI;
            else
                return 180.f + atan(-v.y / -v.x) * 180.f / PI;
    }

    inline float angle(const sf::Vector2f& a, const sf::Vector2f& b)
    {
        return m::angle(m::rotate(b, -m::angle(a)));
    }

    inline float sqrLength(const sf::Vector2f& v)
    {
        return v.x * v.x + v.y * v.y;
    }

    inline float length(const sf::Vector2f& v)
    {
        return std::sqrt(sqrLength(v));
    }

    inline float dist(const sf::Vector2f& a, const sf::Vector2f& b)
    {
        return length(a - b);
    }

    inline float sqrDist(const sf::Vector2f& a, const sf::Vector2f& b)
    {
        return sqrLength(a - b);
    }

    inline float dot(const sf::Vector2f& a, const sf::Vector2f& b)
    {
        return a.x * b.x + a.y * b.y;
    }

    inline sf::Vector2f limit(const sf::Vector2f& v, float maxLength)
    {
        if (length(v) <= maxLength)
        {
            return v;
        }
        
        auto n = normalize(v);
        return n * maxLength;
    }    
    
    inline sf::Vector2f setMag(const sf::Vector2f& v, float mag)
    {
        auto n = normalize(v);
        return n * mag;
    }

    template <typename T>
    inline T lerp(const T& a, const T& b, float t)
    {
        return a + (b - a) * t;
    }

    inline sf::Color lerp(const sf::Color& a, const sf::Color& b, float t)
    {
        t = std::clamp(t, 0.f, 1.f);

        return sf::Color(
            (sf::Uint8)(a.r + (b.r - a.r) * t),
            (sf::Uint8)(a.g + (b.g - a.g) * t),
            (sf::Uint8)(a.b + (b.b - a.b) * t),
            (sf::Uint8)(a.a + (b.a - a.a) * t)
        );
    }

    struct Ray
    {
        sf::Vector2f origin;
        sf::Vector2f direction;

        Ray(sf::Vector2f _origin, sf::Vector2f _direction) : origin(_origin), direction(m::normalize(_direction)) {}

        void rotateAround(sf::Vector2f pivot, float degrees)
        {
            auto diff = origin - pivot;
            origin = m::rotate(diff, degrees) + pivot;
            direction = m::rotate(direction, degrees);
        }
        void rotate(float degrees)
        {
            direction = m::rotate(direction, degrees);
        }
        void move(sf::Vector2f v)
        {
            origin += v;
        }

        sf::Vector2f getPoint(float distance)
        {
            return origin + direction * distance;
        }
    };
}