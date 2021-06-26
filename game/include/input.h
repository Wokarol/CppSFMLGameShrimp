#pragma once

#include <SFML/Graphics.hpp>

namespace wok
{
    struct input
    {
        template <class T>
        struct key
        {
            const T boundKey;

            bool wasPressedThisFrame = false;
            bool wasReleasedThisFrame = false;
            bool isPressed = false;

            key(T key) : boundKey(key) {}

            void resetSingleFrameData()
            {
                wasPressedThisFrame = false;
                wasReleasedThisFrame = false;
            }
            void checkForKeyPressed(T key)
            {
                if (key == boundKey)
                {
                    wasPressedThisFrame = true;
                    isPressed = true;
                }
            }
            void checkForKeyReleased(T key)
            {
                if (key == boundKey)
                {
                    wasReleasedThisFrame = true;
                    isPressed = false;
                }
            }
        };

        static inline sf::Vector2f movement;
        static inline sf::Vector2f mousePositionInWorld;

        static inline key<sf::Mouse::Button> attack{ sf::Mouse::Left };
        static inline key<sf::Mouse::Button> knockback{ sf::Mouse::Right };
        static inline bool slowMode;

        static void handleInputKeysPressed(const sf::Event& event);
        static void handleInputKeysReleased(const sf::Event& event);
        static void handleMouseButtonsPressed(const sf::Event& event);
        static void handleMouseButtonsReleased(const sf::Event& event);
        static void onMouseMoved();
        static void resetKeyStateBeforeTheFrame();

        static const sf::Mouse::Button uiMouseButton = sf::Mouse::Button::Left;
    };
}
